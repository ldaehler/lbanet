#include "ConnectedTrackerServant.h"
#include <algorithm>


/***********************************************************
constructor
***********************************************************/
ConnectedTrackerServant::ConnectedTrackerServant(const Ice::CommunicatorPtr& communicator, SharedData * shd)
: _communicator(communicator), _shd(shd)
{

}

/***********************************************************
add a connected person
***********************************************************/
Ice::Long ConnectedTrackerServant::Connect(const std::string& Nickname, const Ice::Current&)
{
	return _shd->GetId(Nickname);
}

/***********************************************************
remove a disconnected person
***********************************************************/
void ConnectedTrackerServant::Disconnect(Ice::Long Id, const Ice::Current&)
{
	_shd->Disconnect(Id);
}

/***********************************************************
get list of connected people
***********************************************************/
LbaNet::ConnectedL ConnectedTrackerServant::GetConnected(const Ice::Current&)
{
	return _shd->GetConnected();
}