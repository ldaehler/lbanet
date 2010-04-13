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

#include "GameObject.h"
#include "LogHandler.h"


/************************************************************************/
/* constructor - creates and registers the node                                             
/************************************************************************/
GameObject::GameObject() 
: m_node(new ZCom_Node()), m_deleteme(false)
{
}


/************************************************************************/
  // destructor
  // when object get deleted, just delete the node, too
  // there is no need for unregistering or anything else                                          
/************************************************************************/
GameObject::~GameObject() 
{ 
	if (m_node) 
		delete m_node;  
}




/************************************************************************/
/* handle node events                                                  */
/************************************************************************/
void GameObject::Process()
{
	while (m_node->checkEventWaiting()) 
	{
		eZCom_Event       eve_type;        // event type
		eZCom_NodeRole    eve_remote_role; // role of remote sender
		ZCom_ConnID       eve_connid;      // connection id of sender
		unsigned int	  estimated_time_sent;

		// get next waiting event
		ZCom_BitStream *eve_data = m_node->getNextEvent(&eve_type, &eve_remote_role, 
														&eve_connid, &estimated_time_sent);

		// if init event
		if (eve_type == eZCom_EventInit)
			HandleInitEvent(eve_data, eve_remote_role, eve_connid);


		// the server object has been deleted on the server, we can delete it here, too
		if (eve_type == eZCom_EventRemoved)
		{
			if(eve_remote_role == eZCom_RoleAuthority)
			{
				m_deleteme = true;
				#ifdef _DEBUG
					LogHandler::getInstance()->LogToFile("Node " + GetObjectName() + " deleted on the server.");
				#endif
			}
			else
			{
				HandleQuitEvent(eve_data, eve_remote_role, eve_connid);
			}
		}

		// a custom application event arrived
		if (eve_type == eZCom_EventUser)
			HandleUserEvent(eve_data, eve_remote_role, eve_connid, estimated_time_sent);
	}

	// child custom process step
	CustomProcess();
}
