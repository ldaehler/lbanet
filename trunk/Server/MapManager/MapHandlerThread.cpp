/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/


#include "MapHandlerThread.h"
#include "InfosReceiverServant.h"
#include "ActorReceiverServant.h"

#include "ContainerActor.h"

#include "ServerSignaler.h"
#include "MapManagerServant.h"
#include "HurtArea.h"
#include "DoorActor.h"
#include "NPCActor.h"
#include "ScriptedZoneActor.h"

/***********************************************************
	Constructor
***********************************************************/
MapHandlerThread::MapHandlerThread(SharedData	* SD, ActorsObserverPrx	publisher,
										const std::string &mapName, MapManagerServant *	stopper,
										const std::map<long, Actor *> & actors)
: _SD(SD), _publisher(publisher), _stopping(false), _mapName(mapName),
	_stopper(stopper), _actors(actors)
{
	_signaler = new ServerSignaler(this);
	std::map<long, Actor *>::iterator it =	_actors.begin();
	std::map<long, Actor *>::iterator end =	_actors.end();
	for(; it != end; ++it)
		it->second->SetSignaler(_signaler);
}


/***********************************************************
	destructor
***********************************************************/
MapHandlerThread::~MapHandlerThread()
{
	std::map<long, Actor *>::iterator it = _actors.begin();
	std::map<long, Actor *>::iterator end = _actors.end();
	for(;it != end; ++it)
		it->second->SetSignaler(NULL);
		//delete it->second;

	delete _signaler;
}




/***********************************************************
	callback function called when a message is received from IceStorm
***********************************************************/
void MapHandlerThread::UpdatedInfo(const LbaNet::ActorInfo& asi)
{
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator it = _players.find(asi.ActorId);
	if(it != _players.end())
		it->second.first = asi;
}



/***********************************************************
	a player join a map
***********************************************************/
void MapHandlerThread::Join(const ActorLifeInfo &PlayerId)
{
	_players[PlayerId.ActorId] = std::make_pair(LbaNet::ActorInfo(), PlayerId);
	_stopping = false;
}

/***********************************************************
 a player leave a map
***********************************************************/
bool MapHandlerThread::Leave(const ActorLifeInfo &PlayerId)
{
	// remove unlocked doors
	std::map<Ice::Long, std::vector<Ice::Long> >::iterator itdel = _unlocked.find(PlayerId.ActorId);
	if(itdel != _unlocked.end())
		_unlocked.erase(itdel);

	//desactivate actors
	std::map<Ice::Long, Ice::Long>::iterator it =_todeactivate.find(PlayerId.ActorId);
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(PlayerId.ActorId);

	if(it != _todeactivate.end())
	{
		if(itp != _players.end())
		{
			_actors[(long)it->second]->ProcessDesactivation(itp->second.first.X, itp->second.first.Y, itp->second.first.Z, itp->second.first.Rotation);

			if(_publisher)
			{
				LbaNet::ActorActivationInfo ai;
				ai.Activate = false;
				ai.ActivatedId = it->second;
				ai.ActorId = it->first;
				ai.X = itp->second.first.X;
				ai.Y = itp->second.first.Y;
				ai.Z = itp->second.first.Z;
				ai.Rotation = itp->second.first.Rotation;
				_publisher->ActivatedActor(ai);
			}
		}

		_todeactivate.erase(it);
	}


	//close container
	std::map<Ice::Long, std::pair<Ice::Long, time_t> >::iterator itlocked = _lockedContainers.begin();
	while(itlocked != _lockedContainers.end())
	{
		if(itlocked->second.first == PlayerId.ActorId)
		{
			_lockedContainers.erase(itlocked);
			itlocked = _lockedContainers.begin();
		}
		else
			++itlocked;
	}


	// untarget actors
	std::map<Ice::Long, Ice::Long>::iterator ittargeted =_targetedactor.find(PlayerId.ActorId);
	if(ittargeted != _targetedactor.end())
	{
		std::map<long, Actor *>::iterator itact = _actors.find(ittargeted->second);
		if(itact != _actors.end())
			itact->second->UpdateTargetedActor(PlayerId.ActorId, false);

		_targetedactor.erase(ittargeted);
	}



	if(itp != _players.end())
		_players.erase(itp);


	if(_players.size() == 0)
	{
		_stopping = true;
		_timerstart = IceUtil::Time::now().toMilliSecondsDouble();
	}

	return (_players.size() == 0);
}


