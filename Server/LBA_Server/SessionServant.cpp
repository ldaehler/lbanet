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


#include "SessionServant.h"
#include <IceUtil/UUID.h>
#include <Ice/Ice.h>
#include "MapInfoXmlReader.h"


#define _FIRST_USER_CREATED_ID_ 10000000
#define _STARTING_LIFE_ 50
#define _STARTING_MANA_ 50


/***********************************************************
constructor
***********************************************************/
SessionServant::SessionServant(const std::string& userId, const RoomManagerPrx& manager,
									const ConnectedTrackerPrx& ctracker, const MapManagerPrx& map_manager,
									std::string	version, DatabaseHandler & dbh)
: _manager(manager), _curr_actor_room(""), _userId(userId), _ctracker(ctracker), _map_manager(map_manager),
	_userNum(-1), _version(version), _currColor("FFFFFFFF"), _dbh(dbh), _selfptr(NULL), _client_observer(NULL),
	_QH(const_cast<SessionServant *>(this)), _currWorldName(""), _needquestupdate(false), _magicballused(false),
	_playerdead(false)
{
	_userNum = _ctracker->Connect(_userId);

	_lifeinfo.ActorId = _userNum;
	_lifeinfo.Name = _userId;

	_session_world_inventory_files["Lba1Original"] = "Data/Inventory/lba1_inventory.xml";
	_session_world_inventory_files["Lba1Expanded"] = "Data/Inventory/lba1E_inventory.xml";
	_session_world_inventory_files["GiantCitadel"] = "Data/Inventory/lba1_inventory.xml";
	_session_world_inventory_files["GiantPrincipal"] = "Data/Inventory/lba1_inventory.xml";


	_session_world_quest_files["Lba1Original"] = "Data/Quest/lba1.xml";
	_session_world_quest_files["Lba1Expanded"] = "Data/Quest/lba1E.xml";
	_session_world_quest_files["GiantCitadel"] = "Data/Quest/lba1.xml";
	_session_world_quest_files["GiantPrincipal"] = "Data/Quest/lba1.xml";
}


/***********************************************************
the client join a chat room
***********************************************************/
ChatRoomParticipantPrx SessionServant::JoinChat(	const std::string& room,
														const ChatRoomObserverPrx& observer,
														const Ice::Current& current)
{
    Lock sync(*this);

	std::map<std::string, ChatRoomParticipantPrx>::iterator it = _chat_rooms.find(room);
	if(it != _chat_rooms.end())
		return it->second;


	Ice::Identity id;
    id.category = "_" + _userId;
    id.name = IceUtil::generateUUID();

	ChatParticipantServant * cptr = new ChatParticipantServant(room, observer, _manager);
    ChatRoomParticipantPrx proxy = ChatRoomParticipantPrx::uncheckedCast(current.adapter->add(cptr, id));

	if(room == "World")
	{
		proxy->Say("info", "#joined " +_userId);
	}

	_chat_rooms[room] = proxy;
    return proxy;
}


/***********************************************************
the client leave a chat room
***********************************************************/
void SessionServant::LeaveChat(	const std::string& room, const ChatRoomObserverPrx& observer,
										const Ice::Current& current)
{
    Lock sync(*this);

	std::map<std::string, ChatRoomParticipantPrx>::iterator it = _chat_rooms.find(room);
	if(it != _chat_rooms.end())
	{
		if(room == "World")
		{
			it->second->Say("info", "#left " + _userId);
			_ctracker->Disconnect(_userNum);
		}

		current.adapter->remove(it->second->ice_getIdentity());
		_chat_rooms.erase(it);
	}
}




