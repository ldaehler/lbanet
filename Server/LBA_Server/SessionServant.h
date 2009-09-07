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


#ifndef _SESSION_SERVANT_H
#define _SESSION_SERVANT_H

#include <ClientSession.h>
#include <RoomManager.h>
#include <ActorInfo.h>
#include <ConnectedTracker.h>
#include <MapManager.h>

#include <map>

#include "ChatParticipantServant.h"
#include "ActorsParticipantServant.h"

using namespace LbaNet;


class SessionServant : public ClientSession, public IceUtil::Mutex
{
public:
	//! constructor
	SessionServant(const std::string& userId, const RoomManagerPrx& manager,
					const ConnectedTrackerPrx& ctracker, const MapManagerPrx& map_manager);

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


	// callback function called when an actor id activated
    virtual void ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&);

	// callback function called when an actor id signaled
    virtual void SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&);

	//get updated info
	virtual LbaNet::UpdateSeq GetUpdatedInfo(const Ice::Current&);


	//! get server time
	Ice::Long GetTime(const Ice::Current&);

private:
	std::string							_userId;
	Ice::Long							_userNum;

	std::map<std::string, ChatRoomParticipantPrx>	_chat_rooms;

	std::string							_curr_actor_room;
	ActorsParticipantPrx				_actors_room;
	LbaNet::MapObserverPrx				_actors_manager;

	const RoomManagerPrx				_manager;
	const ConnectedTrackerPrx			_ctracker;
	const MapManagerPrx					_map_manager;

};

#endif
