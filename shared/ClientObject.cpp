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

#include "ClientObject.h"
#include "LogHandler.h"

// declare static member
ZCom_ClassID ClientObject::m_classid = ZCom_Invalid_ID;




/************************************************************************/
/* register this class                                        
/************************************************************************/
void ClientObject::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("ClientObject", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
ClientObject::ClientObject(ZCom_Control *_control, unsigned int id, const std::string & name,
								const std::string & status, const std::string & namecolor,
								ClientListHandlerBase* clH, ChatSubscriberBase* WorldSubscriber)
: m_id(id), m_name(name), m_status(status), m_namecolor(namecolor), m_clH(clH),
	m_WorldSubscriber(WorldSubscriber)
{
	_control->ZCom_registerDynamicNode( m_node, m_classid );
	#ifdef _DEBUG
			std::stringstream strs;
			strs<<"New Node "<<GetObjectName()<<" of id "<<m_id;
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
	#endif


	ZCom_BitStream *adata = new ZCom_BitStream();
	adata->addInt(m_id, 32);
	adata->addString(m_name.c_str());
	adata->addString(m_status.c_str());
	adata->addString(m_namecolor.c_str());
	m_node->setAnnounceData(adata);

	if(m_clH)
		m_clH->Connected(m_id, m_name, m_status, m_namecolor);
}


/************************************************************************/
/* destructor                                        
/************************************************************************/
ClientObject::~ClientObject()
{
	#ifdef _DEBUG
		std::stringstream strs;
		strs<<"Deleted Node "<<GetObjectName()<<" of id "<<m_id;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	#endif


	if(m_clH)
		m_clH->Disconnected(m_id);
}


/************************************************************************/
/* handle user event                                     
/************************************************************************/
void ClientObject::HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid)
{

	// type of custom event is in the first 2 bits of the event
	unsigned int etype = data->getInt(2);
	switch(etype)
	{
		//subscribe event
		case 0:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleOwner)
			{
				// get status
				char buf[255];
				data->getString(buf, 255);
				m_status = buf;

				//send to all
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(0, 2);
				evt->addString(m_status.c_str());
				m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_AUTH_2_ALL, evt);
			}

			// if coming from server to all
			if(remoterole == eZCom_RoleAuthority)
			{
				// get status
				char buf[255];
				data->getString(buf, 255);
				m_status = buf;
				m_clH->ChangedStatus(m_id, m_status, m_namecolor);
			}
		}
		break;

		//unsubscribe event
		case 1:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleOwner)
			{
				// get color
				char buf[255];
				data->getString(buf, 255);
				m_namecolor = buf;

				//send to all
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(1, 2);
				evt->addString(m_namecolor.c_str());
				m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_AUTH_2_ALL, evt);
			}

			// if coming from server to all
			if(remoterole == eZCom_RoleAuthority)
			{
				// get color
				char buf[255];
				data->getString(buf, 255);
				m_namecolor = buf;
				m_clH->ChangedStatus(m_id, m_status, m_namecolor);
			}
		}
		break;

		//text event from client
		case 2:
		{
			// if event comes from the owner client
			if(remoterole == eZCom_RoleOwner)
			{
				// 0 if text is complete - 1 if we expect more
				unsigned int textform = data->getInt(1);

				// check who to send the text to
				unsigned int sendto = data->getInt(32);

				// get text
				unsigned short sizes = data->getStringLength()+1;
				char *buf = new char[sizes];
				data->getString(buf, sizes);

				#ifdef _DEBUG
					std::stringstream strs;
					strs<<"Machine "<<eventconnid<<" sent "<<buf;
					LogHandler::getInstance()->LogToFile(strs.str());
				#endif

				//send to concerned person
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(2, 2);
				evt->addInt(textform, 1);
				evt->addString(buf);
				m_node->sendEventDirect(eZCom_ReliableOrdered, evt, sendto);	

				delete buf;
			}


			// if event comes from the server
			if(remoterole == eZCom_RoleAuthority)
			{
				// 0 if text is complete - 1 if we expect more
				unsigned int textform = data->getInt(1);

				// get text
				unsigned short sizes = data->getStringLength()+1;
				char *buf = new char[sizes];
				data->getString(buf, sizes);

				#ifdef _DEBUG
					std::stringstream strs;
					strs<<"Server "<<eventconnid<<" sent "<<buf<<" from "<<m_clH->GetName(m_id);
					LogHandler::getInstance()->LogToFile(strs.str());
				#endif

				// addtextpart
				_tmpstring[m_id].append(buf);

				// if end of text
				if(textform == 0)
				{
					//publish text to all subscribers
					m_WorldSubscriber->ReceivedText("All", "from " + m_clH->GetName(m_id), _tmpstring[m_id]);

					//clear the string
					_tmpstring[m_id].clear();
				}

				delete buf;
			}		
		}
		break;
	}
}


/************************************************************************/
/* change player status                                     
/************************************************************************/
void ClientObject::ChangeStatus(const std::string & status)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(0, 2);
	evt->addString(status.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* change player color                               
/************************************************************************/
void ClientObject::ChangeColor(const std::string & color)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(1, 2);
	evt->addString(color.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* whisper to someone                                         
/************************************************************************/
bool ClientObject::Whisper(const std::string & playername, std::string text)
{
	unsigned int id = m_clH->GetId(playername);
	if(id == 0)
		return false;


	//send part of the string text
	while(text.length() > _MAX_CHAR_SIZE_)
	{
		std::string Textpart = 	text.substr(0, _MAX_CHAR_SIZE_);
		text = text.substr(_MAX_CHAR_SIZE_);

		ZCom_BitStream *evt = new ZCom_BitStream();
		evt->addInt(2, 2);
		evt->addInt(1, 1);
		evt->addInt(id, 32);
		evt->addString(Textpart.c_str());
		m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);	
	}
	
	// send last part of the text
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(2, 2);
	evt->addInt(0, 1);
	evt->addInt(id, 32);
	evt->addString(text.c_str());
	m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);


	return true;
}