/***********************************************************
	callback function called when an actor id activated
***********************************************************/
void MapHandlerThread::ActivateActor(const ActorActivationInfoWithCallback& ai)
{
	std::map<long, Actor *>::iterator it =	_actors.find((long)ai.ainfo.ActivatedId);
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(ai.ainfo.ActorId);
	if(it != _actors.end() && itp != _players.end())
	{
		if(ai.ainfo.Activate)
		{
			if(it->second->GetType() == 4) //if it is a door
			{
				DoorActor * dact = static_cast<DoorActor *>(it->second);
				if(dact->GetLocked())
				{
					std::vector<Ice::Long> &vecunlocked =_unlocked[ai.ainfo.ActorId];
					// check if player already unlocked the door
					if(std::find(vecunlocked.begin(), vecunlocked.end(), ai.ainfo.ActivatedId) == vecunlocked.end())
					{
						// if we do not have the key - can not activate the door
						if(!ai.clientPtr || !ai.clientPtr->HasItem(dact->GetKeyId(), 1))
							return;

						// add player unlocked door to memory
						vecunlocked.push_back(ai.ainfo.ActivatedId);

						if(dact->GetDesKey())
						{
							LbaNet::UpdatedItemSeq InventoryChanges;
							LbaNet::UpdatedItem itm;
							itm.ItemId = dact->GetKeyId();
							itm.NewCount = -1;
							itm.InformPlayer = true;
							InventoryChanges.push_back(itm);
							ai.clientPtr->ApplyInventoryChanges(InventoryChanges);
						}
					}
				}
			}

			if(it->second->GetType() == 13) //if it is a scriptable zone
			{
				ScriptedZoneActor * dact = static_cast<ScriptedZoneActor *>(it->second);
				long item = dact->GetNeededItemId();
				if(item >= 0)
				{
					// if we do not have the item - can not activate
					if(!ai.clientPtr || !ai.clientPtr->HasItem(item, 1))
						return;

					// if needed - destroy the item
					if(dact->GetDesItem())
					{
						LbaNet::UpdatedItemSeq InventoryChanges;
						LbaNet::UpdatedItem itm;
						itm.ItemId = item;
						itm.NewCount = -1;
						itm.InformPlayer = true;
						InventoryChanges.push_back(itm);
						ai.clientPtr->ApplyInventoryChanges(InventoryChanges);
					}
				}
			}

			it->second->ProcessActivation(ai.ainfo.X, ai.ainfo.Y, ai.ainfo.Z, ai.ainfo.Rotation);

			if(it->second->NeedDesactivation())
				_todeactivate[ai.ainfo.ActorId] = ai.ainfo.ActivatedId;
		}
		else
		{
			it->second->ProcessDesactivation(ai.ainfo.X, ai.ainfo.Y, ai.ainfo.Z, ai.ainfo.Rotation);
			std::map<Ice::Long, Ice::Long>::iterator itpd =_todeactivate.find(ai.ainfo.ActorId);
			if(itpd != _todeactivate.end())
				_todeactivate.erase(itpd);
		}

		if(_publisher)
			_publisher->ActivatedActor(ai.ainfo);
	}

}


/***********************************************************
	callback function called when an actor id signaled
***********************************************************/
void MapHandlerThread::SignalActor(const LbaNet::ActorSignalInfo& ai)
{
	if(_publisher)
		_publisher->SignaledActor(ai);

	for(size_t i=0; i< ai.Targets.size(); ++i)
	{
		std::map<long, Actor *>::iterator it =	_actors.find((long)ai.Targets[i]);
		if(it != _actors.end())
			it->second->OnSignal(ai.SignalId);
	}
}


/***********************************************************
send signal
***********************************************************/
void MapHandlerThread::SendSignal(long signal, const std::vector<long> &targets)
{
	std::vector<long>::const_iterator it = targets.begin();
	std::vector<long>::const_iterator end = targets.end();
	for(; it != end; ++it)
	{
		std::map<long, Actor *>::iterator itm = _actors.find(*it);
		if(itm != _actors.end())
			itm->second->OnSignal(signal);
	}
}


