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

#include "GameServerHandler.h"
#include "LogHandler.h"

// declare static member
ZCom_ClassID GameServerHandler::m_classid = ZCom_Invalid_ID;




/************************************************************************/
/* register this class                                        
/************************************************************************/
void GameServerHandler::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("GameServerHandler");
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
GameServerHandler::GameServerHandler(ZCom_Control *_control, GameServerCallbackBase* callbH)
: m_callbH(callbH)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
			std::stringstream strs;
			strs<<"New Node "<<GetObjectName();
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
	#endif

	m_node->setPrivate(true);
}



/************************************************************************/
/* destructor                                        
/************************************************************************/
GameServerHandler::~GameServerHandler()
{
	#ifdef _DEBUG
		std::stringstream strs;
		strs<<"Deleted Node "<<GetObjectName();
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	#endif
}


/************************************************************************/
/* handle user event                                     
/************************************************************************/
void GameServerHandler::HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid)
{

	// type of custom event is in the first 3 bits of the event
	unsigned int etype = data->getInt(3);
	switch(etype)
	{
		//game server register
		case 0:
		{
			// get server name
			char buf[255];
			data->getString(buf, 255);

			// get server address
			char bufad[255];
			data->getString(bufad, 255);

			// add it to the list
			m_servers[buf] = bufad;

			std::stringstream strs;
			strs<<"New game server "<<buf<<" at address "<<bufad;
			LogHandler::getInstance()->LogToFile(strs.str(), 2);   

			//send acknowledgment
			ZCom_BitStream *evt = new ZCom_BitStream();
			evt->addInt(4, 3);
			evt->addBool(true);
			m_node->sendEventDirect(eZCom_ReliableUnordered, evt, eventconnid);
		}
		break;

		//game server unregister
		case 1:
		{
			// get server name
			char buf[255];
			data->getString(buf, 255);

			//remove it from list
			std::map<std::string, std::string>::iterator it = m_servers.find(buf);
			if(it != m_servers.end())
				m_servers.erase(it);


			std::stringstream strs;
			strs<<"Game server "<<buf<<" quitted";
			LogHandler::getInstance()->LogToFile(strs.str(), 2);   

			//send acknowledgment
			ZCom_BitStream *evt = new ZCom_BitStream();
			evt->addInt(4, 3);
			evt->addBool(true);
			m_node->sendEventDirect(eZCom_ReliableUnordered, evt, eventconnid);
		}
		break;

		//client ask for address
		case 2:
		{
			// get server name
			char buf[255];
			data->getString(buf, 255);

			std::string reply;
			std::map<std::string, std::string>::iterator it = m_servers.find(buf);
			if(it != m_servers.end())
				reply = it->second;

			//send address to client
			ZCom_BitStream *evt = new ZCom_BitStream();
			evt->addInt(3, 3);
			evt->addString(buf);
			evt->addString(reply.c_str());
			m_node->sendEventDirect(eZCom_ReliableUnordered, evt, eventconnid);
		}
		break;


		//client receive address from server
		case 3:
		{
			// get server name
			char buf[255];
			data->getString(buf, 255);

			// get server address
			char bufad[255];
			data->getString(bufad, 255);

			//forward reply to the client
			m_callbH->ReceivedAddress(buf, bufad);
		}
		break;


		//acknowledgment from the server after reg/dereg
		case 4:
		{
			bool ok = data->getBool();

			//forward aknowledgment to the server
			m_callbH->Aknowldeged(ok);
		}
		break;
	}
}



/************************************************************************/
/* register a game server                                    
/************************************************************************/
void GameServerHandler::RegisterGameServer(const std::string &Name, const std::string &Adress)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(0, 3);
	evt->addString(Name.c_str());
	evt->addString(Adress.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* unregister a game server                                    
/************************************************************************/
void GameServerHandler::UnregisterGameServer(const std::string &Name)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(1, 3);
	evt->addString(Name.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* called by client to get game server address                                     
/************************************************************************/
void GameServerHandler::GetGameServerAddress(const std::string &Name)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(2, 3);
	evt->addString(Name.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}