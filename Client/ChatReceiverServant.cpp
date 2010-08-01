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
#include "ChatBox.h"
#include "ThreadSafeWorkpile.h"

// callback function called when a message is received from IceStorm
void ChatReceiverServant::Message(const std::string& Sender, const std::string& Text, const Ice::Current&)
{
	if(Sender == "info")
	{
		if(Text.substr(0, 7) == "#joined")
		{
			ThreadSafeWorkpile::JoinEvent ev;
			ev.ListName = "online";
			ev.Joined = true;
			ev.Clear = false;
			ev.Nickname = Text.substr(8);
			ev.Color = "FFFFFFFF";
			ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
			return;
		}

		if(Text.substr(0, 5) == "#left")
		{
			ThreadSafeWorkpile::JoinEvent ev;
			ev.ListName = "online";
			ev.Joined = false;
			ev.Clear = false;
			ev.Nickname = Text.substr(6);
			ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
			return;
		}

		if(Text.substr(0, 7) == "#status")
		{
			ThreadSafeWorkpile::JoinEvent ev;
			ev.ListName = "online";
			ev.Joined = true;
			ev.Clear = false;
			std::string tmptxt = Text.substr(8);
			ev.Nickname = tmptxt.substr(0, tmptxt.find(" "));
			tmptxt = tmptxt.substr(tmptxt.find(" ")+1);
			ev.Status = tmptxt.substr(0, tmptxt.find(" "));
			tmptxt = tmptxt.substr(tmptxt.find(" ")+1);
			ev.Color = tmptxt;
			ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
			ThreadSafeWorkpile::getInstance()->ChatColorChanged(ev.Nickname, ev.Color);
			return;
		}

	}

	// remove name from whisper
	std::string message = Text;
	if(message[0] == '/')
	{
		std::vector<std::string> tok;
		std::stringstream ss(message);
		std::string buf;
		while(ss >> buf)
		{
			tok.push_back(buf);
		}

		if(tok.size() > 1)
		{
			message = "";
			for(size_t i=1; i<tok.size(); ++i)
				message += tok[i] + " ";
		}
	}


	ThreadSafeWorkpile::ChatTextData cdata;
	cdata.Channel = _RoomName;
	cdata.Sender = Sender;
	cdata.Text = message;
	ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
}