/***********************************************************
running thread
***********************************************************/
void MapHandlerThread::run()
{
	// init time
	_lasttime = IceUtil::Time::now().toMilliSecondsDouble();


	// forever loop until thread is terminated
	while(_SD->WaitForTime(IceUtil::Time::milliSeconds(20)))
	{


		// get updates
		std::vector<std::pair<ActorLifeInfo, bool> >  joinedmap;
		std::vector<LbaNet::ActorInfo>  pinfos;
		std::vector<ActorActivationInfoWithCallback>  ainfos;
		std::vector<LbaNet::ActorSignalInfo>  sinfos;
		std::vector<std::pair<Ice::Long, Ice::Long> > hurtinfos;
		std::vector<std::pair<Ice::Long, Ice::Float> > hurtfallinfos;
		std::vector<Ice::Long> raisedinfos;
		std::vector<LifeManaInfo> lfminfos;
		std::vector<ContainerQueryInfo> conqinfos;
		std::vector<ContainerUpdateInfo> conuinfos;
		std::vector<TargetedActorPlayer> targetedinfos;
		std::vector<TargetedActorPlayer> untargetedinfos;

		_SD->GetJoined(joinedmap);
		_SD->GetUpdatedinfo(pinfos);
		_SD->GetActorinfo(ainfos);
		_SD->GetSignalinfo(sinfos);
		_SD->GetHurtedPlayer(hurtinfos);
		_SD->GetHurtedFallPlayer(hurtfallinfos);
		_SD->GetRaisedFromDead(raisedinfos);
		_SD->GetAllUpdateLifeMana(lfminfos);
		_SD->GetAllContainerQuerys(conqinfos);
		_SD->GetAllContainerUpdates(conuinfos);
		_SD->GetAllTargetedActors(targetedinfos);
		_SD->GetAllUntargetedActors(untargetedinfos);

		Lock sync(*this);
		// update state
		{

			// player raised
			{
				std::vector<Ice::Long>::const_iterator it = raisedinfos.begin();
				std::vector<Ice::Long>::const_iterator end = raisedinfos.end();
				for(; it != end; ++it)
				{
					Raised(*it);
				}
			}

			// player hurt
			{
				std::vector<std::pair<Ice::Long, Ice::Long> >::const_iterator it = hurtinfos.begin();
				std::vector<std::pair<Ice::Long, Ice::Long> >::const_iterator end = hurtinfos.end();
				for(; it != end; ++it)
				{
					Hurt(it->first, it->second);
				}
			}

			// player fall hurt
			{
				std::vector<std::pair<Ice::Long, Ice::Float> >::const_iterator it = hurtfallinfos.begin();
				std::vector<std::pair<Ice::Long, Ice::Float> >::const_iterator end = hurtfallinfos.end();
				for(; it != end; ++it)
				{
					HurtFall(it->first, it->second);
				}
			}


			// player join/leave
			{
				std::vector<std::pair<ActorLifeInfo, bool> >::const_iterator it = joinedmap.begin();
				std::vector<std::pair<ActorLifeInfo, bool> >::const_iterator end = joinedmap.end();
				for(; it != end; ++it)
				{
					if(it->second)
						Join(it->first);
					else
						Leave(it->first);
				}
			}

			// actor info
			{
				std::vector<ActorActivationInfoWithCallback>::const_iterator it = ainfos.begin();
				std::vector<ActorActivationInfoWithCallback>::const_iterator end = ainfos.end();
				for(; it != end; ++it)
					ActivateActor(*it);
			}

			// signal info
			{
				std::vector<LbaNet::ActorSignalInfo>::const_iterator it = sinfos.begin();
				std::vector<LbaNet::ActorSignalInfo>::const_iterator end = sinfos.end();
				for(; it != end; ++it)
					SignalActor(*it);
			}


			// player info
			{
				std::vector<LbaNet::ActorInfo>::const_iterator it = pinfos.begin();
				std::vector<LbaNet::ActorInfo>::const_iterator end = pinfos.end();
				for(; it != end; ++it)
					UpdatedInfo(*it);
			}


			//life mana info
			{
				std::vector<LifeManaInfo>::const_iterator it = lfminfos.begin();
				std::vector<LifeManaInfo>::const_iterator end = lfminfos.end();
				for(; it != end; ++it)
					UpdateLifeMana(*it);
			}


			//container query info
			{
				std::vector<ContainerQueryInfo>::const_iterator it = conqinfos.begin();
				std::vector<ContainerQueryInfo>::const_iterator end = conqinfos.end();
				for(; it != end; ++it)
					UpdateContainerQuery(*it);
			}

			//container update info
			{
				std::vector<ContainerUpdateInfo>::const_iterator it = conuinfos.begin();
				std::vector<ContainerUpdateInfo>::const_iterator end = conuinfos.end();
				for(; it != end; ++it)
					UpdateContainerUpdate(*it);
			}

			//targeted update info
			{
				std::vector<TargetedActorPlayer>::const_iterator it = targetedinfos.begin();
				std::vector<TargetedActorPlayer>::const_iterator end = targetedinfos.end();
				for(; it != end; ++it)
					UpdateTargetedActor(*it, true);
			}

			//untargeted update info
			{
				std::vector<TargetedActorPlayer>::const_iterator it = untargetedinfos.begin();
				std::vector<TargetedActorPlayer>::const_iterator end = untargetedinfos.end();
				for(; it != end; ++it)
					UpdateTargetedActor(*it, false);
			}
		}


		// process actors
		double tnow = IceUtil::Time::now().toMilliSecondsDouble();
		float tdiff = (float)(tnow - _lasttime);
		_lasttime = tnow;

		std::map<long, Actor *>::iterator it =_actors.begin();
		std::map<long, Actor *>::iterator end =_actors.end();
		for(; it != end; ++it)
		{
			it->second->Process(tnow, tdiff);
			if(it->second->NeedsUpdate())
			{
				ActorStateInfo currState;
				if(it->second->Getstate(currState))
				{
					LbaNet::ActorUpdateInfo ui;
					ui.ActorId = currState.ActorId;
					ui.On = currState.On;
					ui.Open = currState.Open;
					ui.Counter = currState.Counter;
					ui.SignalOn = currState.SignalOn;
					ui.CurrentScript = currState.CurrentScript;
					ui.X = currState.X;
					ui.Y = currState.Y;
					ui.Z = currState.Z;
					ui.Rotation = currState.Rotation;
					ui.Visible = currState.Visible;

					std::vector<std::pair<long, long>>::iterator ittar = currState.Targets.begin();
					std::vector<std::pair<long, long>>::iterator endtar = currState.Targets.end();
					for(; ittar != endtar; ++ittar)
					{
						LbaNet::TargetedInfo tinfo;
						tinfo.TargetActorId = ittar->first;
						tinfo.TargetPlayerId = ittar->second;
						ui.Targets.push_back(tinfo);
					}

					std::vector<long>::const_iterator itcs = currState.CurrentSignals.begin();
					std::vector<long>::const_iterator endcs = currState.CurrentSignals.end();
					for(; itcs != endcs; ++itcs)
						ui.CurrentSignals.push_back(*itcs);

					_publisher->UpdateActorState(ui);
				}
			}
		}

		// if no player since 2min - stop the thread
		if(_stopping && tnow -_timerstart > 120000)
			_stopper->StopThread(_mapName);
	}
}