/***********************************************************
the client change actor information room
***********************************************************/
ActorsParticipantPrx SessionServant::ChangeRoom(		const std::string& newroom,
														const std::string & actorname,
														const ActorsObserverPrx& observer,
														const Ice::Current& current)
{
    Lock sync(*this);
	_client_observer = observer;

	if(_needquestupdate)
	{
		_needquestupdate = false;
		InitializeClientQuests(_QH.GetQuestsStarted(), _QH.GetQuestsFinished());
	}


	cleanEphemereItems();

	// if we are already in the new rooom do nothing
	if(_curr_actor_room == newroom)
		return _actors_room;


	// delete old room pointer
	if(_curr_actor_room != "")
	{
		_map_manager->LeaveMap(_curr_actor_room, _userNum);
		current.adapter->remove(_actors_room->ice_getIdentity());

		_actors_manager = NULL;
	}


	// create new room handler
	Ice::Identity id;
    id.category = "_" + _userId;
    id.name = IceUtil::generateUUID();

	ActorsParticipantServant *actors_room_ptr = new ActorsParticipantServant(newroom, _userNum, actorname, observer, _manager);
    _actors_room = ActorsParticipantPrx::uncheckedCast(current.adapter->add(actors_room_ptr, id));
	_curr_actor_room = newroom;
	_actors_manager = _map_manager->JoinMap(_curr_actor_room, _userNum, _lifeinfo, _selfptr);

    return _actors_room;
}




/***********************************************************
quit current world
***********************************************************/
void SessionServant::QuitCurrentWorld()
{
	if(_currWorldName != "")
	{
		cleanEphemereItems();
		_dbh.UpdateInventory(_playerInventory, _currWorldName, _userNum);
		_dbh.SetQuestInfo(_currWorldName, _userNum, _QH.GetQuestsStarted(), _QH.GetQuestsFinished());
		_dbh.QuitWorld(_currWorldName, _userNum, _lifeinfo.CurrentLife, _lifeinfo.CurrentMana, 
						_lifeinfo.MaxLife, _lifeinfo.MaxMana);
	}
}



/***********************************************************
destroy the session
***********************************************************/
void SessionServant::destroy(const Ice::Current& current)
{
    Lock sync(*this);
	try
	{
		QuitCurrentWorld();
		_ctracker->Disconnect(_userNum);
	}
	catch(...)
	{
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
	}

	std::map<std::string, ChatRoomParticipantPrx>::iterator worldit = _chat_rooms.find("World");
	if(worldit != _chat_rooms.end())
	{
		try
		{
			worldit->second->Say("info", "#left " + _userId);
			_map_manager->LeaveMap(_curr_actor_room, _userNum);
		}
		catch(...)
		{
			// Ignore. The ice mediated invocation can throw an
			// exception on shutdown.
		}
	}

	try
	{
		current.adapter->remove(current.id);
	}
	catch(...)
	{
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
	}

	for(std::map<std::string, ChatRoomParticipantPrx>::const_iterator p = _chat_rooms.begin(); p != _chat_rooms.end(); ++p)
	{
		try
		{
			current.adapter->remove(p->second->ice_getIdentity());
		}
		catch(...)
		{
			// Ignore. The ice mediated invocation can throw an
			// exception on shutdown.
		}
	}

	if(_curr_actor_room != "")
	{
		try
		{
			current.adapter->remove(_actors_room->ice_getIdentity());
		}
		catch(...)
		{
			// Ignore. The ice mediated invocation can throw an
			// exception on shutdown.
		}
	}

	try
	{
		_chat_rooms.clear();
	}
	catch(...)
	{
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
	}
}


/***********************************************************
get the list of people connected
***********************************************************/
LbaNet::ConnectedL SessionServant::GetConnected(Ice::Long & ownid, const Ice::Current&)
{
    Lock sync(*this);
	ownid = _userNum;
	return _ctracker->GetConnected();
}


/***********************************************************
get server time
***********************************************************/
Ice::Long SessionServant::GetTime(const Ice::Current&)
{
	return IceUtil::Time::now().toMilliSeconds();
}



/***********************************************************
callback function called when an actor id activated
***********************************************************/
void SessionServant::ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&)
{
	try
	{
		if(_actors_manager)
			_actors_manager->ActivateActor(ai);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during ActivateActor: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during ActivateActor"<<std::endl;
    }
}

/***********************************************************
callback function called when an actor id signaled
***********************************************************/
void SessionServant::SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&)
{
	try
	{
		if(_actors_manager)
			_actors_manager->SignalActor(ai);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during SignalActor: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during SignalActor"<<std::endl;
    }
}


/***********************************************************
get updated info
***********************************************************/
LbaNet::UpdateSeq SessionServant::GetUpdatedInfo(const Ice::Current&)
{
	try
	{
		if(_actors_manager)
			return _actors_manager->GetUpdatedInfo();
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during GetUpdatedInfo: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during GetUpdatedInfo"<<std::endl;
    }

	return LbaNet::UpdateSeq();
}



