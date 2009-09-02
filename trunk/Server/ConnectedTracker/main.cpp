#include <Ice/Application.h>

#include "ConnectedTrackerServant.h"
#include "PermissionsVerifierServant.h"
#include "SharedData.h"


class LbaServer : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[])
    {
		SharedData shd;

		Ice::PropertiesPtr prop = communicator()->getProperties();
		_adapter = communicator()->createObjectAdapter(prop->getProperty("IdentityAdapter"));
		_adapter->add(new ConnectedTrackerServant(communicator(), &shd), communicator()->stringToIdentity(prop->getProperty("ConnectedServantName")));
		_adapter->add(new PermissionsVerifierServant(&shd), communicator()->stringToIdentity(prop->getProperty("VerifierServantName")));
		_adapter->activate();

		communicator()->waitForShutdown();

		return EXIT_SUCCESS;
	}

private:
	Ice::ObjectAdapterPtr _adapter;
};


int main(int argc, char** argv)
{
    LbaServer app;
    return app.main(argc, argv/*, "config"*/);
}