/***********************************************************
get updated info
***********************************************************/
LbaNet::UpdateSeq MapHandlerThread::GetUpdatedInfo()
{
	LbaNet::UpdateSeq res;
	Lock sync(*this);

	std::map<long, Actor *>::iterator it =_actors.begin();
	std::map<long, Actor *>::iterator end =_actors.end();
	ActorStateInfo actState;
	for(; it != end; ++it)
	{
		if(it->second->Getstate(actState))
		{
			LbaNet::ActorUpdateInfo ui;
			ui.ActorId = it->first;
			ui.On = actState.On;
			ui.Open = actState.Open;
			ui.Counter = actState.Counter;
			ui.SignalOn = actState.SignalOn;
			ui.CurrentScript = actState.CurrentScript;
			ui.X = actState.X;
			ui.Y = actState.Y;
			ui.Z = actState.Z;
			ui.Rotation = actState.Rotation;
			ui.Visible = actState.Visible;

			std::vector<std::pair<long, long>>::iterator ittar = actState.Targets.begin();
			std::vector<std::pair<long, long>>::iterator endtar = actState.Targets.end();
			for(; ittar != endtar; ++ittar)
			{
				LbaNet::TargetedInfo tinfo;
				tinfo.TargetActorId = ittar->first;
				tinfo.TargetPlayerId = ittar->second;
				ui.Targets.push_back(tinfo);
			}

			std::vector<long>::iterator itcs = actState.CurrentSignals.begin();
			std::vector<long>::iterator endcs = actState.CurrentSignals.end();
			for(; itcs != endcs; ++itcs)
				ui.CurrentSignals.push_back(*itcs);

			res.push_back(ui);
		}
	}

	return res;
}