/***********************************************************
	get player info
***********************************************************/
LbaNet::PlayerSeq SessionServant::GetPlayersInfo(const Ice::Current&)
{
	try
	{
		if(_actors_manager)
			return _actors_manager->GetPlayersInfo();
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during GetUpdatedInfo: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during GetUpdatedInfo"<<std::endl;
    }

	return LbaNet::PlayerSeq();
}


/***********************************************************
change player status
***********************************************************/
void SessionServant::ChangeStatus(const std::string& Status, const Ice::Current&)
{
    Lock sync(*this);
	try
	{
		_currStatus = Status;
		_ctracker->ChangeStatus(_userId, Status);

		std::map<std::string, ChatRoomParticipantPrx>::iterator worldit = _chat_rooms.find("World");
		if(worldit != _chat_rooms.end())
		{
			worldit->second->Say("info", "#status " + _userId + " " + Status + " " + _currColor);
		}
	}
	catch(...)
	{
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
	}
}


/***********************************************************
change name display color
***********************************************************/
void SessionServant::ChangeNameColor(const std::string& Color, const Ice::Current&)
{
    Lock sync(*this);
	try
	{
		_currColor = Color;
		_ctracker->ChangeNameColor(_userId, Color);

		std::map<std::string, ChatRoomParticipantPrx>::iterator worldit = _chat_rooms.find("World");
		if(worldit != _chat_rooms.end())
		{
			worldit->second->Say("info", "#status " + _userId + " " + _currStatus + " " + Color);
		}
	}
	catch(...)
	{
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
	}
}


/***********************************************************
get server version
***********************************************************/
std::string SessionServant::GetVersion(const Ice::Current&)
{
    Lock sync(*this);
	return _version;
}


/***********************************************************
return the current life state
***********************************************************/
LbaNet::ActorLifeInfo SessionServant::GetLifeInfo(const Ice::Current&)
{
	Lock sync(*this);
	return _lifeinfo;
}

/***********************************************************
called when actor have been hurt
***********************************************************/
void SessionServant::GotHurtByActor(Ice::Long hurtingactorid, const Ice::Current&)
{
	Lock sync(*this);
	try
	{
		if(_actors_manager)
			return _actors_manager->GotHurtByActor(_userNum, hurtingactorid);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during GotHurtByActor: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during GotHurtByActor"<<std::endl;
    }
}

/***********************************************************
called when actor have been hurt
***********************************************************/
void SessionServant::GotHurtByFalling(Ice::Float fallingdistance, const Ice::Current&)
{
	Lock sync(*this);
	try
	{
		if(_actors_manager)
			return _actors_manager->GotHurtByFalling(_userNum, fallingdistance);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during GotHurtByFalling: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during GotHurtByFalling"<<std::endl;
    }
}


/***********************************************************
player is dead and reborn
***********************************************************/
void SessionServant::PlayerRaisedFromDead(const Ice::Current&)
{
	Lock sync(*this);
	try
	{
		if(!_playerdead)
			return;

		_playerdead = false;

		if(_actors_manager)
			return _actors_manager->RaisedFromDead(_userNum);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during GotHurtByFalling: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during GotHurtByFalling"<<std::endl;
    }
}



/***********************************************************
called when player is drowning
***********************************************************/	       
void SessionServant::PlayerDrowning(const Ice::Current&)
{
	Lock sync(*this);
	_playerdead = true;

	// record kill in database
	_dbh.RecordKill(_currWorldName, _userNum, 6, 0);
}


