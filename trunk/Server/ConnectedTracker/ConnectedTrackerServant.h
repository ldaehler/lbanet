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
