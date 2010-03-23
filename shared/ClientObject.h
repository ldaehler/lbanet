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

#ifndef _LBA_NET_CLIENT_OBJECT_H_
#define _LBA_NET_CLIENT_OBJECT_H_

#include "GameObject.h"
#include "ClientListHandlerBase.h"
#include "ChatSubscriberBase.h"

#include <boost/shared_ptr.hpp>
#include <map>

class ZCom_Control;


/***********************************************************************
 * Module:  ClientObject.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class representing a connecting client
 ***********************************************************************/
class ClientObject : public GameObject
{

public:
	// constructor
	ClientObject(ZCom_Control *_control, unsigned int id, const std::string & name,
					const std::string & status, const std::string & namecolor,
					ClientListHandlerBase* clH, ChatSubscriberBase* WorldSubscriber);

	// destructor
	virtual ~ClientObject();


	// class registration
	static void registerClass(ZCom_Control *_control);
	static unsigned int getClassID() { return m_classid; }


	//! change player status
	void ChangeStatus(const std::string & status);

	//! change player color
	void ChangeColor(const std::string & color);

	//! whisper to someone 
	bool Whisper(const std::string & playername, std::string text);


protected:
	// return the object name
	virtual std::string GetObjectName()
	{return "ClientObject";}

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid);

	// do a custom process step if required
	virtual void CustomProcess(){}

	//used to reset announcement data
	void ResetAnnouncement();

private:
	// the class id for zoidcom
	static unsigned int		m_classid;

	unsigned int			m_id;
	std::string				m_name;
	std::string				m_status;
	std::string				m_namecolor;

	std::map<unsigned int, std::string>	_tmpstring;

	// client list handler
	ClientListHandlerBase* m_clH;
	ChatSubscriberBase* m_WorldSubscriber;
};


#endif