/***********************************************************
player has changed world
***********************************************************/
LbaNet::SavedWorldInfo SessionServant::ChangeWorld(const std::string& WorldName,
														   const Ice::Current&)
{
    Lock sync(*this);

	// save old world info
	QuitCurrentWorld();


	// retrieve info from db
	_currWorldName = WorldName;
	LbaNet::SavedWorldInfo swinfo = _dbh.ChangeWorld(WorldName, _userNum);

	if(swinfo.CurrentLife < 0)
		swinfo.CurrentLife = _STARTING_LIFE_;
	if(swinfo.MaxLife < 0)
		swinfo.MaxLife = _STARTING_LIFE_;
	if(swinfo.CurrentMana < 0)
		swinfo.CurrentMana = _STARTING_MANA_;
	if(swinfo.MaxMana < 0)
		swinfo.MaxMana = _STARTING_MANA_;


	// reload inventory
	_inventory_db.clear();
	MapInfoXmlReader::LoadInventory(_session_world_inventory_files[WorldName], _inventory_db);


	// reload quest info
	std::map<long, QuestPtr> quests;
	MapInfoXmlReader::LoadQuests(_session_world_quest_files[WorldName], quests, 
									const_cast<SessionServant *>(this), &_QH);
	_QH.Initialize(quests);

	std::vector<long> questStarted, questFinished;
	_dbh.GetQuestInfo(WorldName, _userNum, questStarted, questFinished);

	_QH.SetStartedFinished(questStarted, questFinished);
	_needquestupdate = true;


	_playerInventory = swinfo.inventory;

	_lifeinfo.CurrentLife = swinfo.CurrentLife;
	_lifeinfo.CurrentMana = swinfo.CurrentMana;
	_lifeinfo.MaxLife = swinfo.MaxLife;
	_lifeinfo.MaxMana = swinfo.MaxMana;


	return swinfo;
}

/***********************************************************
player update his current position in the world
***********************************************************/
void SessionServant::UpdatePositionInWorld(const LbaNet::PlayerPosition& Position,
												   const Ice::Current&)
{
    Lock sync(*this);
	_dbh.UpdatePositionInWorld(Position, _currWorldName, _userNum);
}



/***********************************************************
player update his current inventory gui info
***********************************************************/
void SessionServant::UpdateInventory(const InventoryInfo &Inventory, const Ice::Current&)
{
    Lock sync(*this);
    _playerInventory.UsedShorcuts = Inventory.UsedShorcuts;

	LbaNet::InventoryMap::const_iterator itm =  Inventory.InventoryStructure.begin();
	LbaNet::InventoryMap::const_iterator endm =  Inventory.InventoryStructure.end();
	for(;itm !=  endm; ++itm)
	{
		LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(itm->first);
		if(itlocal != _playerInventory.InventoryStructure.end())
			itlocal->second.PlaceInInventory = itm->second.PlaceInInventory;
	}
}


/***********************************************************
player use an item from inventory
***********************************************************/
void SessionServant::UseItem(Ice::Long ItemId, const Ice::Current& cur)
{
    Lock sync(*this);
	LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(ItemId);
	if(itlocal != _playerInventory.InventoryStructure.end())
	{
		ItemInfo & itmtmp = _inventory_db[ItemId];

		// only use consumable items type
		if(itmtmp.type != 1)
			return;

		bool used = false;
		bool updatedlifemana = false;
		int deltalife = 0;
		int deltamana = 0;

		if(itmtmp.valueA == 1) // if life potion
		{
			used = true;
			updatedlifemana = true;
			deltalife = (int)(itmtmp.Effect * _lifeinfo.MaxLife / 100.0);
		}

		if(itmtmp.valueA == 2) // if mana potion
		{
			used = true;
			updatedlifemana = true;
			deltamana = (int)(itmtmp.Effect * _lifeinfo.MaxMana / 100.0);
		}

		if(itmtmp.valueA == 3) // if clover
		{
			used = true;
			updatedlifemana = true;
			deltalife = (int)(itmtmp.Effect * _lifeinfo.MaxLife / 100.0);
			deltamana = (int)(itmtmp.Effect * _lifeinfo.MaxMana / 100.0);
		}

		if(used)
		{
			LbaNet::UpdatedItemSeq InventoryChanges;
			LbaNet::UpdatedItem itm;
			itm.ItemId = ItemId;
			itm.NewCount = -1;
			itm.InformPlayer = true;
			InventoryChanges.push_back(itm);
			ApplyInternalInventoryChanges(InventoryChanges);
		}

		try
		{
			if(updatedlifemana && _actors_manager)
				_actors_manager->UpdateLifeMana(_userNum, deltalife, deltamana);
		}
		catch(const IceUtil::Exception& ex)
		{
			std::cout<<"SessionServant - Exception during UseItem: "<< ex.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"SessionServant - Unknown exception during UseItem"<<std::endl;
		}

	}

}


