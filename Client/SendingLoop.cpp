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

#include "SendingLoop.h"
#include "InfosReceiverServant.h"
#include "ChatReceiverServant.h"
#include "ThreadSafeWorkpile.h"
#include "LogHandler.h"
#include "GameEvents.h"
#include "InventoryHandler.h"

#include <cctype>
#include <Ice/Application.h>

/***********************************************************
constructor
***********************************************************/
IceConnectionManager::IceConnectionManager(	const Ice::ObjectAdapterPtr& adapter,
											const LbaNet::ClientSessionPrx& session,
											const std::string& category)
	: _adapter(adapter), _session(session), _category(category)
{
	try
	{
		// create the actor observer - we keep for the whole game as we only need one
		Ice::Identity id;
		id.name = "ActorObserver";
		id.category = _category;
		_actors_observer = LbaNet::ActorsObserverPrx::uncheckedCast(
												_adapter->add(new InfosReceiverServant(), id)->ice_oneway());
	
		// add the whisper interface
		Ice::Identity id2;
		id2.name = "ChatObserver.Whisper";
		id2.category = _category;
		LbaNet::ChatRoomObserverPrx observer = LbaNet::ChatRoomObserverPrx::uncheckedCast(
		_adapter->add(new ChatReceiverServant("Whisper"), id2)->ice_oneway());
		_chat_observers.insert(std::make_pair("Whisper", observer));
		_session->SetWhisperInterface(observer);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception creating Actor observer: ") + ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception creating Actor observer. "));
    }

	JoinChat("World");
}


/***********************************************************
find a chat room if we already joined
***********************************************************/
LbaNet::ChatRoomParticipantPrx IceConnectionManager::FindChatRoom(const std::string& room) const
{
	std::map<std::string, LbaNet::ChatRoomParticipantPrx>::const_iterator p = _chat_publishers.find(room);
	LbaNet::ChatRoomParticipantPrx r;
	if(p != _chat_publishers.end())
	{
		r = p->second;
	}
	else
	{
		std::string upercroom = room;
		upercroom[0] = std::toupper(upercroom[0]);

		p = _chat_publishers.find(upercroom);
		if(p != _chat_publishers.end())
		{
			r = p->second;
		}
	}
	return r;
}


/***********************************************************
join a chat channel
***********************************************************/
bool IceConnectionManager::JoinChat(const std::string& roomName)
{
	bool succed = true;

	try
	{
		Ice::Identity id;
		id.name = "ChatObserver." + roomName;
		id.category = _category;
		LbaNet::ChatRoomObserverPrx observer = LbaNet::ChatRoomObserverPrx::uncheckedCast(
		_adapter->add(new ChatReceiverServant(roomName), id)->ice_oneway());

		LbaNet::ChatRoomParticipantPrx pub = _session->JoinChat(roomName, observer);

		_chat_observers.insert(make_pair(roomName, observer));
		_chat_publishers.insert(make_pair(roomName, pub));
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception entering chat room ") + ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception entering chat room ") + roomName);
		succed = false;
    }

	return succed;
}


/***********************************************************
leave a chat channel
***********************************************************/
bool IceConnectionManager::LeaveChat(const std::string& roomName)
{
	bool succed = true;

	try
	{
		std::map<std::string, LbaNet::ChatRoomObserverPrx>::iterator it = _chat_observers.find(roomName);
		if(it != _chat_observers.end())
		{
			_session->LeaveChat(roomName, it->second);
			_adapter->remove(it->second->ice_getIdentity());
			_chat_observers.erase(roomName);
			_chat_publishers.erase(roomName);
		}
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception leaving chat room ") + roomName + ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception leaving chat room ") + roomName);
		succed = false;
    }

	return succed;
}


