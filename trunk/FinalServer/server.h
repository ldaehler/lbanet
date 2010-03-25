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

#ifndef _LBANET_server_h_
#define _LBANET_server_h_

#include <boost/shared_ptr.hpp>
#include <zoidcom.h>
#include "ClientObjectHandler.h"
#include "ClientListHandlerBase.h"
#include "GameObject.h"
#include "DatabaseHandlerBase.h"

class ChatChannelManager;
class GameServerHandler;


class Server : public ZCom_Control
{
public:
	//! constructor
	Server( int _internalport, int _udpport, 
			unsigned int uplimittotal, unsigned int uplimitperconnection,
			unsigned short downpacketpersecond, unsigned short downbyteperpacket,
			ClientListHandlerBase* clH, DatabaseHandlerBase * dbH);

	//! destructor
	~Server();

	//! process server internal stuff
	void Process();

protected:


	// called on incoming connections
	eZCom_RequestResult ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, 
																		ZCom_BitStream &_reply );

	// called when incoming connection has been established
	void ZCom_cbConnectionSpawned( ZCom_ConnID _id );

	// called when a connection closed
	void ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata );

#ifndef _ZOID_USED_NEW_VERSION_
	// called when a connection wants to enter a channel
	bool ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_channel, ZCom_BitStream &_reason);

	// called when a connection enters a channel
	void ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason);
#else
	// called when a connection wants to enter a channel
	eZCom_RequestResult ZCom_cbChannelSubscriptionChangeRequest( ZCom_ConnID _id, 
											zU32 _requested_channel, ZCom_BitStream &_reason );

	// called when a connection enters a channel
	void ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason );
#endif


	// called when broadcast has been received
	eZCom_RequestResult ZCom_cbDiscoverRequest(const ZCom_Address &_addr, ZCom_BitStream &_request, 
													ZCom_BitStream &_reply) {return eZCom_DenyRequest;}

	//data received from the clients
	virtual void ZCom_cbDataReceived( ZCom_ConnID _id, ZCom_BitStream &_data ) {}


	// client stuff
	virtual void ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply ) {}
	virtual void ZCom_cbNodeRequest_Dynamic( ZCom_ConnID _id, ZCom_ClassID _requested_class, 
								ZCom_BitStream *_announcedata, eZCom_NodeRole _role, ZCom_NodeID _net_id ) {}
	virtual void ZCom_cbNodeRequest_Tag( ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, zU32 _tag ) {}
	virtual void ZCom_cbDiscovered( const ZCom_Address & _addr, ZCom_BitStream &_reply )  {}


private:
	int m_conncount;
	unsigned int m_uplimittotal;
	unsigned int m_uplimitperconnection;
	unsigned short m_downpacketpersecond;
	unsigned short m_downbyteperpacket;

	//client list handler
	ClientListHandlerBase* m_clH;

	//chat manager
	ChatChannelManager*	m_chatM;

	//client manager
	ClientObjectHandler m_clientH;

	GameServerHandler * m_gamesH;


	// database handler
	DatabaseHandlerBase *_dbh;

	//map connection id to player db id
	std::map<unsigned int, std::pair<long, std::string> > _playerDbMap;
};


#endif
