#include <IceBox/IceBox.h>

#include "PermissionsVerifierServant.h"
#include "ClientSessionManagerServant.h"

#ifndef CHATSERVICE_SERVICE_API
#   define CHATSERVICE_SERVICE_API ICE_DECLSPEC_EXPORT
#endif



class ClientSessionServiceServant : public IceBox::Service
{
public:

	virtual void start(const std::string& name, const Ice::CommunicatorPtr& communicator, 
							const Ice::StringSeq& args)
    {
		_adapter = communicator->createObjectAdapter(name + "-SessionMAdapter");


		_adapter->add(new PermissionsVerifierServant(), communicator->stringToIdentity(name + "-Verifier"));
		_adapter->add(new ClientSessionManagerServant(communicator), communicator->stringToIdentity(name + "-SessionManager"));
		_adapter->activate();
    }

    virtual void stop()
    {
		_adapter->deactivate();
    }

private:

    Ice::ObjectAdapterPtr _adapter;
};

extern "C"
{
	CHATSERVICE_SERVICE_API ::IceBox::Service* create(Ice::CommunicatorPtr communicator)
	{
		return new ClientSessionServiceServant;
	}
}
