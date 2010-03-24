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

#include "MainServerclient.h"
#include "LogHandler.h"
#include "ChatChannelManager.h"
#include "ChatChannel.h"
#include "ClientObject.h"
#include "GameServerHandler.h"



/***********************************************************************
 * Constructor
 ***********************************************************************/
MainServerClient::MainServerClient(unsigned short downpacketpersecond, unsigned short downbyteperpacket, 
									GameServerCallbackBase* callbH)
: m_id(ZCom_Invalid_ID), m_zoi_id(ZCom_Invalid_ID), m_connected(false),  
	m_downpacketpersecond(downpacketpersecond), m_downbyteperpacket(downbyteperpacket),	_callbH(callbH)
{
	// this will allocate the sockets and create local bindings
    if ( !ZCom_initSockets( eZCom_EnableUDP, 0, 0, 0 ) )
    {
		LogHandler::getInstance()->LogToFile("Main client: Failed to initialize sockets!", 2);
    }

    // string shown in log output
    ZCom_setDebugName("ZCOM_MainCLI");



	// register classes
	ChatChannelManager::registerClass(this);
	ChatChannel::registerClass(this);
	ClientObject::registerClass(this);
	GameServerHandler::registerClass(this);
}


/***********************************************************************
 * Destructor
 ***********************************************************************/
MainServerClient::~MainServerClient()
{

}



/***********************************************************************
 * connect to a server given an address + port
 ***********************************************************************/
void MainServerClient::ConnectToServer(const std::string & address, const std::string & login, 
								 const std::string & password, const std::string & excpectedversion)
{
	if(m_connected)
		return;

	// create target address 
	ZCom_Address dst_udp;
	dst_udp.setAddress( eZCom_AddressUDP, 0, address.c_str() );

	//prepare login info
    ZCom_BitStream *req = new ZCom_BitStream();
	req->addString( login.c_str() );
	req->addString( password.c_str() );
	req->addString( excpectedversion.c_str() );

	// connect to server
	if (!ZCom_Connect( dst_udp, req))
		LogHandler::getInstance()->LogToFile("Main client: unable to start connecting!", 2);
}

/***********************************************************************
 * called when initiated connection process yields a result
 ***********************************************************************/
void MainServerClient::ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply )
{
	m_zoi_id = _id;
	m_id = _reply.getInt(32);
	std::string reason(_reply.getStringStatic());

	if ( _result != eZCom_ConnAccepted )
	{
		m_connected = false;
	}
	else
	{
		m_connected = true;
		ZCom_requestDownstreamLimit(m_zoi_id, m_downpacketpersecond, m_downbyteperpacket);

		#ifndef _ZOID_USED_NEW_VERSION_
		ZCom_requestZoidMode( m_zoi_id, 1 );
		#else
		ZCom_changeObjectChannelSubscription( m_zoi_id, 1, eZCom_Subscribe );
		#endif
	}
}


/***********************************************************************
 * connection has closed
 ***********************************************************************/
void MainServerClient::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, 
									 ZCom_BitStream &_reasondata )
{
	m_connected = false;
}


/***********************************************************************
 * zoidlevel transition finished
 ***********************************************************************/
#ifndef _ZOID_USED_NEW_VERSION_
void MainServerClient::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason)
#else
void MainServerClient::ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason )
#endif
{
	// disconnect on failure
	if (_result == eZCom_ChannelSubscriptionDenied || _result == eZCom_ChannelSubscriptionFailed_Node)
	{
		std::stringstream strs;
		strs<<"Main Client: Connection with ID: "<<_id<<" channel subscription failed: "
				<<_new_channel<<" - disconnecting";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   

		ZCom_Disconnect(_id, NULL);
	}
	else
	{
		std::stringstream strs;
		strs<<"Main Clien: Connection with ID: "<<_id<<" channel subscription successfull: "<<_new_channel;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	}
}


/***********************************************************************
 * server wants to tell us about new node
 ***********************************************************************/
void MainServerClient::ZCom_cbNodeRequest_Dynamic(ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, 
											ZCom_NodeID _net_id)
{
	// if this is the channel handler
	if(_requested_class == ChatChannelManager::getClassID())
	{
		m_channelM = boost::shared_ptr<ChatChannelManager>(new ChatChannelManager(this, NULL));
	}


	// if this is the channel handler
	if(_requested_class == GameServerHandler::getClassID())
	{
		m_gameSM = boost::shared_ptr<GameServerHandler>(new GameServerHandler(this, _callbH));
	}
}



/***********************************************************
process server internal stuff
***********************************************************/
void MainServerClient::Process()
{
	//process game server M
	if(m_gameSM)
		m_gameSM->Process();

	//process channel manager
	if(m_channelM)
		m_channelM->Process();
}


/***********************************************************
close connection
***********************************************************/
void MainServerClient::CloseConnection()
{
	if(!m_connected)
		return;

	ZCom_Disconnect( m_zoi_id, NULL );

	// process until everything has been sent
	while(m_connected)
	{
		ZCom_processInput( eZCom_NoBlock );
		ZCom_processOutput();
		ZoidCom::Sleep(10);
	}
}


/***********************************************************
advertize game server to main
***********************************************************/
void MainServerClient::Advertize(const std::string & Name, const std::string & Address)
{
	if(m_gameSM)
		m_gameSM->RegisterGameServer(Name, Address);
}


/***********************************************************
deadvertize game server to main
***********************************************************/
void MainServerClient::Deadvertize(const std::string & Name)
{
	if(m_gameSM)
		m_gameSM->UnregisterGameServer(Name);
}



