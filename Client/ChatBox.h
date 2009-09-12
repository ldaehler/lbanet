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


#ifndef _LBA_NET_CHAT_BOX_
#define _LBA_NET_CHAT_BOX_

class AgentSharedData;
class GameGUI;
class IrcThread;

namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
}

#include <iostream>
#include <string>
#include <list>



//*************************************************************************************************
//*                               class ChatBox
//*************************************************************************************************
/**
* @brief Class handling a chat box
*
*/
class ChatBox
{
 public:
	//! constructor
	 ChatBox(GameGUI * gamgui);

	//! destructor
	virtual ~ChatBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! set the current map of the game
	void SetCurrentMap(const std::string & WorldName, const std::string & MapName);


	//! handle send button event
	bool HandleSend (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleBChannel (const CEGUI::EventArgs& e);

	//! handle enter key
	bool HandleEnterKey (const CEGUI::EventArgs& e);

	//! handle release key
	bool HandleReleaseKey (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleLbSelected (const CEGUI::EventArgs& e);

	//! handle enter key
	bool HandleCCOk (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleCCCancel (const CEGUI::EventArgs& e);


	//! handle event when the channel window is closed
	bool HandleCloseChannels (const CEGUI::EventArgs& e);

	//! handle event when the channel window is closed
	bool HandleCloseChatbox (const CEGUI::EventArgs& e);

	//! display the chatbox on screen
	void Show();

	//! used to process text to add
	void Process();

	//! used to give focus to the chatbox
	void Focus(bool focus);

	// set irc handler
	void SetIrc(IrcThread * IRC)
	{_IRC = IRC;}

protected:
	// add a channel to the chat
	void AddChannel(const std::string & channel);

	// add a tab to the chat
	void AddTab(const std::string & tabName);

	// send text outside
	void SendText(const std::string & channel, const std::string & Text);

	//! add new text to the chatbox
	void AddText(std::string channel, const std::string & Sender, const std::string & Text);

private:
	CEGUI::Window*			_myChat;
	CEGUI::Listbox*			_lb;
	CEGUI::Window*			_myChannels;

	std::string				_currentWorld;
	std::string				_currentMap;
	std::list<std::string>	_channels;

	IrcThread *				_IRC;

	std::string				_text_copyed;
	bool					_control_key_on;
	bool					_shift_key_on;
	int						_currSelectedch;

	GameGUI *				_gamgui;
};

#endif
