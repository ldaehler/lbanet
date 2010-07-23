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


#ifndef _LBA_NET_MAIL_BOX_
#define _LBA_NET_MAIL_BOX_

class GameGUI;

namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
	class Vector2;
}

#include <string>
#include <list>
#include <map>
#include <vector>
#include <LbaTypes.h>

//*************************************************************************************************
//*                               class MailBox
//*************************************************************************************************
/**
* @brief Class handling a MailBox
*
*/
class MailBox
{
 public:
	//! constructor
	 MailBox(GameGUI * gamgui);

	//! destructor
	virtual ~MailBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void Show(const std::vector<LbaNet::PMInfo> &pms);

	//! handle close button event
	bool HandleQuit(const CEGUI::EventArgs& e);	

	//! handle list selected event
	bool Handlelistselected(const CEGUI::EventArgs& e);	

	//! handle list selected event
	bool HandleCloseRead(const CEGUI::EventArgs& e);	

	//! handle list selected event
	bool HandleReadDelete(const CEGUI::EventArgs& e);	

	//! handle list selected event
	bool HandleReadReply(const CEGUI::EventArgs& e);

	//! handle list selected event
	bool HandleCompose(const CEGUI::EventArgs& e);

	//! handle list selected event
	bool HandleWriteSend(const CEGUI::EventArgs& e);

	//! handle list selected event
	bool HandleWriteCancel(const CEGUI::EventArgs& e);

	//! handle list selected event
	bool Handlecontactselected(const CEGUI::EventArgs& e);	

	//! add mail window
	void HideMails();

protected:
	//! display a mail on the screen
	void DisplayMail(const LbaNet::PMInfo & pm);

	//! display a write mail on the screen
	void DisplayWriteMail();

	//! switch to mailbox
	void SwitchToMailBox();

private:
	CEGUI::Window*					_myBox;
	CEGUI::Window*					_readMailBox;
	CEGUI::Window*					_writeMailBox;

	std::vector<LbaNet::PMInfo>		_currentpms;
	long							_currentread;
	std::string						_currentcomposeTo;
	std::string						_currentcomposeText;

	long							_replyto;
	bool							_hidemailbox;
	
	GameGUI *						_gamgui;

};

#endif
