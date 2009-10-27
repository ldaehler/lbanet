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


#ifndef _LBA_NET_CONTAINER_BOX_
#define _LBA_NET_CONTAINER_BOX_

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
//*                               class ContainerBox
//*************************************************************************************************
/**
* @brief Class handling a ContainerBox
*
*/
class ContainerBox
{
 public:
	//! constructor
	 ContainerBox(GameGUI * gamgui, int containersize, int boxsize);

	//! destructor
	virtual ~ContainerBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void Show();

	//! handle windows closing event
	bool HandleClose (const CEGUI::EventArgs& e);

	//! handle windows resize event
	bool HandleResize (const CEGUI::EventArgs& e);

	//! drag and drop
	bool handle_ItemDropped(const CEGUI::EventArgs& args);

	//! handle windows resize event
	bool HandleObjectClicked (const CEGUI::EventArgs& e);


	//! process what is needed in the game GUI
	void Process();

protected:
	//! resize container
	void ResizeBox();

private:
	CEGUI::Window*			_myBox;
	GameGUI *				_gamgui;

	int							_inventorysize;
	int							_boxsize;
	std::vector<CEGUI::Window*>	_inv_boxes;
	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >	_objects;
};

#endif