/***********************************************************
change the room for actor info
***********************************************************/
bool IceConnectionManager::ChangeActorsRoom(const std::string& newRoom, const std::string& actorName)
{
	bool succed = true;


	// change the actor connection
	try
	{
		_actors_publisher = _session->ChangeRoom(newRoom, actorName, _actors_observer);

		LbaNet::UpdateSeq actorsstates =_session->GetUpdatedInfo();
		std::vector<ActorStateInfo> newstate;
		for(size_t i=0; i<actorsstates.size(); ++i)
		{
			ActorStateInfo asi;
			asi.ActorId = actorsstates[i].ActorId;

			// used for switch
			asi.On = actorsstates[i].On;

			//used for door
			asi.Open = actorsstates[i].Open;
			asi.Counter = actorsstates[i].Counter;
			asi.SignalOn = actorsstates[i].SignalOn;

			//used for lift
			asi.CurrentScript = actorsstates[i].CurrentScript;
			LbaNet::TargetSeq::iterator itcs = actorsstates[i].CurrentSignals.begin();
			LbaNet::TargetSeq::iterator endcs = actorsstates[i].CurrentSignals.end();
			for(; itcs != endcs; ++itcs)
				asi.CurrentSignals.push_back(*itcs);

			asi.X = actorsstates[i].X;
			asi.Y = actorsstates[i].Y;
			asi.Z = actorsstates[i].Z;
			asi.Rotation = actorsstates[i].Rotation;

			//NPC info
			asi.Visible = actorsstates[i].Visible;

			LbaNet::TargetedSeq::const_iterator ittar = actorsstates[i].Targets.begin();
			LbaNet::TargetedSeq::const_iterator endtar = actorsstates[i].Targets.end();
			for(; ittar != endtar; ++ittar)
				asi.Targets.push_back(std::make_pair<long, long>(ittar->TargetActorId, ittar->TargetPlayerId));

			newstate.push_back(asi);
		}
		ThreadSafeWorkpile::getInstance()->UpdateActorState(newstate);

		// add players already present in the map
		LbaNet::PlayerSeq players =_session->GetPlayersInfo();
		for(size_t i=0; i<players.size(); ++i)
		{
			ThreadSafeWorkpile::getInstance()->UpdateActor(players[i].ai);
			ThreadSafeWorkpile::getInstance()->UpdateActorLife(players[i].li);
		}

	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception changing room connection: ")+ ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception changing room connention: "));
		succed = false;
    }

	return succed;
}


/***********************************************************
get the publisher to the actors channel
***********************************************************/
LbaNet::ActorsParticipantPrx IceConnectionManager::GetActorPublisher()
{
	return _actors_publisher;
}



/***********************************************************
activate an actor
***********************************************************/
void IceConnectionManager::ActivateActor(const LbaNet::ActorActivationInfo & info)
{
	_session->ActivateActor(info);
}

/***********************************************************
signal an actor
***********************************************************/
void IceConnectionManager::SignalActor(const LbaNet::ActorSignalInfo & info)
{
	_session->SignalActor(info);
}

/***********************************************************
change player status
***********************************************************/
void IceConnectionManager::ChangeStatus(const std::string& status)
{
	try
	{
		_session->ChangeStatus(status);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception by ChangeStatus: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception by ChangeStatus "));
    }
}



/***********************************************************
change player color name
***********************************************************/
void IceConnectionManager::ChangeColor(const std::string& color)
{
	try
	{
		_session->ChangeNameColor(color);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception by ChangeColor: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception by ChangeColor "));
    }
}



/***********************************************************
ask for a container info
***********************************************************/
void IceConnectionManager::AskForContainerInfo(long containerid)
{
	try
	{
		_session->AskForContainerContent(containerid);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception by AskForContainerInfo: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception by AskForContainerInfo "));
    }
}



/***********************************************************
return current player life state
***********************************************************/
LbaNet::ActorLifeInfo IceConnectionManager::GetPlayerLife()
{
	try
	{
		return _session->GetLifeInfo();
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception GetPlayerLife: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception GetPlayerLife "));
    }

	return LbaNet::ActorLifeInfo ();
}


