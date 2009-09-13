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
#include "MapInfoXmlReader.h"
#include "ServerSignaler.h"
#include "MapManagerServant.h"

/***********************************************************
	Constructor
***********************************************************/
MapHandlerThread::MapHandlerThread(SharedData	* SD, ActorsObserverPrx	publisher,
										const std::string &mapName, MapManagerServant *	stopper)
: _SD(SD), _publisher(publisher), _stopping(false), _mapName(mapName), _stopper(stopper)
{
	_signaler = new ServerSignaler(this);


	std::string worldname = mapName.substr(0, mapName.find("_"));
	std::string mapS = mapName.substr(mapName.find("_")+1);
	mapS = mapS.substr(0, mapS.find_last_of("_"));
	WorldInfo wi;
	MapInfoXmlReader::LoadWorld("Data/" + worldname + ".xml", wi);
	std::string localfile = wi.Maps[mapS].Files["LocalActors"];
	std::string externalfile = wi.Maps[mapS].Files["ExternalActors"];

	std::map<long, SpriteInfo> tmp;
	std::map<long, ModelInfo> tmp2;
	MapInfoXmlReader::LoadActors("Data/" + localfile, tmp, tmp, tmp2, _actors, _signaler);

	std::map<long, Actor *> map;
	MapInfoXmlReader::LoadActors("Data/" + externalfile, tmp, tmp, tmp2, map, _signaler);
	std::map<long, Actor *>::iterator it = map.begin();
	std::map<long, Actor *>::iterator end = map.end();
	for(;it != end; ++it)
		_actors[it->first] = it->second;
}


/***********************************************************
	destructor
***********************************************************/
MapHandlerThread::~MapHandlerThread()
{
	std::map<long, Actor *>::iterator it = _actors.begin();
	std::map<long, Actor *>::iterator end = _actors.end();
	for(;it != end; ++it)
		delete it->second;

	delete _signaler;
}




/***********************************************************
	callback function called when a message is received from IceStorm
***********************************************************/
void MapHandlerThread::UpdatedInfo(const LbaNet::ActorInfo& asi)
{
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator it = _players.find(asi.ActorId);
	if(it != _players.end())
		it->second = asi;
}



/***********************************************************
	a player join a map
***********************************************************/
void MapHandlerThread::Join(Ice::Long PlayerId)
{
	_players[PlayerId] = LbaNet::ActorInfo();
	_stopping = false;
}

/***********************************************************
 a player leave a map
***********************************************************/
bool MapHandlerThread::Leave(Ice::Long PlayerId)
{
	std::map<Ice::Long, Ice::Long>::iterator it =_todeactivate.find(PlayerId);
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator itp = _players.find(PlayerId);

	if(it != _todeactivate.end())
	{
		if(itp != _players.end())
		{
			_actors[(long)it->second]->ProcessDesactivation(itp->second.X, itp->second.Y, itp->second.Z, itp->second.Rotation);

			if(_publisher)
			{
				LbaNet::ActorActivationInfo ai;
				ai.Activate = false;
				ai.ActivatedId = it->second;
				ai.ActorId = it->first;
				ai.X = itp->second.X;
				ai.Y = itp->second.Y;
				ai.Z = itp->second.Z;
				ai.Rotation = itp->second.Rotation;
				_publisher->ActivatedActor(ai);
			}
		}

		_todeactivate.erase(it);
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
void MapHandlerThread::ActivateActor(const LbaNet::ActorActivationInfo& ai)
{
	if(_publisher)
		_publisher->ActivatedActor(ai);

	std::map<long, Actor *>::iterator it =	_actors.find((long)ai.ActivatedId);
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator itp = _players.find(ai.ActorId);
	if(it != _actors.end() && itp != _players.end())
	{
		if(ai.Activate)
		{
			it->second->ProcessActivation(ai.X, ai.Y, ai.Z, ai.Rotation);

			if(it->second->NeedDesactivation())
				_todeactivate[ai.ActorId] = ai.ActivatedId;
		}
		else
		{
			it->second->ProcessDesactivation(ai.X, ai.Y, ai.Z, ai.Rotation);
			std::map<Ice::Long, Ice::Long>::iterator itpd =_todeactivate.find(ai.ActorId);
			if(itpd != _todeactivate.end())
				_todeactivate.erase(itpd);
		}
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
		std::vector<std::pair<Ice::Long, bool> >  joinedmap;
		std::vector<LbaNet::ActorInfo>  pinfos;
		std::vector<LbaNet::ActorActivationInfo>  ainfos;
		std::vector<LbaNet::ActorSignalInfo>  sinfos;
		_SD->GetJoined(joinedmap);
		_SD->GetUpdatedinfo(pinfos);
		_SD->GetActorinfo(ainfos);
		_SD->GetSignalinfo(sinfos);		


		Lock sync(*this);
		// update state
		{
			// player join/leave
			{
				std::vector<std::pair<Ice::Long, bool> >::const_iterator it = joinedmap.begin();
				std::vector<std::pair<Ice::Long, bool> >::const_iterator end = joinedmap.end();
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
				std::vector<LbaNet::ActorActivationInfo>::const_iterator it = ainfos.begin();
				std::vector<LbaNet::ActorActivationInfo>::const_iterator end = ainfos.end();
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
		}


		// process actors
		double tnow = IceUtil::Time::now().toMilliSecondsDouble();
		float tdiff = (float)(tnow - _lasttime);
		_lasttime = tnow;

		std::map<long, Actor *>::iterator it =_actors.begin();
		std::map<long, Actor *>::iterator end =_actors.end();
		for(; it != end; ++it)
			it->second->Process(tnow, tdiff);

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

	std::map<Ice::Long, LbaNet::ActorInfo>::iterator it =_players.begin();
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator end =_players.end();
	for(; it != end; ++it)
		res.push_back(it->second);

	return res;
}