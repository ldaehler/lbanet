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

#include "ServerConnectionHandler.h"
#include "LogHandler.h"
#include "SendingLoop.h"
#include "ThreadSafeWorkpile.h"
#include "md5.h"

/***********************************************************
	Constructor
***********************************************************/
ServerConnectionHandler::ServerConnectionHandler(Ice::CommunicatorPtr communicator)
: _communicator(communicator), _adapter(NULL), _session(NULL), _router(NULL),
	_thread_started(false), _ircOn(false), _ircth(NULL), _serveron(false)
{
	LogHandler::getInstance()->LogToFile("Trying to connect to glacier");
	Ice::RouterPrx defaultRouter = _communicator->getDefaultRouter();
	if(defaultRouter)
	{
		try
		{
			// timeout after 2 seconds if the serve does not respond
			Glacier2::RouterPrx::checkedCast(defaultRouter->ice_timeout(2000));
		}
		catch(const IceUtil::Exception& ex)
		{
			LogHandler::getInstance()->LogToFile(std::string("Connection to server failed") + ex.what());
			return;
		}
		catch(...)
		{
			LogHandler::getInstance()->LogToFile("Connection to server failed");
			return;
		}

		_serveron = true;
		LogHandler::getInstance()->LogToFile("Connection to server ok");
	}
}



/***********************************************************
	Destructor
***********************************************************/
ServerConnectionHandler::~ServerConnectionHandler()
{
	Disconnect();
}




/***********************************************************
connect to the server
***********************************************************/
int ServerConnectionHandler::Connect(const std::string &user, const std::string &password, 
									 bool &ircon, std::string & reason)
{
	Disconnect();

	Ice::RouterPrx defaultRouter = _communicator->getDefaultRouter();
	if(!defaultRouter)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not connect: no default router set"));
		return 0;
	}

	try
	{
		_router = Glacier2::RouterPrx::checkedCast(defaultRouter);
	}
	catch(const IceUtil::Exception& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception connecting to the server: ") + ex.what());
		return 0;
	}
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception connecting to the server. "));
		return 0;
	}

	if(!_router)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not connect:  configured router is not a Glacier2 router"));
		return 0;
	}

	std::string md5pass = MD5(password).hexdigest();

	try
	{
		_session = LbaNet::ClientSessionPrx::uncheckedCast(_router->createSession(user, md5pass));
	}
	catch(const Glacier2::PermissionDeniedException& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Permission denied: ") + ex.what());
		LogHandler::getInstance()->InformUser("Permission denied.");
		reason = ex.reason;
		return -1;
	}

	try
	{
		std::string verS = _session->GetVersion();
		Ice::PropertiesPtr prop = _communicator->getProperties();
		std::string serverV = prop->getPropertyWithDefault("LbanetServerVersion", "v0");
		if(verS != serverV)
		{
			reason = "Server version mismatch - please update your game.";
			Disconnect();
			return -1;
		}
	}
	catch(const Ice::Exception& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Error getting server version: ") + ex.what());
		return 0;
	}


	try
	{
		_adapter = _communicator->createObjectAdapter("LbaNetClient");
		_adapter->activate();

	}
	catch(const Ice::Exception& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Error creating adapter: ") + ex.what());
		return 0;
	}

	// clear join list
	ThreadSafeWorkpile::JoinEvent evcl;
	evcl.Clear = true;
	ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(evcl);

	// fill it with already connected people
	if(_session)
	{
		Ice::Long ownid = -1;
		LbaNet::ConnectedL listco = _session->GetConnected(ownid);
		ThreadSafeWorkpile::getInstance()->SetPlayerId(ownid);
		LbaNet::ConnectedL::const_iterator it = listco.begin();
		LbaNet::ConnectedL::const_iterator end = listco.end();
		for(;it != end; ++it)
		{
			ThreadSafeWorkpile::JoinEvent ev;
			ev.ListName = "online";
			ev.Joined = true;
			ev.Clear = false;
			ev.Nickname = it->first;
			ev.Status = it->second.Status;
			ev.Color = it->second.NameColor;
			ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
			ThreadSafeWorkpile::getInstance()->ChatColorChanged(ev.Nickname, ev.Color);
		}


		//synchronize time with server
		//SynchronizedTimeHandler::getInstance()->Initialize(session);
	}



	IceUtil::ThreadPtr t = new SendingLoopThread(_adapter, _session,
						_router->getServerProxy()->ice_getIdentity().category,
						user);
	_tc = t->start();
	_thread_started = true;


	//---------------------------------------------------------------
	// start the irc thread
	Ice::PropertiesPtr prop = _communicator->getProperties();
	_ircOn = (prop->getPropertyAsInt("IrcOn") == 1);
	std::string IrcServer = prop->getProperty("IrcServer");
	std::string IrcChannel = "#" + prop->getProperty("IrcChannel");
	if(_ircOn)
	{
		ircon = true;
		IrcThread::IrcCoInfo coi;
		coi.Server = IrcServer;
		coi.Nickname = user;
		coi.Channel = IrcChannel;

		_ircth = new IrcThread(coi);
		_tcirc = _ircth->start();
	}
	else
		ircon = false;



	return 1;
}


/***********************************************************
disconnect from the server
***********************************************************/
void ServerConnectionHandler::Disconnect()
{
	try
	{
		if(_adapter)
		{
			_adapter->deactivate();
			_adapter->destroy();
		}

		if(_thread_started)
		{
			ThreadSafeWorkpile::getInstance()->QuitSending();
			_tc.join();
			_thread_started = false;
		}

		if(_ircOn)
		{
			ThreadSafeWorkpile::getInstance()->QuitIrc();
			_ircth->Quit();
			_tcirc.join();
			_ircOn = false;
		}
	}
	catch(const Ice::Exception& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Error destroying the adapter: ") + ex.what());
	}

	try
	{
		if(_router)
			_router->destroySession();
	}
	catch(const Ice::Exception& ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Error destroying the session: ") + ex.what());
	}

	_router = NULL;
	_session = NULL;
	_adapter = NULL;
}