/***********************************************************
get player info
***********************************************************/
LbaNet::PlayerSeq MapHandlerThread::GetPlayersInfo()
{
	LbaNet::PlayerSeq res;
	Lock sync(*this);

	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator it =_players.begin();
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator end =_players.end();
	for(; it != end; ++it)
	{
		PlayerFullInfo pi;
		pi.ai = it->second.first;
		pi.li = it->second.second;
		res.push_back(pi);
	}

	return res;
}


/***********************************************************
get player life
***********************************************************/
LbaNet::ActorLifeInfo MapHandlerThread::GetPlayerLife(Ice::Long PlayerId)
{
	Lock sync(*this);
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(PlayerId);
	if(itp != _players.end())
	{
		return itp->second.second;
	}

	return LbaNet::ActorLifeInfo();
}


/***********************************************************
called when a player is hurted
***********************************************************/
void MapHandlerThread::Hurt(Ice::Long PlayerId, Ice::Long hurtingid)
{
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(PlayerId);
	std::map<long, Actor *>::iterator it =	_actors.find((long)hurtingid);
	if(itp != _players.end() && it != _actors.end())
	{
		if(it->second->GetType() == 11)
		{
			HurtArea * tmp = static_cast<HurtArea *>(it->second);
			itp->second.second.CurrentLife -= tmp->GetLifeTaken();

			if(_publisher)
				_publisher->UpdatedLife(itp->second.second);
		}
	}
}

/***********************************************************
called when a player is hurted
***********************************************************/
void MapHandlerThread::HurtFall(Ice::Long PlayerId, Ice::Float fallingdistance)
{
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(PlayerId);
	if(itp != _players.end())
	{
		if(fallingdistance > 0)
		{
			float hurtedlife = fallingdistance * 2;
			itp->second.second.CurrentLife -= hurtedlife;

			if(_publisher)
				_publisher->UpdatedLife(itp->second.second);
		}
	}
}


/***********************************************************
called when a player id dead and raised
***********************************************************/
void MapHandlerThread::Raised(Ice::Long PlayerId)
{
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(PlayerId);
	if(itp != _players.end())
	{
		itp->second.second.CurrentLife = itp->second.second.MaxLife;

		if(_publisher)
			_publisher->UpdatedLife(itp->second.second);
	}
}


/***********************************************************
object used
***********************************************************/
void MapHandlerThread::UpdateLifeMana(const LifeManaInfo &itinfo)
{
	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >::iterator itp = _players.find(itinfo.ActorId);
	if(itp != _players.end())
	{
		itp->second.second.CurrentLife = std::min(itp->second.second.CurrentLife+itinfo.LifeDelta, itp->second.second.MaxLife);
		itp->second.second.CurrentMana = std::min(itp->second.second.CurrentMana+itinfo.ManaDelta, itp->second.second.MaxMana);

		if(_publisher)
			_publisher->UpdatedLife(itp->second.second);
	}

}



/***********************************************************
object used
***********************************************************/
void MapHandlerThread::UpdateContainerQuery(const ContainerQueryInfo &itinfo)
{
	LbaNet::ContainerInfo cinfo;
	cinfo.ContainerId = itinfo.ContainerId;
    cinfo.LockedById = -1;

	std::map<long, Actor *>::iterator it =	_actors.find((long)itinfo.ContainerId);
	if(it != _actors.end() && it->second->GetType() == 5) // if we have a container
	{
		std::map<Ice::Long, std::pair<Ice::Long, time_t> >::iterator itloc =
														_lockedContainers.find(itinfo.ContainerId);

		// if locked by a player since more than 5min then unlock - else inform the player it is already locked
		if((itloc != _lockedContainers.end()) && ((time(NULL) - itloc->second.second) < 300))
		{
			cinfo.LockedById = itloc->second.first;
		}
		else
		{
			_lockedContainers[itinfo.ContainerId] = std::make_pair<Ice::Long, time_t>(itinfo.ActorId, time(NULL));
			cinfo.LockedById = itinfo.ActorId;
			ContainerActor * cont = static_cast<ContainerActor *>(it->second);
			const std::map<long, int> & content = cont->GetCurrentContent();
			std::map<long, int>::const_iterator itmapC = content.begin();
			std::map<long, int>::const_iterator endmapC = content.end();
			for(; itmapC != endmapC; ++itmapC)
				cinfo.Content[itmapC->first] = itmapC->second;
		}

	}


	if(itinfo.clientPtr)
		itinfo.clientPtr->UpdateContainerInfo(cinfo);
}


