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
#include "ChatSubscriberBase.h"

#include <iostream>

class ChatSubscriberSimple : public ChatSubscriberBase
{

public:
	// constructor
	ChatSubscriberSimple(){}

	// destructor
	virtual ~ChatSubscriberSimple(){}

	// received text message
	virtual void ReceivedText(const std::string & SenderName, const std::string & Text)
	{
		std::cout<<SenderName<<": "<<Text<<std::endl;
	}
};

int main( int argc, char **argv )
{
	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	// set up connection class
	boost::shared_ptr<ConnectionHandler> ConH = boost::shared_ptr<ConnectionHandler>(new ConnectionHandler("Zoidcom.log"));

	boost::shared_ptr<ChatSubscriberSimple> simpleSub(new ChatSubscriberSimple());

	// set up chat client
	boost::shared_ptr<ChatClient> Chatcl = boost::shared_ptr<ChatClient>(new ChatClient(simpleSub));


	// start main thread engine
	LbaNetEngine engine(Chatcl);
	engine.run();

	Chatcl->CloseConnection();

	Chatcl.reset();
	ConH.reset();
	return 0;
}

