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


#ifndef _LBANET_CHAT_client_h_
#define _LBANET_CHAT_client_h_

#include <zoidcom.h>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "ChatSubscriberBase.h"
#include "ClientObjectHandler.h"
#include "ClientListHandlerBase.h"
#include "GameObject.h"

class ChatChannelManager;
class LbaNetEngine;



class ChatClient : public ZCom_Control
{
public:

	//! constructor
	ChatClient(ChatSubscriberBase* WorldSubscriber,	ClientListHandlerBase* clH,
					unsigned short downpacketpersecond, unsigned short downbyteperpacket);

	//!destructor
	~ChatClient();

	//! connect to a server given an address + port
	void ConnectToServer(const std::string & address, const std::string & login, 
							const std::string & password, const std::string & excpectedversion,
							LbaNetEngine * engine);

	//! check if client is connected
	bool IsConnected() {return m_connected;}

	//! check if client is connected
	bool HasChannelManager() {return (m_channelM != NULL);}


	//! subscribe to world channel
	void SubscribeWorld();



	//! subscribe to channel 'name'
	void SubscribeChannel(const std::string & channelname, ChatSubscriberBase* Subscriber);

	//! unsubscribe to channel 'name'
	void UnsubscribeChannel(const std::string & channelname);

	//! send text to a specific channel
	void SendText(const std::string & channelname, const std::string & text);

	//! process server internal stuff
	void Process();

	// close connection
	void CloseConnection();

	//! change player status
	void ChangeStatus(const std::string & status);

	//! change player color
	void ChangeColor(const std::string & color);

	//! whisper to someone 
	bool Whisper(const std::string & playername, const std::string & text);


protected:
	// called when initiated connection process yields a result
	void ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply );

	// connection has closed
	void ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata );

	
	// server wants to tell us about new node
	void ZCom_cbNodeRequest_Dynamic(ZCom_ConnID _id, ZCom_ClassID _requested_class, 
								ZCom_BitStream *_announcedata, eZCom_NodeRole _role, ZCom_NodeID _net_id);

	//data received from the server
	virtual void ZCom_cbDataReceived( ZCom_ConnID _id, ZCom_BitStream &_data ) {}


#ifndef _ZOID_USED_NEW_VERSION_
	// called when a connection wants to enter a channel
	bool ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_channel, ZCom_BitStream &_reason){return false;}

	// called when a connection enters a channel
	void ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason);
#else
	// called when a connection wants to enter a channel
	eZCom_RequestResult ZCom_cbChannelSubscriptionChangeRequest( ZCom_ConnID _id, 
											zU32 _requested_channel, ZCom_BitStream &_reason ) {return eZCom_DenyRequest;}

	// called when a connection enters a channel
	void ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason );
#endif


	// server stuff
	virtual eZCom_RequestResult ZCom_cbConnectionRequest( ZCom_ConnID  _id, ZCom_BitStream &_request, ZCom_BitStream &_reply ) {return eZCom_DenyRequest;}
	virtual void ZCom_cbConnectionSpawned( ZCom_ConnID _id ) {}
	virtual void ZCom_cbNodeRequest_Tag( ZCom_ConnID _id, ZCom_ClassID _requested_class, ZCom_BitStream *_announcedata, eZCom_NodeRole _role, zU32 _tag ) {}
	virtual eZCom_RequestResult ZCom_cbDiscoverRequest( const ZCom_Address &_addr, ZCom_BitStream &_request, ZCom_BitStream &_reply ) {return eZCom_DenyRequest;}
	virtual void ZCom_cbDiscovered( const ZCom_Address & _addr, ZCom_BitStream &_reply )  {}


	// when a text has been written on the chat
	void HandleChatText();

	// process a line of text
	void ProcessText(const std::string & Text);

	// check if player is afk
	void CheckAfk();

private:
	// id given by the server
	ZCom_ConnID		m_zoi_id;
	ZCom_ConnID		m_id;
	bool			m_connected;

	unsigned short m_downpacketpersecond;
	unsigned short m_downbyteperpacket;

	boost::shared_ptr<ChatChannelManager> m_channelM;


	std::map<std::string, ChatSubscriberBase* >	m_waitingsubs;

	ClientObjectHandler m_clientHandler;

	//client list handler
	ClientListHandlerBase* m_clH;
	ChatSubscriberBase* m_WorldSubscriber;


	//used for callback
	LbaNetEngine * _engine;

	// afk counters
	unsigned long			_afk_counter;
	bool					_afked;
};

#endif