/***********************************************************
check if we have item in inventory
***********************************************************/
bool SessionServant::HasItem(Ice::Long ItemId, int QUantity, const Ice::Current&)
{
    Lock sync(*this);
	LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(ItemId);
	if(itlocal != _playerInventory.InventoryStructure.end())
	{
		if(itlocal->second.Number >= QUantity)
			return true;
	}

	return false;
}


/***********************************************************
callback functions to apply inventory changes
***********************************************************/
void SessionServant::ApplyInternalInventoryChanges(const UpdatedItemSeq &InventoryChanges)
{
	UpdatedItemSeq clientseq;
	UpdatedItemSeq clientInventoryChanges;

	UpdatedItemSeq::const_iterator it = InventoryChanges.begin();
	UpdatedItemSeq::const_iterator end = InventoryChanges.end();
	for(; it != end; ++it)
	{
		LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(it->ItemId);
		if(itlocal != _playerInventory.InventoryStructure.end())
		{
			itlocal->second.Number += it->NewCount;

			LbaNet::UpdatedItem itm;
			itm.ItemId = it->ItemId;
			itm.NewCount = itlocal->second.Number;
			clientseq.push_back(itm);

			if(itlocal->second.Number <= 0)
				_playerInventory.InventoryStructure.erase(itlocal);
		}
		else
		{
			if(it->NewCount > 0)
			{
				LbaNet::InventoryItem itinv;
				itinv.Number = it->NewCount;
				itinv.PlaceInInventory = -1;
				_playerInventory.InventoryStructure[it->ItemId] = itinv;

				LbaNet::UpdatedItem itm;
				itm.ItemId = it->ItemId;
				itm.NewCount = it->NewCount;
				clientseq.push_back(itm);
			}
		}

		if(it->InformPlayer)
			clientInventoryChanges.push_back(*it);
	}

	try
	{
		if(clientseq.size() > 0 && _client_observer)
		{
			_client_observer->ApplyInventoryChanges(clientseq);

			if(clientInventoryChanges.size() > 0)
				_client_observer->InformInventoryChanges(clientInventoryChanges);
		}
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during ApplyInventoryChanges: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during ApplyInventoryChanges"<<std::endl;
    }
}



/***********************************************************
cleanEphemereItems
***********************************************************/
void SessionServant::cleanEphemereItems()
{
	UpdatedItemSeq clientseq;

	LbaNet::InventoryMap::iterator it = _playerInventory.InventoryStructure.begin();
	while(it != _playerInventory.InventoryStructure.end())
	{
		// not for player created items
		if(it->first < _FIRST_USER_CREATED_ID_ && _inventory_db[it->first].Ephemere)
		{
			LbaNet::UpdatedItem itm;
			itm.ItemId = it->first;
			itm.NewCount = 0;
			clientseq.push_back(itm);

			_playerInventory.InventoryStructure.erase(it);
			// erase does not return iterator for a map in linux
			it = _playerInventory.InventoryStructure.begin();
		}
		else
			++it;
	}


	try
	{
		if(clientseq.size() > 0 && _client_observer)
			_client_observer->ApplyInventoryChanges(clientseq);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during cleanEphemereItems: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during cleanEphemereItems"<<std::endl;
    }
}



/***********************************************************
callback functions to apply inventory changes
***********************************************************/
void SessionServant::ApplyInventoryChanges(const UpdatedItemSeq &InventoryChanges, const Ice::Current&)
{
    Lock sync(*this);
	ApplyInternalInventoryChanges(InventoryChanges);
}


/***********************************************************
get container content
***********************************************************/
void SessionServant::AskForContainerContent(Ice::Long ContainerId, const Ice::Current&)
{
    Lock sync(*this);

	try
	{
		if(_actors_manager)
			_actors_manager->AskForContainer(_userNum, ContainerId);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"SessionServant - Exception during AskForContainerContent: "<< ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"SessionServant - Unknown exception during AskForContainerContent"<<std::endl;
	}
}



/***********************************************************
callback functions to update container info
***********************************************************/
void SessionServant::UpdateContainerInfo(const ContainerInfo &container, const Ice::Current&)
{
    Lock sync(*this);

	try
	{
		if(_client_observer)
			_client_observer->UpdateContainerInfo(container);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during UpdateContainerInfo: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during UpdateContainerInfo"<<std::endl;
    }
}


