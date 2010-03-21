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

#ifndef _LBA_NET_CHAT_CHANNEL_MGT_H_
#define _LBA_NET_CHAT_CHANNEL_MGT_H_

#include "GameObject.h"
#include <map>

class ChatChannel;

/***********************************************************************
 * Module:  ChatChannelManager.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class ChatChannelManager
 ***********************************************************************/
class ChatChannelManager : public GameObject
{

public:
	// constructor
	ChatChannelManager(ZCom_Control *_control);

	// destructor
	virtual ~ChatChannelManager(){}


	// class registration
	static void registerClass(ZCom_Control *_control);
	static ZCom_ClassID getClassID() { return m_classid; }


	// return pointer to channel
	// return NULL if does not exist
	ChatChannel * GetChannel(const std::string &name);

	// add channel
	// return pointer to channel
	ChatChannel * GetOrAddChannel(const std::string &name);

	// remove channel
	void RemoveChannel(const std::string &name);

	// used by client to subscribe channel
	void SubscribeChannelToServer(const std::string &name, const std::string &sendername);

	// used by client to unsubscribe channel
	void UnsubscribeChannelToServer(const std::string &name);

	// check if class is initialized
	bool IsInitialized()
	{ return _intialized;}


	//called when a client has disconnected
	void ClientDisconnected(unsigned int clientid);

protected:
	// return the object name
	virtual std::string GetObjectName()
	{return "ChatChannelManager";}

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid);

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid);

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid);

	// do a custom process step if required
	virtual void CustomProcess();

private:
	// the class id for zoidcom
	static ZCom_ClassID  m_classid;

	// controller
	ZCom_Control *	_controler;

	// channel map
	std::map<std::string, ChatChannel *> _channels;


	bool	_intialized;
};

#endif