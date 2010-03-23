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

#include "chatclient.h"
#include "LogHandler.h"
#include "ChatChannelManager.h"
#include "ChatChannel.h"
#include "ClientObject.h"
#include "LbaNetEngine.h"
#include "InternalWorkpile.h"

/***********************************************************************
 * Constructor
 ***********************************************************************/
ChatClient::ChatClient(ChatSubscriberBase* WorldSubscriber, ClientListHandlerBase* clH,
							unsigned short downpacketpersecond, unsigned short downbyteperpacket)
: m_id(ZCom_Invalid_ID), m_connected(false), m_WorldSubscriber(WorldSubscriber),
	m_subscribed_world(false), m_clH(clH), 
	m_downpacketpersecond(downpacketpersecond), m_downbyteperpacket(downbyteperpacket),
	_engine(NULL), _afked(false), _refresh_counter(0), _afk_counter(0)
{
	// this will allocate the sockets and create local bindings
    if ( !ZCom_initSockets( eZCom_EnableUDP, 0, 0, 0 ) )
    {
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
    }

    // string shown in log output
    ZCom_setDebugName("ZCOM_CLI");


	//register classes
	ChatChannelManager::registerClass(this);
	ChatChannel::registerClass(this);
	ClientObject::registerClass(this);
}


/***********************************************************************
 * Destructor
 ***********************************************************************/
ChatClient::~ChatClient()
{

}



/***********************************************************************
 * connect to a server given an address + port
 ***********************************************************************/
void ChatClient::ConnectToServer(const std::string & address, const std::string & login, 
								 const std::string & password, const std::string & excpectedversion,
								 LbaNetEngine * engine)
{
	_engine = engine;

	if(m_connected)
	{
		LogHandler::getInstance()->LogToFile("Zoid: Already connected to chat server - skipping connection", 2);
		return;
	}


	// clear the online list
	InternalWorkpile::JoinEvent evcl;
	evcl.ListName = "online";
	evcl.Clear = true;
	InternalWorkpile::getInstance()->HappenedJoinEvent(evcl);


	// create target address 
	ZCom_Address dst_udp;
	dst_udp.setAddress( eZCom_AddressUDP, 0, address.c_str() );

	//prepare login info
    ZCom_BitStream *req = new ZCom_BitStream();
	req->addString( login.c_str() );
	req->addString( password.c_str() );
	req->addString( excpectedversion.c_str() );

	// connect to server
	LogHandler::getInstance()->LogToFile("Chat client connecting to "+address, 2);
	if (!ZCom_Connect( dst_udp, req))
	{
		LogHandler::getInstance()->LogToFile("Chat client: unable to start connecting!", 2);
	}
}

/***********************************************************************
 * called when initiated connection process yields a result
 ***********************************************************************/
void ChatClient::ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply )
{
	std::string reason(_reply.getStringStatic());

	std::stringstream strs;
	strs<<"Client: The connection process for: "<<_id<<" returned with resultcode "<<_result<<", the reply was "<<reason;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	if ( _result != eZCom_ConnAccepted )
	{
		m_connected = false;
		int errorcode = -1;
		if(_result == eZCom_ConnTimeout)
			errorcode = 0;
		_engine->ConnectionCallback(errorcode, reason);
	}
	else
	{
		m_connected = true;
		ZCom_requestDownstreamLimit(_id, m_downpacketpersecond, m_downbyteperpacket);
		ZCom_changeObjectChannelSubscription( _id, 1, eZCom_Subscribe );
		m_id = _id;
		_engine->ConnectionCallback(1, "");
	}
}


/***********************************************************************
 * connection has closed
 ***********************************************************************/
void ChatClient::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, 
									 ZCom_BitStream &_reasondata )
{
	std::stringstream strs;
	strs<<"Client: Connection with ID: "<<_id<<" has been closed, reason is: "<<_reasondata.getStringStatic();
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	m_connected = false;
	_engine->ConnectionCallback(-2, "");
}


/***********************************************************************
 * zoidlevel transition finished
 ***********************************************************************/
void ChatClient::ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason )
{
	// disconnect on failure
	if (_result == eZCom_ChannelSubscriptionDenied || _result == eZCom_ChannelSubscriptionFailed_Node)
	{
		std::stringstream strs;
		strs<<"Client: Connection with ID: "<<_id<<" channel subscription failed: "
				<<_new_channel<<" - disconnecting";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   

		ZCom_Disconnect(_id, NULL);
	}
	else
	{
		std::stringstream strs;
		strs<<"Client: Connection with ID: "<<_id<<" channel subscription successfull: "<<_new_channel;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	}
}


/***********************************************************************
 * server wants to tell us about new node
 ***********************************************************************/
void ChatClient::ZCom_cbNodeRequest_Dynamic(ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, 
											ZCom_NodeID _net_id)
{
	// if this is the channel handler
	if(_requested_class == ChatChannelManager::getClassID())
	{
		m_channelM = boost::shared_ptr<ChatChannelManager>(new ChatChannelManager(this, m_clH));
	}


	// if this is the channel 
	if(_requested_class == ChatChannel::getClassID())
	{
		if(m_channelM)
		{
			// get channel name and create the channel
			char _buf[255];
			_announcedata->getString( _buf, 255 );
			ChatChannel * chn = m_channelM->GetOrAddChannel(_buf);

			// attach the subscriber
			std::map<std::string, ChatSubscriberBase* >::iterator it = m_waitingsubs.find(_buf);
			if(it != m_waitingsubs.end())
			{
				chn->AttachSubscriber(it->second);
				m_waitingsubs.erase(it);
			}
		}
	}


	// if this is the client 
	if(_requested_class == ClientObject::getClassID())
	{
		unsigned int clid = _announcedata->getInt( 32 );
		char _buf[255];
		_announcedata->getString( _buf, 255 );
		char _status[255];
		_announcedata->getString( _status, 255 );
		char _color[255];
		_announcedata->getString( _color, 255 );

		m_clientHandler.Addclient(clid, new ClientObject(this, clid, _buf, _status, _color, m_clH));
	}
	
}




