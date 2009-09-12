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


#if !defined(__LbaNetModel_1_ServerConnectionHandler_h)
#define __LbaNetModel_1_ServerConnectionHandler_h

#include <Ice/Application.h>
#include <Glacier2/Router.h>
#include "ClientSession.h"
#include "IrcThread.h"

/***********************************************************************
 * Module:  ServerConnectionHandler.h
 * Author:  vivien
 * Purpose: Class used to take care of ice conenction to the server
 ***********************************************************************/

class ServerConnectionHandler
{
public:
	//! constructor
	ServerConnectionHandler(Ice::CommunicatorPtr communicator);

   //! destructor
   ~ServerConnectionHandler();

   //! connect to the server
   int Connect(const std::string &user, const std::string &password, bool &ircon, std::string & reason);

	//! disconnect from the server
	void Disconnect();

	//! get the irc thread
	IrcThread * GetIrcThread()
	{ return _ircth;}

	//! query the serrver to see if it is on
	bool ServerOn()
	{return _serveron;}

private:
	// pointer to the ice communicator
	Ice::CommunicatorPtr		_communicator;
	bool						_serveron;

	Glacier2::RouterPrx			_router;
	LbaNet::ClientSessionPrx	_session;
	Ice::ObjectAdapterPtr		_adapter;
	IceUtil::ThreadControl		_tc;
	bool						_thread_started;

	IceUtil::ThreadControl		_tcirc;
	IrcThread *					_ircth;
	bool						_ircOn;
};

#endif