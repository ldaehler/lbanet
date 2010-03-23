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
								ClientListHandlerBase* clH)
: m_id(id), m_name(name), m_status(status), m_namecolor(namecolor), m_clH(clH)
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
void ClientObject::Whisper(const std::string & playername, const std::string & text)
{

}