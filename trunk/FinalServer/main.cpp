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

class SimpleClientListHandler : public ClientListHandlerBase
{

public:
	// constructor
	SimpleClientListHandler(){}

	// destructor
	virtual ~SimpleClientListHandler(){}

	// new client connected
	virtual void Connected(unsigned int id, const std::string & Name, 
								const std::string & Status, const std::string & Color)
	{
		#ifdef _DEBUG
			std::stringstream strs;
			strs<<"Client "<<id<<" with name "<<Name<<" connected ";
			LogHandler::getInstance()->LogToFile(strs.str());
		#endif

		_clientmap[id] = Name;
	}

	// client disconnected
	virtual void Disconnected(unsigned int id)
	{
		#ifdef _DEBUG
			std::stringstream strs;
			strs<<"Client "<<id<<" with name "<<_clientmap[id]<<" disconnected ";
			LogHandler::getInstance()->LogToFile(strs.str());
		#endif

		std::map<unsigned int, std::string>::iterator it = _clientmap.find(id);
		if(it != _clientmap.end())
			_clientmap.erase(it);
	}

	// return the name given a client id
	virtual std::string GetName(unsigned int id)
	{
		return _clientmap[id];
	}

	// client changed status
	virtual void ChangedStatus(unsigned int id, const std::string & Status, const std::string & Color){}

private:
	std::map<unsigned int, std::string> _clientmap;

};




int main(int argc, char *argv[])
{
	LogHandler::getInstance()->Init("ChatServer.log");

	// set up connection class
	boost::shared_ptr<ConnectionHandler> ConH = boost::shared_ptr<ConnectionHandler>(new ConnectionHandler("Zoidcom.log"));

	// server operates on internal port 1 and UDP port 8899
	boost::shared_ptr<SimpleClientListHandler> clListH = boost::shared_ptr<SimpleClientListHandler>(new SimpleClientListHandler());
	boost::shared_ptr<Server> Serv = boost::shared_ptr<Server>(new Server(1, 8899, 8000, 2000, 20, 200, clListH.get()));



	// zoidcom needs to get called regularly to get anything done so we enter the mainloop now
	while (1)
	{
		// processes incoming packets
		// all callbacks are generated from within the processInput calls
		Serv->ZCom_processInput( eZCom_NoBlock );

		// process internal stuff
		Serv->Process();

		// outstanding data will be packed up and sent from here
		Serv->ZCom_processOutput();

		// pause the program for a few milliseconds
		ZoidCom::Sleep(10);
	}
}