/***********************************************************
send player hurt event
***********************************************************/
void IceConnectionManager::AddPlayerHurt(long hurtingid)
{
	try
	{
		_session->GotHurtByActor(hurtingid);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AddPlayerHurt: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AddPlayerHurt "));
    }
}

/***********************************************************
send player hurt fall event
***********************************************************/
void IceConnectionManager::AddPlayerHurtFall(float fallingdistance)
{
	try
	{
		_session->GotHurtByFalling(fallingdistance);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AddPlayerHurtFall: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AddPlayerHurtFall "));
    }
}


/***********************************************************
player has been raised
***********************************************************/
void IceConnectionManager::PlayerRaised()
{
	try
	{
		_session->PlayerRaisedFromDead();
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception PlayerRaised: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception PlayerRaised "));
    }
}


/***********************************************************
player has changed world
***********************************************************/
LbaNet::SavedWorldInfo IceConnectionManager::ChangeWorld(const std::string& WorldName)
{
	try
	{
		return _session->ChangeWorld(WorldName);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception ChangeWorld: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception ChangeWorld "));
    }

	return LbaNet::SavedWorldInfo();
}

/***********************************************************
player update his current position in the world
***********************************************************/
void IceConnectionManager::UpdatePositionInWorld(const LbaNet::PlayerPosition& Position)
{
	try
	{
		_session->UpdatePositionInWorld(Position);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception UpdatePositionInWorld: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception UpdatePositionInWorld "));
    }
}


/***********************************************************
update inventory
***********************************************************/
void IceConnectionManager::UpdateInventory(const LbaNet::InventoryInfo &Inventory)
{
	try
	{
		_session->UpdateInventory(Inventory);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception UpdateInventory: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception UpdateInventory "));
    }
}


/***********************************************************
called to tell the server and obejct has been used
***********************************************************/
void IceConnectionManager::InventoryUsed(long ItemId)
{
	try
	{
		_session->UseItem(ItemId);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception UseItem: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception UseItem "));
    }
}



/***********************************************************
called by server to give container update from inventory
***********************************************************/
void IceConnectionManager::UpdateInvFromContainer(const ThreadSafeWorkpile::UpdateInvContainer & cinfo)
{
	try
	{
		_session->UpdateInventoryFromContainer(cinfo.containerid, cinfo.Taken, cinfo.Put);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception UpdateInvFromContainer: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception UpdateInvFromContainer "));
    }
}



/***********************************************************
a player wisper to another
***********************************************************/
bool IceConnectionManager::Whisper(const std::string& To, const std::string& Message)
{
	try
	{
		return _session->Whisper(To, Message);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception Whisper: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception Whisper "));
    }

	return false;
}


/***********************************************************
add friend
***********************************************************/
void IceConnectionManager::AddFriend(const std::string& Name)
{
	try
	{
		_session->AddFriend(Name);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AddFriend: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AddFriend "));
    }
}

/***********************************************************
remove friend
***********************************************************/
void IceConnectionManager::RemoveFriend(const std::string& Name)
{
	try
	{
		_session->RemoveFriend(Name);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception RemoveFriend: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception RemoveFriend "));
    }
}


/***********************************************************
send letter to the server
***********************************************************/
void IceConnectionManager::SendLetter(const ThreadSafeWorkpile::WrittenLetter &wl)
{
	try
	{
		_session->AddLetter(wl.subject, wl.message);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception SendLetter: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception SendLetter "));
    }
}


/***********************************************************
ask for letter info
***********************************************************/
void IceConnectionManager::AskLetterInfo(long letterId)
{
	try
	{
		LbaNet::LetterInfo linfo = _session->GetLetterInfo(letterId);
		if(linfo.Id >= 0)
			InventoryHandler::getInstance()->UpdateUserCreatedItemInfo(linfo.Id, linfo.Writter, 
																linfo.Date, linfo.Title, linfo.Message);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AskLetterInfo: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AskLetterInfo "));
    }
}


