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


#ifndef _LBA_NET_INVENTORY_BOX_
#define _LBA_NET_INVENTORY_BOX_

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
//*                               class InventoryBox
//*************************************************************************************************
/**
* @brief Class handling a InventoryBox
*
*/
class InventoryBox
{
 public:
	//! constructor
	 InventoryBox(GameGUI * gamgui, int boxsize);

	//! destructor
	virtual ~InventoryBox();

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

	//! drag and drop
	bool handle_ItemDroppedOnItem(const CEGUI::EventArgs& args);

	//! update item to the inventory
	//! if item didnt exist add it to the inventory
	//! if number = 0 remove it from inventory
	//! if no more place in inventory when adding then return false
	bool UpdateItem(long Id, const std::string & Description,
					long number, int PlaceInInventory = -1);

	//! handle windows resize event
	bool HandleObjectClicked (const CEGUI::EventArgs& e);

	//! process what is needed in the game GUI
	void Process();

protected:
	//! resize inventory
	void ResizeBox();

	//! find first empty space in inventory
	CEGUI::Window* FindFirstEmptySpace();

	//! empty the inventory
	void CleanInventory();

	//! resize inventory
	void ResizeInventory(int newsize);


private:
	CEGUI::Window*			_myBox;
	GameGUI *				_gamgui;

	int							_inventorysize;
	int							_boxsize;
	std::vector<CEGUI::Window*>	_inv_boxes;

	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >	_objects;
};

#endif
