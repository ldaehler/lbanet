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

#include "ActorObject.h"
#include "LogHandler.h"
#include "ObjectsDescription.h"
#include "ZoidSerializer.h"

// declare static member
ZCom_ClassID ActorObject::m_classid = ZCom_Invalid_ID;



/************************************************************************/
/* register this class                                        
/************************************************************************/
void ActorObject::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("ActorObject", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
ActorObject::ActorObject(ZCom_Control *_control, const ObjectInfo & oinfo)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName());
	#endif


	// add announcement data
	ZCom_BitStream *adata = new ZCom_BitStream();
	ZoidSerializer zoids(adata);
	oinfo.Serialize(&zoids);
	m_node->setAnnounceData(adata);
}



/************************************************************************/
/* destructor                                        
/************************************************************************/
ActorObject::~ActorObject()
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Deleting node " + GetObjectName());
	#endif
}
