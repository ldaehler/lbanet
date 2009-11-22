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


#ifndef _LBA_NET_LETTER_VIEWER_BOX_
#define _LBA_NET_LETTER_VIEWER_BOX_

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


//*************************************************************************************************
//*                               class LetterViewerBox
//*************************************************************************************************
/**
* @brief Class handling a LetterViewerBox
*
*/
class LetterViewerBox
{
 public:
	//! constructor
	 LetterViewerBox(GameGUI * gamgui);

	//! destructor
	virtual ~LetterViewerBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void Show(long LetterId, const std::string & date, const std::string & from, 
				const std::string & subject, const std::string & message);

	//! handle connect button event
	bool HandleOK(const CEGUI::EventArgs& e);

	//! handle cancel button event
	bool HandleCancel (const CEGUI::EventArgs& e);


private:
	CEGUI::Window*					_myBox;
	GameGUI *						_gamgui;

	long							_LetterId;

};

#endif
