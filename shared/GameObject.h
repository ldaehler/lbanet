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
#ifndef _LBA_NET_GAME_OBJECT_H_
#define _LBA_NET_GAME_OBJECT_H_

class ZCom_Node;
class ZCom_BitStream;
enum eZCom_NodeRole;


#define _MAX_CHAR_SIZE_ 50

//#define _ZOID_USED_NEW_VERSION_


#ifndef _ZOID_USED_NEW_VERSION_
#define eZCom_RequestResult bool
#define eZCom_AcceptRequest true
#define eZCom_DenyRequest false
#define eZCom_EnableUDP true
#define eZCom_RequestedChannelSubscribed eZCom_ZoidEnabled
#define eZCom_AutomaticChannelSubscribed eZCom_ZoidEnabled
#define eZCom_ChannelSubscriptionDenied eZCom_ZoidDenied
#define eZCom_ChannelSubscriptionFailed_Node eZCom_ZoidFailed_Node
#endif _ZOID_USED_NEW_VERSION_


#include <string>

/***********************************************************************
 * Module:  GameObject.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: base game object for replication over network
 ***********************************************************************/
class GameObject
{

public:
	// constructor - creates and registers the node
	GameObject();

	// destructor
	// when object get deleted, just delete the node, too
	// there is no need for unregistering or anything else
	virtual ~GameObject();

	// handle node events
	// checks for eZCom_EventRemoved on client and sets m_deleteme
	void Process();

	// returns true if object is garbage
	bool isGarbage() { return m_deleteme; }

	// return pointer to the node
	ZCom_Node* GetNode()
	{return m_node;}

protected:
	// return the object name
	virtual std::string GetObjectName() = 0;

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid) = 0;

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid) = 0;

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
									unsigned int eventconnid, unsigned int _estimated_time_sent) = 0;

	// do a custom process step if required
	virtual void CustomProcess() = 0;

protected:
	// the node
	ZCom_Node*           m_node;

	// indicates if object is garbage
	bool                 m_deleteme;
};

#endif