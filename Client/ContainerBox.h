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
#include <LbaTypes.h>
#include "ThreadSafeWorkpile.h"

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
	 ContainerBox(GameGUI * gamgui, int boxsize);

	//! destructor
	virtual ~ContainerBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void Show();


	//! drag and drop
	bool handle_ItemDroppedInContainer(const CEGUI::EventArgs& args);
	bool handle_ItemDroppedInInventory(const CEGUI::EventArgs& args);
	bool handle_ItemDroppedInContainerItem(const CEGUI::EventArgs& args);
	bool handle_ItemDroppedInInventoryItem(const CEGUI::EventArgs& args);


	//! handle windows resize event
	bool HandleContainerItemClicked (const CEGUI::EventArgs& e);
	bool HandleInventoryItemClicked (const CEGUI::EventArgs& e);

		
	//! handle windows closing event
	bool HandleOk (const CEGUI::EventArgs& e);
		
	//! handle windows closing event
	bool HandleTakeAll (const CEGUI::EventArgs& e);

	//! handle windows closing event
	bool HandleCancel (const CEGUI::EventArgs& e);

	//! process what is needed in the game GUI
	void Process();

	//! refresh inventory images
	void Refresh();

	//! handle windows enter event
	bool HandleInventoryEnter (const CEGUI::EventArgs& e);

protected:

	//! close box and send update to server
	void CloseAndUpdate();
	
	//! cancel changes
	void Cancel();

	//! take all item from container
	void TakeAll();

	//! add item inside the container
	void AddContainerItem(long Id, int number, CEGUI::Window* parent);

	//! clean container items
	void CleanContainer();

	//! resize inventory
	void ResizeInventory(int newsize);

	//! update inventory
	void UpdateInventory(std::vector<std::pair<long, int> > inv);

	//! clean inventory
	void CleanInventory();

	//! refresh inventory
	void RefreshInventory();

	//! add item inside the container
	std::pair<CEGUI::Window*, CEGUI::Window*> AddInventoryItem(long Id, int number, CEGUI::Window* parent, 
																bool tocontainer);

	
	//! add an item from container to inventory
	//! return the number taken
	int AddItemFromContainerToInventory(long Id, int number);

	//! switch item from container to inventory
	void switchfromcontainertoinventory(long Id, bool full);


	//! update taken and put vectors with last change from user
	void UpdateTakenPut(long itid, int deltaupd);

	
	//! add an item from inventory to container
	//! return the number taken
	int AddItemFromInventoryToContainer(long Id, int number);
	
	//! switch item from inventory to container
	void switchfrominventorytocontainer(long Id, bool full);

	//! find first empty space in inventory
	CEGUI::Window* FindFirstContainerEmptySpace();

private:
	CEGUI::Window*				_myBox;
	GameGUI *					_gamgui;
	int							_boxsize;

	// last server info
	LbaNet::ContainerInfo											_currContainerCopy;
	LbaNet::ContainerInfo											_currContainer;
	ThreadSafeWorkpile::UpdateInvContainer							_serverupdate;


	// container part
	std::vector<CEGUI::Window*>										_cont_boxes;
	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >		_cont_objects;


	// inventory part
	int																_inventory_size;
	std::vector<CEGUI::Window*>										_inv_boxes;
	std::vector<std::pair<long, int> >								_inventory_data;
	std::vector<std::pair<CEGUI::Window*, CEGUI::Window*> >			_inventory_windows;

	double															_last_opened_time;
};

#endif
