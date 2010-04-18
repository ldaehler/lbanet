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

#include "gameclient.h"
#include "LogHandler.h"
#include "md5.h"
#include "InternalWorkpile.h"
#include "GameEvents.h"
#include "MapInfoObject.h"
#include "ActorObject.h"
#include "ZoidSerializer.h"
#include "ObjectsDescription.h"
#include "MusicHandler.h"
#include "InternalWorkpile.h"
#include "PlayerObject.h"

/***********************************************************************
 * Constructor
 ***********************************************************************/
GameClient::GameClient(unsigned short downpacketpersecond, unsigned short downbyteperpacket)
: m_id(ZCom_Invalid_ID), m_zoi_id(ZCom_Invalid_ID), m_connected(false),  
	m_downpacketpersecond(downpacketpersecond), m_downbyteperpacket(downbyteperpacket),
	m_disconnecting(false), m_callback(NULL)
{
	// this will allocate the sockets and create local bindings
    if ( !ZCom_initSockets( eZCom_EnableUDP, 0, 0, 0 ) )
    {
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
    }

    // string shown in log output
    ZCom_setDebugName("ZCOM_GAME_CLI");


	//register classes
	MapInfoObject::registerClass(this);
	ActorObject::registerClass(this);
	PlayerObject::registerClass(this);
}


/***********************************************************************
 * Destructor
 ***********************************************************************/
GameClient::~GameClient()
{

}



/***********************************************************************
 * connect to a server given an address + port
 ***********************************************************************/
void GameClient::ConnectToServer(const std::string & servername, const std::string & address, 
								 const std::string & login, 
								 const std::string & password, const std::string & excpectedversion,
								 GameClientCallbackBase * callback)
{
	m_callback = callback;

	if(m_connected)
	{
		if(m_servername == servername)
		{
			LogHandler::getInstance()->LogToFile("Zoid: Already connected to chat server - skipping connection", 2);
			return;
		}
		else
			CloseConnection();
	}

	m_servername = servername;

	std::string md5pass = MD5(password).hexdigest();


	// create target address 
	ZCom_Address dst_udp;
	dst_udp.setAddress( eZCom_AddressUDP, 0, address.c_str() );

	//prepare login info
    ZCom_BitStream *req = new ZCom_BitStream();
	req->addString( login.c_str() );
	req->addString( md5pass.c_str() );
	req->addString( excpectedversion.c_str() );

	// connect to server
	LogHandler::getInstance()->LogToFile("Game client connecting to "+address, 2);

	//m_id = ZCom_Connect( dst_udp, req);
	if (!ZCom_Connect( dst_udp, req))
		LogHandler::getInstance()->LogToFile("Game client: unable to start connecting!", 2);
}


/***********************************************************************
 * called when initiated connection process yields a result
 ***********************************************************************/
void GameClient::ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply )
{
	m_zoi_id = _id;
	m_id = _reply.getInt(32);
	std::string reason(_reply.getStringStatic());

	std::stringstream strs;
	strs<<"Game Client: The connection process for: "<<m_zoi_id<<" returned with resultcode "<<_result<<", the reply was "<<reason;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	if ( _result != eZCom_ConnAccepted )
	{
		m_connected = false;
		if(reason == "")
			reason = "Server not reachable";

		InternalWorkpile::getInstance()->AddEvent(new GameErrorMessageEvent("Problem connecting to game server " + m_servername + " - " + reason));
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
void GameClient::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, 
									 ZCom_BitStream &_reasondata )
{
	std::stringstream strs;
	strs<<"Game Client: Connection with ID: "<<_id<<" has been closed, reason is: "<<_reasondata.getStringStatic();
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	m_connected = false;

	//disconnect and get to login screen
	if(!m_disconnecting)
		InternalWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(0));
}


/***********************************************************************
 * zoidlevel transition finished
 ***********************************************************************/
#ifndef _ZOID_USED_NEW_VERSION_
void GameClient::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason)
#else
void GameClient::ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason )
#endif
{
	// disconnect on failure
	if (_result == eZCom_ChannelSubscriptionDenied || _result == eZCom_ChannelSubscriptionFailed_Node)
	{
		std::stringstream strs;
		strs<<"Game Client: Connection with ID: "<<_id<<" channel subscription failed: "
				<<_new_channel<<" - disconnecting";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   

		ZCom_Disconnect(_id, NULL);
	}
	else
	{
		std::stringstream strs;
		strs<<"Game Client: Connection with ID: "<<_id<<" channel subscription successfull: "<<_new_channel;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	}
}


