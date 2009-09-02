#include <Ice/Application.h>

#include "ClientSessionManagerServant.h"

class LbaServer : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[])
    {
		Ice::PropertiesPtr prop = communicator()->getProperties();
		_adapter = communicator()->createObjectAdapter(prop->getProperty("IdentityAdapter"));
		_adapter->add(new ClientSessionManagerServant(communicator()), communicator()->stringToIdentity(prop->getProperty("SessionMServantName")));
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

