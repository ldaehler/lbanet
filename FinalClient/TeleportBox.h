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


#ifndef _LBA_NET_TELEPORT_BOX_
#define _LBA_NET_TELEPORT_BOX_

class GameGUI;

namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
}

#include <string>
#include <list>
#include <map>
struct TPInfo;

//*************************************************************************************************
//*                               class TeleportBox
//*************************************************************************************************
/**
* @brief Class handling a CommunityBox box
*
*/
class TeleportBox
{
 public:
	//! constructor
	 TeleportBox();

	//! destructor
	virtual ~TeleportBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! set the list of teleport places
	void SetTeleportList(const std::map<std::string, TPInfo> &_lists);

	//! display the chatbox on screen
	void Show();

	//! handle windows closing event
	bool HandleClose (const CEGUI::EventArgs& e);

	//! handle GO button pressed
	bool HandleGoButton (const CEGUI::EventArgs& e);

private:
	CEGUI::Window*			_myBox;

	std::map<std::string, TPInfo>	_tplist;
};

#endif
