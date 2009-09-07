#ifndef _CLIENT_SESSION_MANAGER_SERVANT_H
#define _CLIENT_SESSION_MANAGER_SERVANT_H

#include <Ice/Ice.h>
#include <Glacier2/Session.h>
#include <RoomManager.h>
#include <ConnectedTracker.h>
#include <MapManager.h>

class ClientSessionManagerServant : public Glacier2::SessionManager
{
public:

    ClientSessionManagerServant(const Ice::CommunicatorPtr& communicator);
    ~ClientSessionManagerServant();

    virtual Glacier2::SessionPrx create(const std::string & userId,const Glacier2::SessionControlPrx &,
											const Ice::Current &current);

private:
	LbaNet::RoomManagerPrx			_manager;
	LbaNet::ConnectedTrackerPrx		_ctracker;
	LbaNet::MapManagerPrx			_map_manager;
};

#endif
