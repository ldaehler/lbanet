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


#ifndef _LBA_NET_DIALOG_BOX_
#define _LBA_NET_DIALOG_BOX_

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
#include <vector>


//*************************************************************************************************
//*                               class NPCDialogBox
//*************************************************************************************************
/**
* @brief Class handling a NPCDialogBox
*
*/
class NPCDialogBox
{
 public:
	//! constructor
	 NPCDialogBox(GameGUI * gamgui);

	//! destructor
	virtual ~NPCDialogBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void ShowDialog(long ActorId, const std::string &ActorName, bool Show);

	//! handle windows closing event
	bool HandleClose(const CEGUI::EventArgs& e);

	//! handle listbox selected
	bool Handlelbelected(const CEGUI::EventArgs& e);

protected:
	//! close dialog and inform actor
	void CloseDialog();

	//! build dialog depending of the actor
	void BuildDialog(long ActorId);

private:
	CEGUI::Window*			_myBox;
	GameGUI *				_gamgui;

	long					_current_dialoged_actor;
};

#endif
