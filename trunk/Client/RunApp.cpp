/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/

#include <Ice/Application.h>

#include "ServerConnectionHandler.h"
#include "LbaNetEngine.h"
#include "RunApp.h"
#include "LogHandler.h"

class IceClient : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[])
    {
		shutdownOnInterrupt();
		LogHandler::getInstance()->LogToFile("Checking connection with the server...");
		ServerConnectionHandler serverH(communicator());

		LogHandler::getInstance()->LogToFile("Reading properties...");
		Ice::PropertiesPtr prop = communicator()->getProperties();
		std::string clientV = prop->getPropertyWithDefault("LbanetClientVersion", "v0");

		LogHandler::getInstance()->LogToFile("Initializing the game engine...");
		LbaNetEngine engine(&serverH, clientV);
		LogHandler::getInstance()->LogToFile("Starting the game engine...");
		engine.run();


		serverH.Disconnect();
		communicator()->shutdown();
		communicator()->waitForShutdown();
		return EXIT_SUCCESS;
	}
};


/***********************************************************
run function
***********************************************************/
int RunApp::Run(int argc, char *argv[])
{
	LogHandler::getInstance()->LogToFile("Entering main program...");
    IceClient app;
    return app.main(argc, argv, "config.client");
}