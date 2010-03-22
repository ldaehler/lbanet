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

#include <zoidcom.h>

#include "ChatChannel.h"
#include "LogHandler.h"

// declare static member
ZCom_ClassID ChatChannel::m_classid = ZCom_Invalid_ID;



/************************************************************************/
/* register this class                                        
/************************************************************************/
void ChatChannel::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("ChatChannel", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
ChatChannel::ChatChannel(ZCom_Control *_control, const std::string & name)
: _name(name)
{
	_control->ZCom_registerDynamicNode( m_node, m_classid );
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName() + " of name " + _name);
	#endif

	m_node->setPrivate(true);

	ZCom_BitStream *adata = new ZCom_BitStream();
	adata->addString(_name.c_str());
	m_node->setAnnounceData(adata);
}


/************************************************************************/
/* destructor                                        
/************************************************************************/
ChatChannel::~ChatChannel()
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Deleting node " + GetObjectName() + " of name " + _name);
	#endif
}

/************************************************************************/
/* handle user event                                          
/************************************************************************/
void ChatChannel::HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
										 unsigned int eventconnid)
{
	// type of custom event is in the first 2 bits of the event
	int etype = (int) data->getInt(2);
	switch(etype)
	{
		//text event from client
		case 0:
		{
			// get text
			unsigned short sizes = data->getStringLength();
			char *buf = new char[sizes];
			data->getString(buf, sizes);

			#ifdef _DEBUG
				std::stringstream strs;
				strs<<"Machine "<<eventconnid<<" sent "<<buf;
				LogHandler::getInstance()->LogToFile(strs.str());
			#endif
			
			// only care if event comes from the owner
			if(remoterole == eZCom_RoleOwner)
			{
				//broadcast the text to all connected client
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(1, 2);

				// add text
				evt->addString(buf);

				// add sender name
				evt->addString(_subscribedclients[eventconnid].c_str());

				m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_AUTH_2_OWNER, evt);
			}

			delete buf;
		}
		break;

		//text event from server
		case 1:
		{
			// get text
			unsigned short sizes = data->getStringLength();
			char *buf = new char[sizes];
			data->getString(buf, sizes);

			// get client name
			unsigned short sizen = data->getStringLength();
			char *bufname = new char[sizen];
			data->getString(bufname, sizen);

			#ifdef _DEBUG
				std::stringstream strs;
				strs<<"Server "<<eventconnid<<" sent "<<buf<<" from "<<bufname;
				LogHandler::getInstance()->LogToFile(strs.str());
			#endif
			
			// only care if event comes from the server
			if(remoterole == eZCom_RoleAuthority)
			{
				//publish text to all subscribers
				std::vector<boost::shared_ptr<ChatSubscriberBase> >::iterator it = _subscribers.begin();
				std::vector<boost::shared_ptr<ChatSubscriberBase> >::iterator end = _subscribers.end();
				for(; it != end; ++it)
					(*it)->ReceivedText(bufname, buf);
			}

			delete buf;
			delete bufname;
		}
		break;
	}
}



/************************************************************************/
/* subscribe a specific client                                         
/************************************************************************/
void ChatChannel::Subscribe(unsigned int clientId, const std::string &clientName)
{
	_subscribedclients[clientId] = clientName;
	m_node->setOwner(clientId, true);


	// in case we get a subscribe and unsubscribe event at the same time
	m_deleteme = false;
}

/************************************************************************/
/* unsubscribe a specific client                                         
/************************************************************************/
void ChatChannel::Unsubscribe(unsigned int clientId)
{
	std::map<unsigned int, std::string>::iterator it = _subscribedclients.find(clientId);
	if(it != _subscribedclients.end())
	{
		_subscribedclients.erase(it);
		m_node->setOwner(clientId, false);

		// if no client connected then delete the channel
		if(_subscribedclients.size() == 0)
			m_deleteme = true;
	}
}



/************************************************************************/
/* attach a subscriber                                  
/************************************************************************/
void ChatChannel::AttachSubscriber(boost::shared_ptr<ChatSubscriberBase> sub)
{
	_subscribers.push_back(sub);
}


/************************************************************************/
/* called when client want to send text                         
/************************************************************************/
void ChatChannel::SendText(const std::string Text)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(0, 2);
	evt->addString(Text.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}