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

#include "server.h"
#include "LogHandler.h"


/***********************************************************
Constructor
***********************************************************/
Server::Server( int _internalport, int _udpport )
: m_conncount(0)
{
	// this will allocate the sockets and create local bindings
	if ( !ZCom_initSockets( true, _udpport, _internalport, 0 ) )
	{
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
	}

	// string shown in log output
	ZCom_setDebugName("ZCOM_SRV");

	// maximum 8k/sec upstream and 2k/sec/connection
	ZCom_setUpstreamLimit(8000, 2000);

	std::stringstream strs;
	strs<<"Server running and listening on udp port: "<<_udpport;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);    
}


/***********************************************************
Destructor
***********************************************************/
Server::~Server()
{

}




/***********************************************************
called on incoming connections
***********************************************************/
bool Server::ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, ZCom_BitStream &_reply )
{
	// retrieve request for login and password
	const char * login = _request.getStringStatic();
	const char * password = _request.getStringStatic();

	// address information
	const ZCom_Address* addr = ZCom_getPeer( _id );
	if ( addr )
	{
		if ( addr->getType() == eZCom_AddressLocal )
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection from localport: "<<addr->getPort();
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
		}
		else if ( addr->getType() == eZCom_AddressUDP )
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection from UDP: "<<(int)addr->getIP( 0 )<<"."<<(int)addr->getIP( 1 )
				<<"."<<(int)addr->getIP( 2 )<<"."<<(int)addr->getIP( 3 )<<":"<<(int)addr->getPort();
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
		}
	}

	// check what the client is requesting
	if ( login && password && strlen( password ) > 0 && strcmp( password, "letmein2" ) == 0 )
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" accepted";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);    

		return true;
	}
	else
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);    

		// deny connection request and send reason back to requester
		_reply.addString( "Incorrect usernam or password" );
		return false;
	}
}


/***********************************************************
called when incoming connection has been established
***********************************************************/
void Server::ZCom_cbConnectionSpawned( ZCom_ConnID _id )
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" has been established.";
	LogHandler::getInstance()->LogToFile(strs.str(), 2);    


	// request 20 packets/second and 200 bytes per packet from client (maximum values of course)
	ZCom_requestDownstreamLimit(_id, 20, 200);

	++m_conncount;
}


/***********************************************************
called when a connection closed
***********************************************************/
void Server::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata )
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" has been closed.";
	LogHandler::getInstance()->LogToFile(strs.str(), 2);  

	--m_conncount;
}



/***********************************************************
called when a connection wants to enter a channel
***********************************************************/
bool Server::ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_level, ZCom_BitStream &_reason)
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" wants to enter the channel "<<(int)_requested_level;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);

	return true;
}


/***********************************************************
called when a connection enters a channel
***********************************************************/
void Server::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_level, ZCom_BitStream &_reason)
{
	//channel connection accepted
	if(_result == eZCom_ZoidEnabled)
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" entered the channel "<<_new_level;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);
	}
}


