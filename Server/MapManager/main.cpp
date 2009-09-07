#include <Ice/Application.h>

#include "MapManagerServant.h"


class LbaServer : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[])
    {
		Ice::PropertiesPtr prop = communicator()->getProperties();
		_adapter = communicator()->createObjectAdapter(prop->getProperty("IdentityAdapter"));
		_adapter->add(new MapManagerServant(communicator(), _adapter), communicator()->stringToIdentity(prop->getProperty("MapManagerServantName")));
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

