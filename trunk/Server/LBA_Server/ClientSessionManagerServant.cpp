#include "ClientSessionManagerServant.h"
#include "SessionServant.h"

#include <IceUtil/UUID.h>


ClientSessionManagerServant::ClientSessionManagerServant(const Ice::CommunicatorPtr& communicator)
{
	try
	{
		_manager = RoomManagerPrx::checkedCast(communicator->stringToProxy(
												communicator->getProperties()->getProperty("RoomManager")));
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception getting room manager proxy: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception getting room manager. "<<std::endl;
	}

	try
	{
		_ctracker = ConnectedTrackerPrx::checkedCast(communicator->stringToProxy(
														communicator->getProperties()->getProperty("ConnectedTracker")));
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception getting room manager proxy: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception getting room manager. "<<std::endl;
	}
}

ClientSessionManagerServant::~ClientSessionManagerServant()
{

}

Glacier2::SessionPrx ClientSessionManagerServant::create(	const std::string & userId,
															const Glacier2::SessionControlPrx &,
															const Ice::Current &current)
{
	Ice::Identity id;
    id.category = "_" + userId;
    id.name = IceUtil::generateUUID();
    return Glacier2::SessionPrx::uncheckedCast(current.adapter->add(new SessionServant(userId, _manager, _ctracker), id));
}
