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

#include "MapInfoObject.h"
#include "LogHandler.h"
#include "MapInfo.h"

// declare static member
ZCom_ClassID MapInfoObject::m_classid = ZCom_Invalid_ID;



/************************************************************************/
/* register this class                                        
/************************************************************************/
void MapInfoObject::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("MapInfoObject", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
MapInfoObject::MapInfoObject(ZCom_Control *_control, const MapInfo & minfo)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName() + " of name " + minfo.Name);
	#endif


	ZCom_BitStream *adata = new ZCom_BitStream();
	adata->addString(minfo.Name.c_str());
	adata->addString(minfo.Type.c_str());
	adata->addString(minfo.Music.c_str());
	adata->addSignedInt(minfo.MusicLoop, 16);
	m_node->setAnnounceData(adata);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
MapInfoObject::MapInfoObject(ZCom_Control *_control)
{
	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName());
	#endif
}


/************************************************************************/
/* destructor                                        
/************************************************************************/
MapInfoObject::~MapInfoObject()
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Deleting node " + GetObjectName());
	#endif
}