/***********************************************************
destroy item
***********************************************************/
void IceConnectionManager::DestroyItem(long Id)
{
	try
	{
		_session->DestroyItem(Id);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception DestroyItem: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception DestroyItem "));
    }
}

 
/***********************************************************
set player targeted by actor
***********************************************************/   
void IceConnectionManager::SetTargeted(long ActorId)
{
	try
	{
		_session->SetTargeted(ActorId);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception SetTargeted: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception SetTargeted "));
    }
}
 
/***********************************************************
set player untargeted by actor
***********************************************************/   
void IceConnectionManager::SetUnTargeted(long ActorId)
{
	try
	{
		_session->SetUnTargeted(ActorId);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception SetUnTargeted: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception SetUnTargeted "));
    }
} 


/***********************************************************
constructor
***********************************************************/
SendingLoopThread::SendingLoopThread(	const Ice::ObjectAdapterPtr& adapter, const LbaNet::ClientSessionPrx& session,
										const std::string& category,
										const std::string & MainName)
	: _main_name(MainName), _refresh_counter(0), _afk_counter(0),
			_connectionMananger(adapter, session, category), _afked(false), _previousworld(false)
{

}

/***********************************************************
running function of the thread
***********************************************************/
void SendingLoopThread::run()
{
	// for each cycle
	while(ThreadSafeWorkpile::getInstance()->WaitOneSendingCycle())
	{
		HandleChatText();


		//-----------------------------------
		// player raised
		{
			if(ThreadSafeWorkpile::getInstance()->IsRaised())
				_connectionMananger.PlayerRaised();
		}

		//-----------------------------------
		// process hurt info
		{
			std::vector<long>  vechurt;
			std::vector<float>  vechurtfall;
			ThreadSafeWorkpile::getInstance()->GetPlayerHurts(vechurt);
			ThreadSafeWorkpile::getInstance()->GetPlayerHurtFalls(vechurtfall);

			for(size_t i=0; i<vechurt.size(); ++i)
				_connectionMananger.AddPlayerHurt(vechurt[i]);

			for(size_t i=0; i<vechurtfall.size(); ++i)
				_connectionMananger.AddPlayerHurtFall(vechurtfall[i]);
		}


		//-----------------------------------
		// process color change 
		{
			std::string newcolor;
			if(ThreadSafeWorkpile::getInstance()->NameColorChanged(newcolor))
				_connectionMananger.ChangeColor(newcolor);
		}


		//-----------------------------------
		// process inventory used
		{
			std::vector<long> objs;
			ThreadSafeWorkpile::getInstance()->GetListOfObjectUsed(objs);
			for(size_t i=0; i< objs.size(); ++i)
				_connectionMananger.InventoryUsed(objs[i]);
		}


		//-----------------------------------
		// process container info
		{
			long contid;
			if(ThreadSafeWorkpile::getInstance()->HasAskedForContainer(contid))
				_connectionMananger.AskForContainerInfo(contid);
		}

		//-----------------------------------
		// process container update
		{
			ThreadSafeWorkpile::UpdateInvContainer cinfo;
			if(ThreadSafeWorkpile::getInstance()->IsUpdatedInvFromContainer(cinfo))
				_connectionMananger.UpdateInvFromContainer(cinfo);
		}

		//-----------------------------------
		// process change world
		std::string NewWorld;
		if(ThreadSafeWorkpile::getInstance()->WorldChanged(NewWorld))
		{
			// update server inventory
			if(_previousworld)
			{
				int inventorysize;
				std::map<long, std::pair<int, int> > invmap = InventoryHandler::getInstance()->GetCurrentInventory(inventorysize);
				std::map<long, std::pair<int, int> > ::iterator itm =  invmap.begin();
				std::map<long, std::pair<int, int> > ::iterator endm =  invmap.end();
				LbaNet::InventoryInfo inv;
				for(; itm != endm; ++itm)
				{
					LbaNet::InventoryItem iitm;
					iitm.Number = itm->second.first;
					iitm.PlaceInInventory = itm->second.second;
					inv.InventoryStructure[itm->first] = iitm;
				}

				std::vector<long> shortc = InventoryHandler::getInstance()->GetShortcut();
				for(size_t i=0; i< shortc.size(); ++i)
					inv.UsedShorcuts.push_back(shortc[i]);

				_connectionMananger.UpdateInventory(inv);
			}


			LbaNet::SavedWorldInfo ppos = _connectionMananger.ChangeWorld(NewWorld);

			ThreadSafeWorkpile::PlayerWorldPos position;
			position.MapName = ppos.ppos.MapName;
			position.X = ppos.ppos.X;
			position.Y = ppos.ppos.Y;
			position.Z = ppos.ppos.Z;
			position.Rotation = ppos.ppos.Rotation;
			ThreadSafeWorkpile::getInstance()->SetNewWorldPlayerPos(position);


			LbaNet::InventoryMap::iterator itm =  ppos.inventory.InventoryStructure.begin();
			LbaNet::InventoryMap::iterator endm =  ppos.inventory.InventoryStructure.end();
			std::map<long, std::pair<int, int> > inv;
			for(; itm != endm; ++itm)
				inv[itm->first] = std::make_pair<int, int>(itm->second.Number, itm->second.PlaceInInventory);
			InventoryHandler::getInstance()->SetCurrentInventory(inv, ppos.inventory.InventorySize);


			std::vector<long> shorcuts;
			for(size_t i=0; i< ppos.inventory.UsedShorcuts.size(); ++i)
				shorcuts.push_back(ppos.inventory.UsedShorcuts[i]);
			InventoryHandler::getInstance()->SetShortcut(shorcuts);

			_previousworld = true;
		}

		//-----------------------------------
		// process info
		ThreadSafeWorkpile::MapChangedInformation mci;
		if(ThreadSafeWorkpile::getInstance()->HasMapChanged(mci))
		{
			ChangeMap(mci);
		}


		//-----------------------------------
		// process added friends
		std::vector<std::string> afriends;
		ThreadSafeWorkpile::getInstance()->GetAddedFriend(afriends);
		for(size_t i=0; i<afriends.size(); ++i)
			_connectionMananger.AddFriend(afriends[i]);

		//-----------------------------------
		// process removed friends
		std::vector<std::string> rfriends;
		ThreadSafeWorkpile::getInstance()->GetRemovedFriend(rfriends);
		for(size_t i=0; i<rfriends.size() > 0; ++i)
			_connectionMananger.RemoveFriend(rfriends[i]);


		//-----------------------------------
		// process send letters
		std::vector<ThreadSafeWorkpile::WrittenLetter> rletters;
		ThreadSafeWorkpile::getInstance()->GetWrittenLetters(rletters);
		for(size_t i=0; i<rletters.size() > 0; ++i)
			_connectionMananger.SendLetter(rletters[i]);


		//-----------------------------------
		// process letter info queries
		std::vector<long> rletterqueries;
		ThreadSafeWorkpile::getInstance()->GetLetterInfoQuerys(rletterqueries);
		for(size_t i=0; i<rletterqueries.size() > 0; ++i)
			_connectionMananger.AskLetterInfo(rletterqueries[i]);


		//-----------------------------------
		// process destroyed items
		std::vector<long> destroyeditems;
		ThreadSafeWorkpile::getInstance()->GetDestroyedItems(destroyeditems);
		for(size_t i=0; i<destroyeditems.size() > 0; ++i)
			_connectionMananger.DestroyItem(destroyeditems[i]);

	
		//-----------------------------------
		// process targeted actors
		std::vector<long> targetedactors;
		ThreadSafeWorkpile::getInstance()->GetTargetedActors(targetedactors);
		for(size_t i=0; i<targetedactors.size() > 0; ++i)
			_connectionMananger.SetTargeted(targetedactors[i]);	
	
		//-----------------------------------
		// process untargeted actors
		std::vector<long> untargetedactors;
		ThreadSafeWorkpile::getInstance()->GetUntargetedActors(untargetedactors);
		for(size_t i=0; i<untargetedactors.size() > 0; ++i)
			_connectionMananger.SetUnTargeted(untargetedactors[i]);	


		if(ThreadSafeWorkpile::getInstance()->HasUpdatedInfo(_last_ai))
		{
			_last_ai.ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
			UpdateActorInfo(_last_ai);
			_refresh_counter = 0;
			_afk_counter = 0;
			if(_afked)
			{
				_afked = false;
				_connectionMananger.ChangeStatus("");
			}
		}
		else // refresh info to server every 5sec in case nothing is happening
		{
			++_refresh_counter;
			if(_refresh_counter == 200)
			{
				UpdateActorInfo(_last_ai);
				_refresh_counter = 0;
				++_afk_counter;
				if(_afk_counter == 60) // afk after 5min
				{
					_afked = true;
					_connectionMananger.ChangeStatus("away");
				}
			}
		}

		//send activation events
		{
			std::vector<LbaNet::ActorActivationInfo> vec;
			ThreadSafeWorkpile::getInstance()->GetAllActivationEvents(vec);
			for(size_t i=0; i<vec.size(); ++i)
			{
				vec[i].ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
				AddActivation(vec[i]);
			}
		}

		//send signal events
		{
			std::vector<LbaNet::ActorSignalInfo> vecsig;
			ThreadSafeWorkpile::getInstance()->GetAllSignalEvents(vecsig);
			for(size_t i=0; i<vecsig.size(); ++i)
			{
				vecsig[i].ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
				AddSignal(vecsig[i]);
			}
		}
	}



	// update server inventory before quitting
	if(_previousworld)
	{
		int inventorysize;
		std::map<long, std::pair<int, int> > invmap = InventoryHandler::getInstance()->GetCurrentInventory(inventorysize);
		std::map<long, std::pair<int, int> > ::iterator itm =  invmap.begin();
		std::map<long, std::pair<int, int> > ::iterator endm =  invmap.end();
		LbaNet::InventoryInfo inv;
		for(; itm != endm; ++itm)
		{
			LbaNet::InventoryItem iitm;
			iitm.Number = itm->second.first;
			iitm.PlaceInInventory = itm->second.second;
			inv.InventoryStructure[itm->first] = iitm;
		}

		std::vector<long> shortc = InventoryHandler::getInstance()->GetShortcut();
		for(size_t i=0; i< shortc.size(); ++i)
			inv.UsedShorcuts.push_back(shortc[i]);

		_connectionMananger.UpdateInventory(inv);
	}
}