/***********************************************************
update player inventory from container content
***********************************************************/
void SessionServant::UpdateInventoryFromContainer(Ice::Long ContainerId, const ItemList &Taken,
												  const ItemList &Put,
												  const Ice::Current&)
{
    Lock sync(*this);

	try
	{
		// check if we really have the item we try to add to the container
		ItemList CheckedPut;
		std::map< Ice::Long, Ice::Int>::const_iterator it = Put.begin();
		std::map< Ice::Long, Ice::Int>::const_iterator end = Put.end();

		for(; it != end; ++it)
		{
			LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(it->first);
			if(itlocal != _playerInventory.InventoryStructure.end())
			{
				if((it->first >= _FIRST_USER_CREATED_ID_) || _inventory_db[it->first].type == 1 ||
						_inventory_db[it->first].type == 6)
					if(itlocal->second.Number >= it->second)
						CheckedPut[it->first] = it->second;
			}
		}

		//check if we did not reached the size limit
		ItemList CheckTaken;
		std::map< Ice::Long, Ice::Int>::const_iterator ittak = Taken.begin();
		std::map< Ice::Long, Ice::Int>::const_iterator endtak = Taken.end();

		for(; ittak != endtak; ++ittak)
		{
			LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(ittak->first);
			if(itlocal != _playerInventory.InventoryStructure.end())
			{
				// check if item we add does not cross the max number for this item type
				int currnumb = itlocal->second.Number;
				int maxitem = _inventory_db[ittak->first].Max;
				int totake = std::min(maxitem-currnumb, ittak->second);

				if(totake > 0)
					CheckTaken[ittak->first] = totake;
			}
			else
			{
				// check if we still have place in the inventory
				if((int)_playerInventory.InventoryStructure.size() < _playerInventory.InventorySize)
				{
					int totake = 0;

					if( ittak->first >= _FIRST_USER_CREATED_ID_)
						totake = 1;
					else
					{
						int maxitem = _inventory_db[ittak->first].Max;
						totake = std::min(maxitem, ittak->second);
					}

					if(totake > 0)
						CheckTaken[ittak->first] = totake;
				}
			}
		}


		// send to map handler to make change to container
		if(_actors_manager)
			_actors_manager->UpdateContainer(ContainerId, _userNum, CheckTaken, CheckedPut);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"SessionServant - Exception during UpdateInventoryFromContainer: "<< ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"SessionServant - Unknown exception during UpdateInventoryFromContainer"<<std::endl;
	}
}




/***********************************************************
set player wisper interface
***********************************************************/
void SessionServant::SetWhisperInterface(const LbaNet::ChatRoomObserverPrx& winterface, const Ice::Current&)
{
	try
	{
		if(_ctracker)
			_ctracker->SetWhisperInterface(_userId, winterface);
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during SetWhisperInterface: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during SetWhisperInterface"<<std::endl;
    }
}

/***********************************************************
a player wisper to another
***********************************************************/
bool SessionServant::Whisper(const std::string& To, const std::string& Message, const ::Ice::Current&)
{
	try
	{
		if(_ctracker)
			return _ctracker->Whisper(_userId, To, Message);
		else
			return false;
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during Whisper: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during Whisper"<<std::endl;
    }

	return false;
}



/***********************************************************
add friend function
***********************************************************/
void SessionServant::AskFriend(const std::string &friendname, const ::Ice::Current&)
{
	_dbh.AskFriend(_userNum, friendname);
}

/***********************************************************
add friend function
***********************************************************/
void SessionServant::AcceptFriend(Ice::Long friendid, const ::Ice::Current&)
{
	_dbh.AcceptFriend(_userNum, friendid);
}

/***********************************************************
remove friend function
***********************************************************/
void SessionServant::RemoveFriend(Ice::Long friendid, const ::Ice::Current&)
{
	_dbh.RemoveFriend(_userNum, friendid);
}

/***********************************************************
get friends function
***********************************************************/
LbaNet::FriendsSeq SessionServant::GetFriends(const ::Ice::Current&)
{
	return _dbh.GetFriends(_userNum);
}



