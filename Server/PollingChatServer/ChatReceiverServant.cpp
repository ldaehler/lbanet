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

#include "ChatReceiverServant.h"
#include <algorithm>

/***********************************************************
callback function called when a message is received from IceStorm
***********************************************************/
void ChatReceiverServant::Message(const std::string& Sender, const std::string& Text, const Ice::Current&)
{
	if(Sender == "info")
	{
		if(Text.substr(0, 7) == "#joined")
		{
			LbaNet::UserJoinedEventPtr ev = new LbaNet::UserJoinedEvent();
			ev->timestamp = IceUtil::Time::now().toMilliSeconds();
			ev->name = Text.substr(8);
			Publish(ev);
			return;
		}

		if(Text.substr(0, 5) == "#left")
		{
			LbaNet::UserLeftEventPtr ev = new LbaNet::UserLeftEvent();
			ev->timestamp = IceUtil::Time::now().toMilliSeconds();
			ev->name = Text.substr(6);
			Publish(ev);
			return;
		}

	//	if(Text.substr(0, 7) == "#status")
	//	{
	//		ThreadSafeWorkpile::JoinEvent ev;
	//		ev.ListName = "online";
	//		ev.Joined = true;
	//		ev.Clear = false;
	//		std::string tmptxt = Text.substr(8);
	//		ev.Nickname = tmptxt.substr(0, tmptxt.find(" "));
	//		tmptxt = tmptxt.substr(tmptxt.find(" ")+1);
	//		ev.Status = tmptxt.substr(0, tmptxt.find(" "));
	//		tmptxt = tmptxt.substr(tmptxt.find(" ")+1);
	//		ev.Color = tmptxt;
	//		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
	//		ThreadSafeWorkpile::getInstance()->ChatColorChanged(ev.Nickname, ev.Color);
	//		return;
	//	}

	}

	LbaNet::MessageEventPtr ev = new LbaNet::MessageEvent();
	ev->timestamp = IceUtil::Time::now().toMilliSeconds();
	ev->name = Sender;
	ev->message = Text;
	Publish(ev);
}

/***********************************************************
subscribe to chat events
***********************************************************/
void ChatReceiverServant::Subscribe(const ChatPollingSessionServantPtr session)
{
	IceUtil::Mutex::Lock sync(_mutex);
	_sessions.push_back(session);
}

/***********************************************************
unsubscribe from chat events
***********************************************************/
void ChatReceiverServant::Unsubscribe(const ChatPollingSessionServantPtr session)
{
	IceUtil::Mutex::Lock sync(_mutex);
	std::vector<ChatPollingSessionServantPtr>::iterator it = std::find(_sessions.begin(), _sessions.end(), session);
	if(it != _sessions.end())
		_sessions.erase(it);
}


/***********************************************************
publish an event
***********************************************************/
void ChatReceiverServant::Publish(const LbaNet::ChatRoomEventPtr & evt)
{
	IceUtil::Mutex::Lock sync(_mutex);
	for(size_t i=0; i<_sessions.size(); ++i)
		_sessions[i]->AddEvent(evt);
}