/***********************************************************
ChangeMap
***********************************************************/
bool SendingLoopThread::ChangeMap(const ThreadSafeWorkpile::MapChangedInformation & NewMap)
{
	// change the actor connection
	bool succeed = _connectionMananger.ChangeActorsRoom(NewMap.NewWorldName + "_" + 
												NewMap.NewMapName+"_Actors", _main_name);


	// disconnect the old map chat channel
	_connectionMananger.LeaveChat(_current_world + "_" + _current_map);

	// connect to the new map channel
	_connectionMananger.JoinChat(NewMap.NewWorldName + "_" + NewMap.NewMapName);


	// update the current map name
	if(succeed)
	{
		_current_world = NewMap.NewWorldName;
		_current_map = NewMap.NewMapName;

		LbaNet::ActorLifeInfo ai = _connectionMananger.GetPlayerLife();
		ThreadSafeWorkpile::getInstance()->AddEvent(new PlayerLifeChangedEvent(ai.CurrentLife, 
													ai.MaxLife, ai.CurrentMana, ai.MaxMana));


		// player update server with his current position in the world
		LbaNet::PlayerPosition ppos;
		ppos.MapName = NewMap.NewMapName;
		ppos.X = NewMap.X;
		ppos.Y = NewMap.Y;
		ppos.Z = NewMap.Z;
		ppos.Rotation = NewMap.Rotation;
		_connectionMananger.UpdatePositionInWorld(ppos);
	}
	else
	{
		_current_world = "";
		_current_map = "";
	}

	return succeed;
}