/***********************************************************
store letter to the server and return the letter id
***********************************************************/
void SessionServant::AddLetter(const std::string& title, const std::string& message, const ::Ice::Current&)
{
	long letterid = _dbh.AddLetter(_userNum, title, message);
	if(letterid >= 0)
	{
		UpdatedItemSeq InventoryChanges;
		LbaNet::UpdatedItem itm;
		itm.ItemId = letterid + _FIRST_USER_CREATED_ID_; // user created inventory obejct start at 10 000 000
		itm.NewCount = 1;
		itm.InformPlayer = true;
		InventoryChanges.push_back(itm);

		Lock sync(*this);
		ApplyInternalInventoryChanges(InventoryChanges);
	}
}

/***********************************************************
return letter info
***********************************************************/
LbaNet::LetterInfo SessionServant::GetLetterInfo(Ice::Long LetterId, const ::Ice::Current&)
{
	LbaNet::LetterInfo li = _dbh.GetLetterInfo(LetterId - _FIRST_USER_CREATED_ID_);
	if(li.Id >= 0)
		li.Id += _FIRST_USER_CREATED_ID_;

	return li;
}


/***********************************************************
destroy an inventory item
***********************************************************/
void SessionServant::DestroyItem(Ice::Long  Id, const ::Ice::Current&)
{
	Lock sync(*this);
	LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(Id);
	if(itlocal != _playerInventory.InventoryStructure.end())
	{
		UpdatedItemSeq InventoryChanges;
		LbaNet::UpdatedItem itm;
		itm.ItemId = Id;
		itm.NewCount = -itlocal->second.Number;
		itm.InformPlayer = false;
		InventoryChanges.push_back(itm);
		ApplyInternalInventoryChanges(InventoryChanges);
	}
}


/***********************************************************
set player targeted by actor
***********************************************************/
void SessionServant::SetTargeted(Ice::Long ActorId, const ::Ice::Current&)
{
	try
	{
		if(_actors_manager)
			_actors_manager->SetTargeted(_userNum, ActorId);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"SessionServant - Exception during SetTargeted: "<< ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"SessionServant - Unknown exception during SetTargeted"<<std::endl;
	}
}


/***********************************************************
set player untargeted by actor
***********************************************************/
void SessionServant::SetUnTargeted(Ice::Long ActorId, const ::Ice::Current&)
{
	try
	{
		if(_actors_manager)
			_actors_manager->SetUnTargeted(_userNum, ActorId);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"SessionServant - Exception during SetUnTargeted: "<< ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"SessionServant - Unknown exception during SetUnTargeted"<<std::endl;
	}
}



/***********************************************************
buy item
***********************************************************/
void SessionServant::BuyItem(Ice::Long FromActorId, Ice::Long Itemid, const ::Ice::Current& cur)
{
	int price = 0;
	int number = 0;

	{
		Lock sync(*this);
		std::map<long, ItemInfo>::iterator itdb = _inventory_db.find(Itemid);
		if(itdb != _inventory_db.end())
		{
			// get the price for the object
			price = itdb->second.Price;

			// check how much money we have
			LbaNet::InventoryMap::iterator iti = _playerInventory.InventoryStructure.find(8);
			if(iti != _playerInventory.InventoryStructure.end())
				number = iti->second.Number;
			else
				return;
		}
		else
			return;
	}


	//if we have enough money
	if(number >= price)
	{
		ItemList CheckedPut;
		CheckedPut[8] = price;
		ItemList CheckTaken;
		CheckTaken[Itemid] = 1;
		UpdateInventoryFromContainer(FromActorId, CheckTaken, CheckedPut, cur);
	}
}



/***********************************************************
start a quest
***********************************************************/
void SessionServant::StartQuest(Ice::Long QuestId, const ::Ice::Current&)
{
	_QH.TriggerQuestStart(QuestId);
}


/***********************************************************
end a quest
***********************************************************/
void SessionServant::EndQuest(Ice::Long QuestId, const ::Ice::Current&)
{
	_QH.TriggerQuestEnd(QuestId);
}



/***********************************************************
tell client only if actor is activated
***********************************************************/
void SessionServant::ActivatedActor(const LbaNet::ActorActivationInfo &ai, bool succeded, const ::Ice::Current&)
{
	if(succeded)
	{
		if(_client_observer)
			_client_observer->ActivatedActor(ai);
	}
	else
	{
		if(_client_observer)
			_client_observer->ActivationAborted(ai);
	}
}