/***********************************************************************
 * subscribe to channel 'name'
 ***********************************************************************/
void ChatClient::SubscribeChannel(const std::string & channelname, ChatSubscriberBase* Subscriber)
{
	if(m_channelM)
	{
		// do nothing if already subscribed
		if(m_channelM->GetChannel(channelname) != NULL)
			return;

		m_channelM->SubscribeChannelToServer(channelname);
		m_waitingsubs[channelname] = Subscriber;
	}
}

/***********************************************************************
 * unsubscribe to channel 'name'
 ***********************************************************************/
void ChatClient::UnsubscribeChannel(const std::string & channelname)
{
	if(m_channelM)
	{
		// do nothing if not already subscribed
		if(m_channelM->GetChannel(channelname) == NULL)
			return;

		m_channelM->UnsubscribeChannelToServer(channelname);
	}
}

/***********************************************************************
 * send text to a specific channel
 ***********************************************************************/
void ChatClient::SendText(const std::string & channelname, const std::string & text)
{
	if(m_channelM)
	{
		ChatChannel * chan = m_channelM->GetChannel(channelname);

		// do nothing if not already subscribed to this channel
		if(chan == NULL)
			return;

		chan->SendText(text);
	}
}



/***********************************************************
process server internal stuff
***********************************************************/
void ChatClient::Process()
{
	//process text to send
	HandleChatText();


	//process chat
	if(m_channelM)
	{
		m_channelM->Process();

		// subscribe to world channel at startup
		if(!m_subscribed_world)
		{
			if(m_channelM->IsInitialized())
			{
				SubscribeChannel("World", m_WorldSubscriber);
				m_subscribed_world = true;
			}
		}
	}

	//process client objects
	m_clientHandler.Process();
}


/***********************************************************
close connection
***********************************************************/
void ChatClient::CloseConnection()
{
	if(!m_connected)
		return;

	ZCom_Disconnect( m_id, NULL );

	// process until everything has been sent
	while(m_connected)
	{
		ZCom_processInput( eZCom_NoBlock );
		ZCom_processOutput();
		ZoidCom::Sleep(10);
	}
}



/***********************************************************
SendText
***********************************************************/
void ChatClient::HandleChatText()
{
	std::vector<std::string> texts;
	InternalWorkpile::getInstance()->GetChatText(texts);

	try
	{
		if(texts.size() > 0)
		{
			_refresh_counter = 0;
			_afk_counter = 0;
			if(_afked)
			{
				_afked = false;
				//_connectionMananger.ChangeStatus("");
			}
		}

		std::vector<std::string>::const_iterator it = texts.begin();
		std::vector<std::string>::const_iterator end = texts.end();
		for(;it != end; ++it)
		{
			ProcessText(*it);
		}
	}
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception sending text to chat"));
	}
}


/***********************************************************
process a line of text
***********************************************************/
void ChatClient::ProcessText(const std::string & Text)
{
	// return if text is empty
	if(Text == "")
		return;

	// if it is a command - preproccess it
	if(Text[0] == '/')
	{
		std::vector<std::string> tok;
		std::stringstream ss(Text);
		std::string buf;
		while(ss >> buf)
		{
			tok.push_back(buf);
		}

		if(tok.size() > 0)
		{
			if(tok[0] == "/join")
			{
				if(tok.size() != 2)
				{
					InternalWorkpile::getInstance()->ReceivedText("All", "info", "You need to specify a room to join.");
					return;
				}

				std::string roomName = tok[1];
				if(m_channelM->GetChannel(roomName) != NULL)
				{
					InternalWorkpile::getInstance()->ReceivedText("All", "info", "You already joined channel "+roomName);
					return;
				}

				SubscribeChannel(roomName, m_WorldSubscriber);
				return;
			}

			if(tok[0] == "/leave")
			{
				if(tok.size() != 2)
				{
					InternalWorkpile::getInstance()->ReceivedText("All", "info", "You need to specify a room to leave.");
					return;
				}

				std::string roomName = tok[1];
				UnsubscribeChannel(roomName);
				return;
			}

			if(tok[0] == "/w")
			{
				if(tok.size() < 3)
				{
					InternalWorkpile::getInstance()->ReceivedText("All", "info", "Incorrect command.");
					return;
				}

				std::string playername = tok[1];
				std::string message;
				for(size_t i=2; i<tok.size(); ++i)
					message += tok[i] + " ";

				//if(!_connectionMananger.Whisper(playername, message))
				//{
				//	InternalWorkpile::getInstance()->ReceivedText("All", "info", "The player " + playername + " is not available.");
				//}
				//else
				//{
				//	InternalWorkpile::getInstance()->ReceivedText("All", "to " + playername, message);
				//	InternalWorkpile::getInstance()->AddWhisperChannel(playername);
				//}
				return;
			}

			if(tok[0] == "/afk")
			{
				//_connectionMananger.ChangeStatus("away");
				return;
			}

			if(tok[0] == "/back")
			{
				//_connectionMananger.ChangeStatus("");
				return;
			}

			// send text to a specific channel
			std::string roomName = tok[0].substr(1);
			if(m_channelM->GetChannel(roomName))
			{
				SendText(roomName, Text.substr(Text.find_first_of(' ')+1));
				return;
			}
			else
			{
				InternalWorkpile::getInstance()->ReceivedText("All", "info", "Invalid command: " + tok[0]);
				return;
			}
		}
	}
}

