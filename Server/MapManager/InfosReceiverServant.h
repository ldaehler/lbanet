#ifndef __INFO_RECEIVER_SERVANT_H_
#define __INFO_RECEIVER_SERVANT_H_

#include <ActorInfo.h>
#include <string>

class MapHandler;

/***********************************************************
  Wrapper class containing the callback function used when a 
  message is received in IceStorm
***********************************************************/
class InfosReceiverServant : public LbaNet::ActorsObserver
{
public:

	explicit InfosReceiverServant(MapHandler * MH)
		: _MH(MH)
	{}

	// callback function called when a message is received from IceStorm
	virtual void UpdatedInfo(const LbaNet::ActorInfo& asi, const Ice::Current&);

	// callback function called when a message is received from IceStorm
	virtual void Quitted(const std::string& ActorName, const Ice::Current&){}
	
	// callback function called when a message is received from IceStorm    
	virtual void ActivatedActor(const LbaNet::ActorActivationInfo &ai, const Ice::Current&){}
	
	// callback function called when a message is received from IceStorm    
	virtual void SignaledActor(const LbaNet::ActorSignalInfo &ai, const Ice::Current&){}


private:
	MapHandler * _MH;
};

#endif
