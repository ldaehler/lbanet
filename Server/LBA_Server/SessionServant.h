#ifndef _SESSION_SERVANT_H
#define _SESSION_SERVANT_H

#include <ClientSession.h>
#include <RoomManager.h>
#include <ActorInfo.h>
#include <ConnectedTracker.h>

#include <map>

#include "ChatParticipantServant.h"
#include "ActorsParticipantServant.h"

using namespace LbaNet;


class SessionServant : public ClientSession, public IceUtil::Mutex
{
public:
	//! constructor
	SessionServant(const std::string& userId, const RoomManagerPrx& manager, 
					const ConnectedTrackerPrx& ctracker);

	//! the client join a chat room
    virtual ChatRoomParticipantPrx JoinChat(const std::string& room, const ChatRoomObserverPrx& observer, 
												const Ice::Current&);

	//! the client leave a chat room
    virtual void LeaveChat(const std::string& room, const ChatRoomObserverPrx& observer, 
								const Ice::Current&);


	//! the client change actor information room
	virtual ActorsParticipantPrx ChangeRoom(	const std::string& newroom, 
												const std::string & actorname,
												const ActorsObserverPrx& observer, const Ice::Current&);


	//! destroy the session
	virtual void destroy(const Ice::Current&);

	//! get the list of people connected
	LbaNet::ConnectedList GetConnected(Ice::Long & ownid, const Ice::Current&);

	//! get server time
	Ice::Long GetTime(const Ice::Current&);

private:
	std::string							_userId;
	Ice::Long							_userNum;
	
	std::map<std::string, ChatRoomParticipantPrx>	_chat_rooms;

	std::string							_curr_actor_room;
	ActorsParticipantPrx				_actors_room;

	const RoomManagerPrx				_manager;
	const ConnectedTrackerPrx			_ctracker;

};

#endif
