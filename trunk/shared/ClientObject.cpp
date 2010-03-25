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
								ClientListHandlerBase* clH, ChatSubscriberBase* WorldSubscriber,
								DatabaseHandlerBase * dbH, long playerdbid)
: m_id(id), m_name(name), m_status(status), m_namecolor(namecolor), m_clH(clH),
	m_WorldSubscriber(WorldSubscriber), m_dbH(dbH), m_playerdbid(playerdbid)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
			std::stringstream strs;
			strs<<"New Node "<<GetObjectName()<<" of id "<<m_id;
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
	#endif

	//reset announcement data
	ResetAnnouncement();


	if(m_clH)
		m_clH->Connected(m_id, m_name, m_status, m_namecolor);
}




/************************************************************************/
/* used to reset announcement data                                    
/************************************************************************/
void ClientObject::ResetAnnouncement()
{
	ZCom_BitStream *adata = new ZCom_BitStream();
	adata->addInt(m_id, 32);
	adata->addString(m_name.c_str());
	adata->addString(m_status.c_str());
	adata->addString(m_namecolor.c_str());
	m_node->setAnnounceData(adata);
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
	unsigned int etype = data->getInt(3);
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

				//reset announcement data
				ResetAnnouncement();

				//send to all
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(0, 3);
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

				//reset announcement data
				ResetAnnouncement();

				//send to all
				ZCom_BitStream *evt = new ZCom_BitStream();
				evt->addInt(1, 3);
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
				evt->addInt(2, 3);
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


		//add friend event
		case 3:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleOwner)
			{
				// get friend name
				char buf[255];
				data->getString(buf, 255);

				if(m_dbH)
					m_dbH->AddFriend(m_playerdbid, buf);
			}

		}
		break;


		//remove friend event
		case 4:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleOwner)
			{
				// get friend name
				char buf[255];
				data->getString(buf, 255);

				if(m_dbH)
					m_dbH->RemoveFriend(m_playerdbid, buf);
			}

		}


		//friends list request
		case 5:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleOwner)
			{
				if(m_dbH)
				{
					std::vector<std::string> friends = m_dbH->GetFriends(m_playerdbid);
					//send message to client for each friends
					for(size_t i=0; i<friends.size(); ++i)
					{
						ZCom_BitStream *evt = new ZCom_BitStream();
						evt->addInt(6, 3);
						evt->addString(friends[i].c_str());
						m_node->sendEventDirect(eZCom_ReliableUnordered, evt, eventconnid);
					}
				}
			}
		}


		//received friend name from server
		case 6:
		{
			// if coming from owner to server
			if(remoterole == eZCom_RoleAuthority)
			{
				// get friend name
				char buf[255];
				data->getString(buf, 255);
				m_clH->NewFriend(buf);
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
	evt->addInt(0, 3);
	evt->addString(status.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}


/************************************************************************/
/* change player color                               
/************************************************************************/
void ClientObject::ChangeColor(const std::string & color)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(1, 3);
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
		evt->addInt(2, 3);
		evt->addInt(1, 1);
		evt->addInt(id, 32);
		evt->addString(Textpart.c_str());
		m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);	
	}
	
	// send last part of the text
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(2, 3);
	evt->addInt(0, 1);
	evt->addInt(id, 32);
	evt->addString(text.c_str());
	m_node->sendEvent(eZCom_ReliableOrdered, ZCOM_REPRULE_OWNER_2_AUTH, evt);


	return true;
}


/************************************************************************/
/* add friend to friend list                                      
/************************************************************************/
void ClientObject::AddFriend(const std::string & name)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(3, 3);
	evt->addString(name.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}

/************************************************************************/
/* remove friend from friend list                                    
/************************************************************************/
void ClientObject::RemoveFriend(const std::string & name)
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(4, 3);
	evt->addString(name.c_str());
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}

/************************************************************************/
/* ask server for friend list                                  
/************************************************************************/
void ClientObject::GetFriendList()
{
	ZCom_BitStream *evt = new ZCom_BitStream();
	evt->addInt(5, 3);
	m_node->sendEvent(eZCom_ReliableUnordered, ZCOM_REPRULE_OWNER_2_AUTH, evt);
}