/***********************************************************
object used
***********************************************************/
void MapHandlerThread::UpdateContainerUpdate(const ContainerUpdateInfo &itinfo)
{
	std::map<long, Actor *>::iterator it =	_actors.find((long)itinfo.ContainerId);
	if(it != _actors.end()) 
	{
		if(it->second->GetType() == 5) // if we have a container
		{
			std::map<Ice::Long, std::pair<Ice::Long, time_t> >::iterator itloc =
															_lockedContainers.find(itinfo.ContainerId);

			// if the container is really locked by the player
			if((itloc != _lockedContainers.end()) && (itloc->second.first == itinfo.ActorId))
			{
				ContainerActor * cont = static_cast<ContainerActor *>(it->second);

				// prepare the update of player inventory
				LbaNet::UpdatedItemSeq InventoryChanges;

				// get current container content
				std::map<long, int> content = cont->GetCurrentContent();

				// for all taken object
				LbaNet::ItemList::const_iterator ittak = itinfo.Taken.begin();
				LbaNet::ItemList::const_iterator endtak = itinfo.Taken.end();
				for(; ittak != endtak; ++ittak)
				{
					std::map<long, int>::iterator itlocal = content.find(ittak->first);
					if(itlocal != content.end())
					{
						if(itlocal->second >= ittak->second)
						{
							// update player inventory
							LbaNet::UpdatedItem itm;
							itm.ItemId = ittak->first;
							itm.NewCount = ittak->second;
							itm.InformPlayer = false;
							InventoryChanges.push_back(itm);

							//update container content
							cont->UpdateContent(ittak->first, -ittak->second);
						}
					}
				}


				// for all added object
				LbaNet::ItemList::const_iterator itput = itinfo.Put.begin();
				LbaNet::ItemList::const_iterator endput = itinfo.Put.end();
				for(; itput != endput; ++itput)
				{
					//update container content
					cont->UpdateContent(itput->first, itput->second);

					// update player inventory
					LbaNet::UpdatedItem itm;
					itm.ItemId = itput->first;
					itm.NewCount = -itput->second;
					itm.InformPlayer = false;
					InventoryChanges.push_back(itm);
				}



				//final update player inventory
				if(itinfo.clientPtr)
					itinfo.clientPtr->ApplyInventoryChanges(InventoryChanges);

				// remove lock
				_lockedContainers.erase(itloc);
			}
		}

		if(it->second->GetType() == 12) // if we have a NPC
		{
			NPCActor * npc = static_cast<NPCActor *>(it->second);
			if(npc->GetNPCType() == 2) // if we have a trader NPC
			{
				if(itinfo.Taken.size() == 1 && itinfo.Put.size() == 1)
				{
					//! get trader items
					const std::map<long, TraderItem> & items = npc->GetItems();
					std::map<long, TraderItem>::const_iterator itit = items.find(itinfo.Taken.begin()->first);
					if(itit != items.end()) // if trader has this item
					{
						if(itinfo.clientPtr)
						{
							LbaNet::UpdatedItemSeq InventoryChanges;

							// first update the money count
							LbaNet::UpdatedItem itm;
							itm.ItemId = itinfo.Put.begin()->first;
							itm.NewCount = -itinfo.Put.begin()->second;
							itm.InformPlayer = false;
							InventoryChanges.push_back(itm);

							//then add the object
							LbaNet::UpdatedItem itm2;
							itm2.ItemId = itinfo.Taken.begin()->first;
							itm2.NewCount = itinfo.Taken.begin()->second;
							itm2.InformPlayer = true;
							InventoryChanges.push_back(itm2);

							itinfo.clientPtr->ApplyInventoryChanges(InventoryChanges);
						}
					}
				}
			}
		}
	}
}


/***********************************************************
update targeted actor
***********************************************************/
void MapHandlerThread::UpdateTargetedActor(const TargetedActorPlayer & info, bool targeted)
{
	std::map<long, Actor *>::iterator it =	_actors.find(info.ActorId);
	if(it != _actors.end())
	{
		if(targeted)
			_targetedactor[info.PlayerId] = info.ActorId;
		else
		{
			std::map<Ice::Long, Ice::Long>::iterator ittar =_targetedactor.find(info.PlayerId);
			if(ittar != _targetedactor.end())
				_targetedactor.erase(ittar);
		}

		it->second->UpdateTargetedActor(info.PlayerId, targeted);
	}
}