/***********************************************************
UpdateInfo
***********************************************************/
void SendingLoopThread::UpdateActorInfo(const LbaNet::ActorInfo & MainInfo)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.GetActorPublisher()->Update(MainInfo);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
		ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(-1));
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
		ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(-1));
	}
}


/***********************************************************
when the engine update information
***********************************************************/
void SendingLoopThread::AddActivation(const LbaNet::ActorActivationInfo & info)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.ActivateActor(info);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
	}
}


/***********************************************************
when the engine update information
***********************************************************/
void SendingLoopThread::AddSignal(const LbaNet::ActorSignalInfo & info)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.SignalActor(info);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
	}
}




/***********************************************************
SendText
***********************************************************/
void SendingLoopThread::HandleChatText()
{
	std::vector<std::string> texts;
	ThreadSafeWorkpile::getInstance()->GetChatText(texts);

	try
	{
		if(texts.size() > 0)
		{
			_refresh_counter = 0;
			_afk_counter = 0;
			if(_afked)
			{
				_afked = false;
				_connectionMananger.ChangeStatus("");
			}
		}

		std::vector<std::string>::const_iterator it = texts.begin();
		std::vector<std::string>::const_iterator end = texts.end();
		for(;it != end; ++it)
		{
			ProcessText(*it);
		}
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from chat : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception sending text to chat"));
	}
}


