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


#ifndef _ACTORS_PARTICIPANT_SERVANT_H
#define _ACTORS_PARTICIPANT_SERVANT_H

#include <ActorInfo.h>
#include <RoomManager.h>

using namespace LbaNet;


class ActorsParticipantServant : public ActorsParticipant
{
public:
	//! constructor
	ActorsParticipantServant(const std::string& room, std::string actor_name,
								const ActorsObserverPrx& observer,
								const RoomManagerPrx& manager);

	//! destructor
	~ActorsParticipantServant();

	//! update
	virtual void Update(const ActorInfo& asi, const Ice::Current &current);


private:
	const std::string _room;
	const std::string _actor_name;
    const RoomManagerPrx _manager;
    const ActorsObserverPrx _observer;
    const ActorsObserverPrx _publish;
};

#endif