/***********************************************************
get the number of item in inventory
***********************************************************/
int SessionServant::GetItemNumber(long id)
{
    Lock sync(*this);

	LbaNet::InventoryMap::iterator itlocal = _playerInventory.InventoryStructure.find(id);
	if(itlocal != _playerInventory.InventoryStructure.end())
	{
		return itlocal->second.Number;
	}
	else
		return 0;
}

/***********************************************************
update item number
***********************************************************/
void SessionServant::UpdateItemNumber(long itemid, int deltaCount)
{
	UpdatedItemSeq InventoryChanges;
	LbaNet::UpdatedItem itm;
    itm.ItemId = itemid;
    itm.NewCount = deltaCount;
    itm.InformPlayer = true;
	InventoryChanges.push_back(itm);

    Lock sync(*this);
	ApplyInternalInventoryChanges(InventoryChanges);
}	




/***********************************************************
inform class that a quest has been started
***********************************************************/
void SessionServant::InformQuestStarted(long Questid)
{
	try
	{
		if(_client_observer)
			_client_observer->InformQuestStarted(Questid);
		else
			std::cout<<"can not inform quest started :"<<Questid<<" - no client ptr"<<std::endl;
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during InformQuestStarted: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during InformQuestStarted"<<std::endl;
    }
}

/***********************************************************
inform class that a quest has been finished
***********************************************************/
void SessionServant::InformQuestFinished(long Questid)
{
	try
	{
		if(_client_observer)
			_client_observer->InformQuestFinished(Questid);
		else
			std::cout<<"can not inform quest finished :"<<Questid<<" - no client ptr"<<std::endl;
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during InformQuestStarted: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during InformQuestStarted"<<std::endl;
    }
}



/***********************************************************
init client with quests started and finished
***********************************************************/
void SessionServant::InitializeClientQuests(std::vector<long> questStarted, std::vector<long> questFinished)
{
	try
	{
		if(_client_observer)
		{
			LbaNet::QuestSeq started, finished;
			for(size_t i=0; i<questStarted.size(); ++i)
				started.push_back(questStarted[i]);
			for(size_t i=0; i<questFinished.size(); ++i)
				finished.push_back(questFinished[i]);

			_client_observer->InitQuestStartedFinished(started, finished);
		}
		else
			std::cout<<"can not init quest at client side - no client ptr"<<std::endl;
	}
    catch(const IceUtil::Exception& ex)
    {
		std::cout<<"SessionServant - Exception during InformQuestStarted: "<< ex.what()<<std::endl;
    }
    catch(...)
    {
		std::cout<<"SessionServant - Unknown exception during InformQuestStarted"<<std::endl;
    }
}



/***********************************************************
called when player throw magic ball
***********************************************************/
void SessionServant::MagicBallStart(const LaunchInfo & linfo, const ::Ice::Current&)
{
	if(_magicballused)
		return;

	_magicballused = true;
	if(_actors_manager)
		_actors_manager->MagicBallPlayed(_userNum, linfo);
}

/***********************************************************
called when magic ball is back
***********************************************************/
void SessionServant::MagicBallEnd(const ::Ice::Current&)
{
	if(!_magicballused)
		return;

	_magicballused = false;
}

/***********************************************************
called when magic ball touch an actor
***********************************************************/
void SessionServant::MagicBallTouchActor(Ice::Long ActorId, const ::Ice::Current&)
{
	if(!_magicballused)
		return;

	if(_actors_manager)
		_actors_manager->MagicBallTouchActor(_userNum, ActorId);
}

/***********************************************************
called when magic ball touch a player
***********************************************************/
void SessionServant::MagicBallTouchPlayer(Ice::Long ActorId, const ::Ice::Current&)
{
	if(!_magicballused)
		return;

	if(_actors_manager)
		_actors_manager->MagicBallTouchPlayer(_userNum, ActorId);
}

 
/***********************************************************
update current life of player - called by map server
***********************************************************/   
void SessionServant::UpdatedLife(const ActorLifeInfo &ali, const ::Ice::Current&)
{
	_lifeinfo = ali;
	_lifeinfo.ShouldHurt = false; // no use for us

	if(!_playerdead && _lifeinfo.CurrentLife <= 0)
	{
		_playerdead = true;

		if(_client_observer)
			_client_observer->Die();


		// record kill in database
		_dbh.RecordKill(_currWorldName, _userNum, ali.ChangeReason, ali.ChangeActorId);
	}
}