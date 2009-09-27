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

/***********************************************************
constructor
***********************************************************/
SessionServant::SessionServant(const std::string& userId, const RoomManagerPrx& manager,
									const ConnectedTrackerPrx& ctracker, const MapManagerPrx& map_manager,
									std::string	version)
: _manager(manager), _curr_actor_room(""), _userId(userId), _ctracker(ctracker), _map_manager(map_manager),
	_userNum(-1), _version(version)
{
	_userNum = _ctracker->Connect(_userId);

	_lifeinfo.CurrentLife = 40;
	_lifeinfo.MaxLife = 40;
	_lifeinfo.CurrentMana = 40;
	_lifeinfo.MaxMana = 40;
	_lifeinfo.ActorId = _userNum;
	_lifeinfo.Name = _userId;
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

	// if we are already in the new rooom do nothing
	if(_curr_actor_room == newroom)
		return _actors_room;


	// delete old room pointer
	if(_curr_actor_room != "")
	{
		current.adapter->remove(_actors_room->ice_getIdentity());
		_lifeinfo = _map_manager->LeaveMap(_curr_actor_room, _userNum);
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
	try
	{
		_ctracker->ChangeStatus(_userId, Status);

		std::map<std::string, ChatRoomParticipantPrx>::iterator worldit = _chat_rooms.find("World");
		if(worldit != _chat_rooms.end())
		{
			worldit->second->Say("info", "#status " + _userId + " " + Status);
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
	return _version;
}


/***********************************************************
return the current life state
***********************************************************/
LbaNet::ActorLifeInfo SessionServant::GetLifeInfo(const Ice::Current&)
{
	return _lifeinfo;
}

/***********************************************************
called when actor have been hurt
***********************************************************/
void SessionServant::GotHurtByActor(Ice::Long hurtingactorid, const Ice::Current&)
{
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