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

#ifndef _LBA_NET_MAP_INFO_OBJECT_H_
#define _LBA_NET_MAP_INFO_OBJECT_H_

#include "GameObject.h"

class ZCom_Control;
class MapInfo;


/***********************************************************************
 * Module:  MapInfoObject.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class MapInfoObject
 ***********************************************************************/
class MapInfoObject : public GameObject
{

public:
	// constructor for server
	MapInfoObject(ZCom_Control *_control, const MapInfo & minfo);

	// constructor for client
	MapInfoObject(ZCom_Control *_control);

	// destructor
	virtual ~MapInfoObject();


	// class registration
	static void registerClass(ZCom_Control *_control);
	static unsigned int getClassID() { return m_classid; }

protected:
	// return the object name
	virtual std::string GetObjectName()
	{return "MapInfoObject";}

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// do a custom process step if required
	virtual void CustomProcess(){}

private:
	// the class id for zoidcom
	static unsigned int  m_classid;

};


#endif