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
#include "ChatChannelManager.h"
#include "ChatChannel.h"
#include "ClientObject.h"
#include "GameServerHandler.h"


#define	_CUR_LBANET_SERVER_VERSION_ "v0.8"

/***********************************************************
Constructor
***********************************************************/
Server::Server( int _internalport, int _udpport, 
				unsigned int uplimittotal, unsigned int uplimitperconnection,
				unsigned short downpacketpersecond, unsigned short downbyteperpacket,
				ClientListHandlerBase* clH, DatabaseHandlerBase * dbH)
: m_conncount(0), m_uplimittotal(uplimittotal), m_uplimitperconnection(uplimitperconnection), 
	m_downpacketpersecond(downpacketpersecond), m_downbyteperpacket(downbyteperpacket),
	m_chatM(NULL), m_clH(clH), _dbh(dbH)
{
	// this will allocate the sockets and create local bindings
	if ( !ZCom_initSockets( eZCom_EnableUDP, _udpport, _internalport, 0 ) )
	{
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
	}

	// string shown in log output
	ZCom_setDebugName("ZCOM_SRV");

	// maximum 8k/sec upstream and 2k/sec/connection
	ZCom_setUpstreamLimit(m_uplimittotal, m_uplimitperconnection);

	std::stringstream strs;
	strs<<"Server running and listening on udp port: "<<_udpport;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);


	// register classes
	ChatChannelManager::registerClass(this);
	ChatChannel::registerClass(this);
	ClientObject::registerClass(this);
	GameServerHandler::registerClass(this);

	//create chat manager
	m_chatM = new ChatChannelManager(this, clH);

	//create game server manager
	m_gamesH = new GameServerHandler(this, NULL);

}


/***********************************************************
Destructor
***********************************************************/
Server::~Server()
{
	if(m_chatM)
		delete m_chatM;

	if(m_gamesH)
		delete m_gamesH;
}




/***********************************************************
called on incoming connections
***********************************************************/
eZCom_RequestResult Server::ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, ZCom_BitStream &_reply )
{
	// retrieve request for login and password
	char login[20];
	_request.getString(login, 20);
	char password[20];
	_request.getString(password, 20);
	char version[20];
	_request.getString(version, 20);

	std::string loginS(login);
	std::string passwordS(password);
	std::string versionS(version);

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
	if ( login && password && version)
	{
		if(versionS == _CUR_LBANET_SERVER_VERSION_)
		{
			// check if it is a game server contacting us
			if(loginS == "GameServer" && passwordS == "GM2SVL2x")
			{
				std::stringstream strs;
				strs<<"Server: Incoming game server connection with ID: "<<_id<<" accepted";
				LogHandler::getInstance()->LogToFile(strs.str(), 2);    
				return eZCom_AcceptRequest;
			}
			else
			{
				//check if user is already connected
				std::map<unsigned int, std::pair<long, std::string> >::iterator itm = _playerDbMap.find(_id);
				if(itm != _playerDbMap.end())
				{
					//if so then refuse connection
					std::stringstream strs;
					strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
					LogHandler::getInstance()->LogToFile(strs.str(), 2);    

					// deny connection request and send reason back to requester
					_reply.addInt(_id, 32);
					_reply.addString( "User is already connected!" );
					return eZCom_DenyRequest;
				}

				//check database for login and pass correctness
				long db_id = _dbh->CheckLogin(login, versionS);
				if(db_id >= 0)
				{
					std::stringstream strs;
					strs<<"Server: Incoming connection with ID: "<<_id<<" accepted";
					LogHandler::getInstance()->LogToFile(strs.str(), 2);    

					//add to client list
					_playerDbMap[_id] = std::make_pair<long, std::string>(db_id, login);

					_reply.addInt(_id, 32);
					_reply.addString( "Good" );
					return eZCom_AcceptRequest;
				}
				else
				{
					std::stringstream strs;
					strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
					LogHandler::getInstance()->LogToFile(strs.str(), 2);    

					// deny connection request and send reason back to requester
					_reply.addInt(_id, 32);
					_reply.addString( "Incorrect username or password" );
					return eZCom_DenyRequest;
				}
			}


		}
		else
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    

			// deny connection request and send reason back to requester
			_reply.addString( "Server version mismatch - please update your game." );
			return eZCom_DenyRequest;
		}
	}
	else
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);    

		// deny connection request and send reason back to requester
		_reply.addString( "Empty username or password" );
		return eZCom_DenyRequest;
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


	std::map<unsigned int, std::pair<long, std::string> >::iterator itm = _playerDbMap.find(_id);
	if(itm != _playerDbMap.end())
	{
		//add to client list
		ClientObject * cl = new ClientObject(this, _id, itm->second.second, "", "", m_clH, NULL, _dbh, itm->second.first);
		cl->GetNode()->setOwner(_id, true);
		m_clientH.Addclient(_id, cl);

		// set connected in the db
		_dbh->SetUserConnected(itm->second.first);
	}



	//set as owner of chat manager and gameserverM
	m_chatM->GetNode()->setOwner(_id, true);
	m_gamesH->GetNode()->setOwner(_id, true);



	// request 20 packets/second and 200 bytes per packet from client (maximum values of course)
	ZCom_requestDownstreamLimit(_id, m_downpacketpersecond, m_downbyteperpacket);

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

	//clear client from channels
	if(m_chatM)
		m_chatM->ClientDisconnected(_id);

	//remove from client list
	m_clientH.Removeclient(_id);

	// disconnect from the db
	std::map<unsigned int, std::pair<long, std::string> >::iterator itm = _playerDbMap.find(_id);
	if(itm != _playerDbMap.end())
	{
		_dbh->DisconnectUser(itm->second.first);
		_playerDbMap.erase(itm);
	}


	--m_conncount;
}



/***********************************************************
called when a connection wants to enter a channel
***********************************************************/
#ifdef _ZOID_USED_NEW_VERSION_
eZCom_RequestResult Server::ZCom_cbChannelSubscriptionChangeRequest( ZCom_ConnID _id, 
											zU32 _requested_channel, ZCom_BitStream &_reason )
#else
bool Server::ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_channel, ZCom_BitStream &_reason)
#endif
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" wants to enter the channel "<<_requested_channel;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);

	return eZCom_AcceptRequest;
}


/***********************************************************
called when a connection enters a channel
***********************************************************/
#ifdef _ZOID_USED_NEW_VERSION_
void Server::ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason )
#else
void Server::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason)
#endif
{
	//channel connection accepted
	if(_result == eZCom_RequestedChannelSubscribed || _result == eZCom_AutomaticChannelSubscribed)
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" entered the channel "<<_new_channel;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);
	}
}




/***********************************************************
process server internal stuff
***********************************************************/
void Server::Process()
{
	//process chat
	if(m_chatM)
		m_chatM->Process();

	//process client handler
	m_clientH.Process();

	//process game server manager
	if(m_gamesH)
		m_gamesH->Process();
	
}