/***********************************************************************
 * server wants to tell us about new node
 ***********************************************************************/
void GameClient::ZCom_cbNodeRequest_Dynamic(ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, 
											ZCom_NodeID _net_id)
{
	// if this is the map info 
	if(_requested_class == MapInfoObject::getClassID())
	{
		char buffname[250], bufftype[250], buffmusic[250];
		_announcedata->getString(buffname, 250);
		_announcedata->getString(bufftype, 250);
		_announcedata->getString(buffmusic, 250);
		int musicloop = _announcedata->getSignedInt(16);

		m_maps_objects.push_back(boost::shared_ptr<MapInfoObject>(new MapInfoObject(this)));
		InternalWorkpile::getInstance()->AddEvent(new NewMapEvent(buffname, bufftype));
		MusicHandler::getInstance()->PlayMusic(std::string("Data/") + buffmusic, musicloop);
	}

	// if this is an actor
	if(_requested_class == ActorObject::getClassID())
	{
		ZoidSerializer zserialize(_announcedata);
		ObjectInfo oinfo(&zserialize);
		m_actors[_id] = boost::shared_ptr<ActorObject>(new ActorObject(this, 1, _id, oinfo, m_callback));	
	}


	// if this is a player
	if(_requested_class == PlayerObject::getClassID())
	{
		ZoidSerializer zserialize(_announcedata);
		ObjectInfo oinfo(&zserialize);

		//set object as non controllablae if we are not the owner
		if(_role == eZCom_RoleProxy)
			oinfo.PhysInfo->SetNonControllable();

		m_players[_id] = boost::shared_ptr<PlayerObject>(
					new PlayerObject(this, 1, _id, oinfo, m_callback, NULL, (_role == eZCom_RoleOwner)));	
	}


	
}



/***********************************************************
process server internal stuff
***********************************************************/
void GameClient::Process()
{
	//process map objects
	{
		std::list<boost::shared_ptr<MapInfoObject> >::iterator it = m_maps_objects.begin();
		while(it != m_maps_objects.end())
		{
			(*it)->Process();
			if((*it)->isGarbage())
				it = m_maps_objects.erase(it);
			else
				++it;
		}
	}

	//process actors
	{
		std::map<unsigned int, boost::shared_ptr<ActorObject> >::iterator it = m_actors.begin();
		while(it != m_actors.end())
		{
			it->second->Process();
			if(it->second->isGarbage())
				it = m_actors.erase(it);
			else
				++it;
		}
	}

	//process players
	{
		std::map<unsigned int, boost::shared_ptr<PlayerObject> >::iterator it = m_players.begin();
		while(it != m_players.end())
		{
			it->second->Process();
			if(it->second->isGarbage())
				it = m_players.erase(it);
			else
				++it;
		}
	}

}



/***********************************************************
close connection
***********************************************************/
void GameClient::CloseConnection()
{
	if(!m_connected)
		return;

	m_disconnecting = true;

	ZCom_Disconnect( m_zoi_id, NULL );

	// process until everything has been sent
	while(m_connected)
	{
		ZCom_processInput( eZCom_NoBlock );
		ZCom_processOutput();
		ZoidCom::Sleep(10);
	}


	//clean up everything
	m_maps_objects.clear();
	m_actors.clear();


	m_disconnecting = false;
}



/***********************************************************
data received from the server
***********************************************************/
void GameClient::ZCom_cbDataReceived( ZCom_ConnID _id, ZCom_BitStream &_data )
{
	//check message type
	unsigned int type = _data.getInt(4);

	switch(type)
	{
		//player is in waiting queue
		case 0:
			//inform user through the gui
			InternalWorkpile::getInstance()->AddEvent(new GameErrorMessageEvent("Map server is full. You have been queued for connection. Please wait for a moment you will get connected automatically..."));
		break;

		//change zoidlevel
		case 1:
			unsigned int newlvl = _data.getInt(32);

			std::stringstream strs;
			strs<<"Game Client: Changing zoidlevel to: "<<newlvl;
			LogHandler::getInstance()->LogToFile(strs.str(), 2);   

			ZCom_requestZoidMode(newlvl, 1);
		break;


	}
}
