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
ChatChannel::ChatChannel(ZCom_Control *_control, const std::string & name,
							ClientListHandlerBase* clH)
: _name(name), _clH(clH)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

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
									unsigned int eventconnid, unsigned int _estimated_time_sent)
{
	// type of custom event is in the first 2 bits of the event
	unsigned int etype = data->getInt(2);

	// 0 if text is complete - 1 if we expect more
	unsigned int textform = data->getInt(1);

	switch(etype)
	{
		//text event from client
		case 0:
		{
			// get text
			unsigned short sizes = data->getStringLength()+1;
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
				evt->addInt(textform, 1);
				evt->addInt(eventconnid, 32);

				// add text
				evt->addString(buf);

				m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_AUTH_2_OWNER, evt);
			}

			delete buf;
		}
		break;

		//text event from server
		case 1:
		{
			//get sender id
			unsigned int senderid = data->getInt(32);

			// get text
			unsigned short sizes = data->getStringLength()+1;
			char *buf = new char[sizes];
			data->getString(buf, sizes);
		
			// only care if event comes from the server
			if(remoterole == eZCom_RoleAuthority)
			{
				// addtextpart
				_tmpstring[senderid].append(buf);

				#ifdef _DEBUG
					std::stringstream strs;
					strs<<"Server "<<eventconnid<<" sent "<<buf;
					LogHandler::getInstance()->LogToFile(strs.str());
				#endif

				// if end of text
				if(textform == 0)
				{
					//publish text to all subscribers
					std::vector<ChatSubscriberBase*>::iterator it = _subscribers.begin();
					std::vector<ChatSubscriberBase*>::iterator end = _subscribers.end();
					for(; it != end; ++it)
					{
						std::string name;
						if(_clH)
							name = _clH->GetName(senderid);
						(*it)->ReceivedText(_name, name, _tmpstring[senderid]);
					}

					//clear the string
					_tmpstring[senderid].clear();
				}
			}

			delete buf;

		}
		break;
	}
}



/************************************************************************/
/* subscribe a specific client                                         
/************************************************************************/
void ChatChannel::Subscribe(unsigned int clientId)
{
	_subscribedclients[clientId] = "";
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
void ChatChannel::AttachSubscriber(ChatSubscriberBase* sub)
{
	_subscribers.push_back(sub);
}


/************************************************************************/
/* called when client want to send text                         
/************************************************************************/
void ChatChannel::SendText(std::string Text)
{
	//send part of the string text
	while(Text.length() > _MAX_CHAR_SIZE_)
	{
		std::string Textpart = 	Text.substr(0, _MAX_CHAR_SIZE_);
		Text = Text.substr(_MAX_CHAR_SIZE_);

		ZCom_BitStream *evt = new ZCom_BitStream();
		evt->addInt(0, 2);
		evt->addInt(1, 1);
		evt->addString(Textpart.c_str());
		m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);	
	}
	
	// send last part of the text
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(0, 2);
	evt->addInt(0, 1);
	evt->addString(Text.c_str());
	m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}