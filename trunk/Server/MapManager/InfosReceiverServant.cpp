#include "InfosReceiverServant.h"
#include "MapHandler.h"

// callback function called when a message is received from IceStorm
void InfosReceiverServant::UpdatedInfo(const LbaNet::ActorInfo& asi, const Ice::Current&)
{
	_MH->UpdatedInfo(asi);
}

