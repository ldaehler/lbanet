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

#if defined(NDEBUG)
	#if defined(_MSC_VER)
		#include "SDL.h"
	#else
		#include "SDL/SDL.h"
	#endif
#endif

#include "LbaNetEngine.h"
#include "UserAllocatorHandler.h"
#include "ConnectionHandler.h"
#include "chatclient.h"
#include "LogHandler.h"
#include "InternalWorkpile.h"

#include <iostream>



class SimpleClientListHandler : public ClientListHandlerBase
{

public:
	// constructor
	SimpleClientListHandler(){}

	// destructor
	virtual ~SimpleClientListHandler(){}

	// new client connected
	virtual void Connected(unsigned int id, const std::string & Name)
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

private:
	std::map<unsigned int, std::string> _clientmap;

};


int main( int argc, char **argv )
{
	LogHandler::getInstance()->Init("LBAClient.log");


	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	// set up connection class
	ConnectionHandler* ConH = new ConnectionHandler("Zoidcom.log");

	boost::shared_ptr<ChatSubscriberBase> simpleSub(InternalWorkpile::getInstance());
	boost::shared_ptr<SimpleClientListHandler> clListH = boost::shared_ptr<SimpleClientListHandler>(new SimpleClientListHandler());

	// set up chat client
	ChatClient* Chatcl = new ChatClient(simpleSub, clListH, 30, 200);


	// start main thread engine
	LbaNetEngine engine(Chatcl);
	engine.run();

	Chatcl->CloseConnection();

	delete Chatcl;
	delete ConH;
	return 0;
}

