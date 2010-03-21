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

#include "ChatChannelManager.h"
#include "LogHandler.h"
#include "ChatChannel.h"

// declare static member
ZCom_ClassID ChatChannelManager::m_classid = ZCom_Invalid_ID;



/************************************************************************/
/* register this class                                        
/************************************************************************/
void ChatChannelManager::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("ChatChannelManager");
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
ChatChannelManager::ChatChannelManager(ZCom_Control *_control)
: _controler(_control), _intialized(false)
{
	_control->ZCom_registerDynamicNode( m_node, m_classid );
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName());
	#endif

	m_node->setPrivate(true);
	m_node->setEventNotification( true, true );
}



/************************************************************************/
/* handle init event                                       
/************************************************************************/
void ChatChannelManager::HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
										 unsigned int eventconnid)
{
	_intialized = true;
}

/************************************************************************/
/* handle init event                                       
/************************************************************************/
void ChatChannelManager::HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
											 unsigned int eventconnid)
{
}



/************************************************************************/
/* handle user event                                          
/************************************************************************/
void ChatChannelManager::HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
											unsigned int eventconnid)
{
	// only care about event coming from owner node
	if(remoterole != eZCom_RoleOwner)
		return;

	// type of custom event is in the first 2 bits of the event
	int etype = (int) data->getInt(2);
	switch(etype)
	{
		//subscribe event
		case 0:
		{
			// get channel to subscribe
			char buf[255];
			data->getString(buf, 255);

			// get sender name
			char name[255];
			data->getString(name, 255);

			#ifdef _DEBUG
				std::stringstream strs;
				strs<<"Client "<<eventconnid<<" with name "<<name<<" subscribing to "<<buf;
				LogHandler::getInstance()->LogToFile(strs.str());
			#endif

			//subscribe
			ChatChannel *  channel = GetOrAddChannel(buf);
			channel->Subscribe(eventconnid, name);
		}
		break;

		//unsubscribe event
		case 1:
		{
			// get channel to unsubscribe
			char buf[255];
			data->getString(buf, 255);


			#ifdef _DEBUG
				std::stringstream strs;
				strs<<"Client "<<eventconnid<<" unsubscribing to "<<buf;
				LogHandler::getInstance()->LogToFile(strs.str());
			#endif

			//unsubscribe
			ChatChannel *  channel = GetOrAddChannel(buf);
			channel->Unsubscribe(eventconnid);
		}
		break;
	}
}




/************************************************************************/
/* return pointer to channel
/* return NULL if does not exist                             
/************************************************************************/
ChatChannel * ChatChannelManager::GetChannel(const std::string &name)
{
	// if channel already exist then just return it
	std::map<std::string, ChatChannel *>::iterator it = _channels.find(name);
	if(it != _channels.end())
		return it->second;

	return NULL;
}


/************************************************************************/
/* add channel
/* return pointer to channel                               
/************************************************************************/
ChatChannel * ChatChannelManager::GetOrAddChannel(const std::string &name)
{
	// if channel already exist then just return it
	std::map<std::string, ChatChannel *>::iterator it = _channels.find(name);
	if(it != _channels.end())
		return it->second;

	ChatChannel * res = new ChatChannel(_controler, name);
	_channels[name] = res;
	return res;
}




/************************************************************************/
/* remove channel                        
/************************************************************************/
void ChatChannelManager::RemoveChannel(const std::string &name)
{
	std::map<std::string, ChatChannel *>::iterator it = _channels.find(name);
	if(it != _channels.end())
	{
		 delete it->second;
		_channels.erase(it);
	}
}




/************************************************************************/
/* do a custom process step if required                  
/************************************************************************/
void ChatChannelManager::CustomProcess()
{
	std::map<std::string, ChatChannel *>::iterator it = _channels.begin();
	while(it != _channels.end())
	{
		it->second->Process();
		if(it->second->isGarbage())
		{
			delete it->second;
			it = _channels.erase(it);
		}
		else
			++it;
	}
}




/************************************************************************/
/* used by client to subscribe channel          
/************************************************************************/
void ChatChannelManager::SubscribeChannelToServer(const std::string &name, const std::string &sendername)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(0, 2);
	evt->addString(name.c_str());
	evt->addString(sendername.c_str());
	m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}

/************************************************************************/
/* used by client to unsubscribe channel              
/************************************************************************/
void ChatChannelManager::UnsubscribeChannelToServer(const std::string &name)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(1, 2);
	evt->addString(name.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* handle init event                                       
/************************************************************************/
void ChatChannelManager::ClientDisconnected(unsigned int clientid)
{
	// unsubscribe client from all channels
	std::map<std::string, ChatChannel *>::iterator it = _channels.begin();
	for(;it != _channels.end(); ++it)
		it->second->Unsubscribe(clientid);
}
