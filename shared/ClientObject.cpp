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
		m_clH->Connected(id, name);
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
