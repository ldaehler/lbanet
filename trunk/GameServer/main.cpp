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
#include <boost/shared_ptr.hpp>

#include "ConnectionHandler.h"
#include "server.h"
#include "LogHandler.h"
#include "UserAllocatorHandler.h"
#include "MySQLDatabaseHandler.h"
#include "SynchronizedTimeHandler.h"

#include <signal.h>



bool global_continue = true;


//used to catch quit signal
void catch_quit_signal(int sig) 
{
	global_continue = false;
	(void) signal(SIGINT, SIG_DFL);
}



int main(int argc, char *argv[])
{
	//set up signal catcher
	(void) signal(SIGINT, catch_quit_signal);
	(void) signal(SIGTERM, catch_quit_signal);
	(void) signal(SIGBREAK, catch_quit_signal);

	
	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	//set up database
	boost::shared_ptr<DatabaseHandlerBase> dbH = boost::shared_ptr<DatabaseHandlerBase>(
		new MySQLDatabaseHandler("lbanet", "localhost", "lbanetuser", "lapichonmelba"));

	// set up data handler
	boost::shared_ptr<ServerDataHandler> dataH = boost::shared_ptr<ServerDataHandler>(new ServerDataHandler("Data/CitadelPvp/World.xml"));
	std::string worldN = dataH->GetWorlName();

	LogHandler::getInstance()->Init(worldN+"-GameServer.log");

	// set up connection class
	boost::shared_ptr<ConnectionHandler> ConH = 
		boost::shared_ptr<ConnectionHandler>(new ConnectionHandler(worldN+"-GameServer-Zoidcom.log"));


	//TODO - add config file
	// server operates on internal port 1 and UDP port 8899
	boost::shared_ptr<Server> Serv = boost::shared_ptr<Server>(new Server(1, 8900, 8000, 2000, 20, 200, 
																				dataH, "127.0.0.1:8899",
																				"127.0.0.1:8900", dbH.get()));

	// init time
	unsigned int lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();

	// zoidcom needs to get called regularly to get anything done so we enter the mainloop now
	while(global_continue)
	{
		// mesure the time used to do one cycle
		unsigned int waittime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();

		//finish process physics
		Serv->FinishProcessPhysic();

		// update replicators and tell zoidcom how much ingame time has passed since the last
		// time this was called
		unsigned int currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		unsigned int diff = (currtime-lasttime);
		lasttime = currtime;
		Serv->ZCom_processReplicators(diff);

		// processes incoming packets
		// all callbacks are generated from within the processInput calls
		Serv->ZCom_processInput( eZCom_NoBlock );


		//finish process physics
		Serv->ProcessPhysicHistoric();


		// process internal stuff
		Serv->Process();

		// outstanding data will be packed up and sent from here
		Serv->ZCom_processOutput();

		// pause the program for a few milliseconds
		unsigned int currwtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		unsigned int wdiff = (currwtime-waittime);
		if(wdiff < SIMULATION_TIME_PER_UPDATE)
			ZoidCom::Sleep(SIMULATION_TIME_PER_UPDATE-wdiff);

		//start process physics
		Serv->StartProcessPhysic();
	}

	// important to reset server before connection
	Serv.reset();

	// wait 10sec for server to deadvertize
	ZoidCom::Sleep(10000);

	ConH.reset();

	return 0;
}
