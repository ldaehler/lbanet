#include "SessionServant.h"
#include <IceUtil/UUID.h>
#include <Ice/Ice.h>

/***********************************************************
constructor
***********************************************************/
SessionServant::SessionServant(const std::string& userId, const RoomManagerPrx& manager, 
									const ConnectedTrackerPrx& ctracker)
: _manager(manager), _curr_actor_room(""), _userId(userId), _ctracker(ctracker), _userNum(-1)
{

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
		_userNum = _ctracker->Connect(_userId);
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
		current.adapter->remove(_actors_room->ice_getIdentity());


	// create new room handler
	Ice::Identity id;
    id.category = "_" + _userId;
    id.name = IceUtil::generateUUID();

	ActorsParticipantServant *actors_room_ptr = new ActorsParticipantServant(newroom, actorname, observer, _manager);
    _actors_room = ActorsParticipantPrx::uncheckedCast(current.adapter->add(actors_room_ptr, id));
	_curr_actor_room = newroom;

    return _actors_room;
}



/***********************************************************
destroy the session
***********************************************************/
void SessionServant::destroy(const Ice::Current& current)
{
    Lock sync(*this);

	std::map<std::string, ChatRoomParticipantPrx>::iterator worldit = _chat_rooms.find("World");
	if(worldit != _chat_rooms.end())
	{
		worldit->second->Say("info", "#left " + _userId);
		_ctracker->Disconnect(_userNum);
	}


	current.adapter->remove(current.id);


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

	_chat_rooms.clear();
}


/***********************************************************
get the list of people connected
***********************************************************/
LbaNet::ConnectedList SessionServant::GetConnected(Ice::Long & ownid, const Ice::Current&)
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