/***********************************************************
process a line of text
***********************************************************/
void SendingLoopThread::ProcessText(const std::string & Text)
{
	// return if text is empty
	if(Text == "")
		return;

	// if it is a command - preproccess it
	if(Text[0] == '/')
	{
		std::vector<std::string> tok;
		std::stringstream ss(Text);
		std::string buf;
		while(ss >> buf)
		{
			tok.push_back(buf);
		}

		if(tok.size() > 0)
		{
			if(tok[0] == "/join")
			{
				if(tok.size() != 2)
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "You need to specify a room to join.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				std::string roomName = tok[1];
				if(_connectionMananger.FindChatRoom(roomName))
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "You already joined channel "+roomName;
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				_connectionMananger.JoinChat(roomName);
				//_cb->AddText("Joined channel "+roomName);
				return;
			}

			if(tok[0] == "/leave")
			{
				if(tok.size() != 2)
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "You need to specify a room to leave.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				std::string roomName = tok[1];
				_connectionMananger.LeaveChat(roomName);
				//_cb->AddText("Left channel "+roomName);
				return;
			}

			if(tok[0] == "/w")
			{
				if(tok.size() < 3)
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "Incorrect command.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				std::string playername = tok[1];
				std::string message;
				for(size_t i=2; i<tok.size(); ++i)
					message += tok[i] + " ";

				if(!_connectionMananger.Whisper(playername, message))
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "The player " + playername + " is not available.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
				}
				else
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "to " + playername;
					cdata.Text = message;
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);

					ThreadSafeWorkpile::getInstance()->AddWhisperChannel(playername);
				}
				return;
			}

			if(tok[0] == "/afk")
			{
				_connectionMananger.ChangeStatus("away");
				return;
			}

			if(tok[0] == "/back")
			{
				_connectionMananger.ChangeStatus("");
				return;
			}

			LbaNet::ChatRoomParticipantPrx room = _connectionMananger.FindChatRoom(tok[0].substr(1));
			if(room)
			{
				room->Say(_main_name, Text.substr(Text.find_first_of(' ')+1));
				return;
			}
			else
			{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "All";
					cdata.Sender = "info";
					cdata.Text = "Invalid command: " + tok[0];
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
				return;
			}
		}
	}
	else
	{
		LbaNet::ChatRoomParticipantPrx room = _connectionMananger.FindChatRoom(_current_map);
		if(room)
			room->Say(_main_name, Text);
	}
}

