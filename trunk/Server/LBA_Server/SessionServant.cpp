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



/***********************************************************
constructor
***********************************************************/
SessionServant::SessionServant(const std::string& userId, const RoomManagerPrx& manager,
									const ConnectedTrackerPrx& ctracker, const MapManagerPrx& map_manager,
									std::string	version, DatabaseHandler & dbh)
: _manager(manager), _curr_actor_room(""), _userId(userId), _ctracker(ctracker), _map_manager(map_manager),
	_userNum(-1), _version(version), _currColor("FFFFFFFF"), _dbh(dbh), _selfptr(NULL), _client_observer(NULL)
{
	_userNum = _ctracker->Connect(_userId);

	_lifeinfo.CurrentLife = 40;
	_lifeinfo.MaxLife = 40;
	_lifeinfo.CurrentMana = 40;
	_lifeinfo.MaxMana = 40;
	_lifeinfo.ActorId = _userNum;
	_lifeinfo.Name = _userId;

	_session_world_inventory_files["Lba1OriginalWorld"] = "Data/Inventory/lba1_inventory.xml";
	_session_world_inventory_files["Lba1Expanded"] = "Data/Inventory/lba1E_inventory.xml";
	_session_world_inventory_files["GiantCitadelWorld"] = "Data/Inventory/lba1_inventory.xml";
	_session_world_inventory_files["GiantPrincipalWorld"] = "Data/Inventory/lba1_inventory.xml";
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


	cleanEphemereItems();

	// if we are already in the new rooom do nothing
	if(_curr_actor_room == newroom)
		return _actors_room;


	// delete old room pointer
	if(_curr_actor_room != "")
	{
		LbaNet::ActorLifeInfo tmp = _map_manager->LeaveMap(_curr_actor_room, _userNum);
		current.adapter->remove(_actors_room->ice_getIdentity());

		_lifeinfo.CurrentLife = tmp.CurrentLife;
		if(_lifeinfo.CurrentLife < 0)
			_lifeinfo.CurrentLife = 0;
		if(_lifeinfo.CurrentLife > _lifeinfo.MaxLife)
			_lifeinfo.CurrentLife = _lifeinfo.MaxLife;

		_lifeinfo.CurrentMana= tmp.CurrentMana;
		if(_lifeinfo.CurrentMana < 0)
			_lifeinfo.CurrentMana = 0;
		if(_lifeinfo.CurrentMana > _lifeinfo.MaxMana)
			_lifeinfo.CurrentMana = _lifeinfo.MaxMana;

		_actors_manager = NULL;
	}


	// create new room handler
	Ice::Identity id;
    id.category = "_" + _userId;
    id.name = IceUtil::generateUUID();

	ActorsParticipantServant *actors_room_ptr = new ActorsParticipantServant(newroom, actorname, observer, _manager);
    _actors_room = ActorsParticipantPrx::uncheckedCast(current.adapter->add(actors_room_ptr, id));
	_curr_actor_room = newroom;
	_actors_manager = _map_manager->JoinMap(_curr_actor_room, _userNum, _lifeinfo);

    return _actors_room;
}



/***********************************************************
destroy the session
***********************************************************/
void SessionServant::destroy(const Ice::Current& current)
{
    Lock sync(*this);
	try
	{
		_dbh.UpdateInventory(_playerInventory, _currWorldName, _userNum);
		_dbh.QuitWorld(_currWorldName, _userNum);
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
			_actors_manager->ActivateActor(ai, _selfptr);
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
player has changed world
***********************************************************/
LbaNet::SavedWorldInfo SessionServant::ChangeWorld(const std::string& WorldName, 
														   const Ice::Current&)
{
    Lock sync(*this);

	cleanEphemereItems();

	// save old world info
	_dbh.UpdateInventory(_playerInventory, _currWorldName, _userNum);
	_dbh.QuitWorld(_currWorldName, _userNum);
	_currWorldName = WorldName;

	// retrieve info from db
	LbaNet::SavedWorldInfo swinfo = _dbh.ChangeWorld(WorldName, _userNum);

	// reload inventory
	_inventory_db.clear();
	MapInfoXmlReader::LoadInventory(_session_world_inventory_files[WorldName], _inventory_db);

	// only for test - remove that part
	if(swinfo.inventory.InventoryStructure.size() == 0)
	{
		LbaNet::InventoryItem itm;
		itm.Number = 1;
		itm.PlaceInInventory = 0;
		swinfo.inventory.InventoryStructure[7] = itm;
		itm.PlaceInInventory = 1;
		swinfo.inventory.InventoryStructure[5] = itm;
		itm.PlaceInInventory = 2;
		swinfo.inventory.InventoryStructure[6] = itm;


		swinfo.inventory.UsedShorcuts[0] = 7;
		swinfo.inventory.UsedShorcuts[1] = 5;
		swinfo.inventory.UsedShorcuts[2] = 6;
	}



	_playerInventory = swinfo.inventory;
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
	}

	try
	{
		if(clientseq.size() > 0 && _client_observer)
			return _client_observer->ApplyInventoryChanges(clientseq);
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
		if(_inventory_db[it->first].Ephemere)
		{
			LbaNet::UpdatedItem itm;
			itm.ItemId = it->first;
			itm.NewCount = 0;
			clientseq.push_back(itm);

			it = _playerInventory.InventoryStructure.erase(it);
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
			_actors_manager->AskForContainer(_userNum, ContainerId, _selfptr);
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
					int maxitem = _inventory_db[ittak->first].Max;
					int totake = std::min(maxitem, ittak->second);

					if(totake > 0)
						CheckTaken[ittak->first] = totake;
				}	
			}
		}


		// send to map handler to make change to container
		if(_actors_manager)
			_actors_manager->UpdateContainer(ContainerId, _userNum, CheckTaken, CheckedPut, _selfptr);
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
}


