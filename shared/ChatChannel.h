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

#ifndef _LBA_NET_CHAT_CHANNEL_H_
#define _LBA_NET_CHAT_CHANNEL_H_

#include "GameObject.h"
#include "ChatSubscriberBase.h"
#include "ClientListHandlerBase.h"

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>



/***********************************************************************
 * Module:  ChatChannel.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class ChatChannel
 ***********************************************************************/
class ChatChannel : public GameObject
{

public:
	// constructor
	ChatChannel(ZCom_Control *_control, const std::string & name,
				ClientListHandlerBase* clH);

	// destructor
	virtual ~ChatChannel();


	// class registration
	static void registerClass(ZCom_Control *_control);
	static ZCom_ClassID getClassID() { return m_classid; }


	//subscribe a specific client
	void Subscribe(unsigned int clientId);

	//unsubscribe a specific client
	void Unsubscribe(unsigned int clientId);

	// attach a subscriber
	void AttachSubscriber(ChatSubscriberBase* sub);

	// called when client want to send text
	void SendText(std::string Text);

protected:
	// return the object name
	virtual std::string GetObjectName()
	{return "ChatChannel";}

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
									unsigned int eventconnid, unsigned int _estimated_time_sent);

	// do a custom process step if required
	virtual void CustomProcess(){}

private:
	// the class id for zoidcom
	static ZCom_ClassID  m_classid;

	// list of subscribed clients
	std::map<unsigned int, std::string> _subscribedclients;

	// channel name
	std::string _name;

	std::map<unsigned int, std::string>			_tmpstring;
	std::vector<ChatSubscriberBase*>			_subscribers;

	ClientListHandlerBase*						_clH;
};


#endif