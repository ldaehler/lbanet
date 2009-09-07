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


#ifndef _ROOM_MANAGER_SERVANT_H
#define _ROOM_MANAGER_SERVANT_H

#include <ConnectedTracker.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

using namespace LbaNet;

#include "SharedData.h"


class ConnectedTrackerServant : public ConnectedTracker
{
public:
	//! constructor
    ConnectedTrackerServant(const Ice::CommunicatorPtr& communicator, SharedData * shd);

	//! add a connected person
    virtual Ice::Long Connect(const std::string& Nickname, const Ice::Current&);

	//! remove a disconnected person
    virtual void Disconnect(Ice::Long Id, const Ice::Current&);

	//! get list of connected people
    virtual LbaNet::ConnectedL GetConnected(const Ice::Current&);


private:
	LbaNet::ConnectedL			_connected;
	const Ice::CommunicatorPtr& _communicator;

	SharedData *				_shd;
};

#endif
