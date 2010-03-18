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
//#include "WorldInfo.h"
//#include "Dialog.h"

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
	 NPCDialogBox(int boxsize);

	//! destructor
	virtual ~NPCDialogBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	//void ShowDialog(long ActorId, const std::string &ActorName, 
	//					DialogHandlerPtr Dialog, bool Show,
	//					const std::map<long, TraderItem> &inventory);

	//! handle windows closing event
	bool HandleClose(const CEGUI::EventArgs& e);

	//! handle listbox selected
	bool Handlelbelected(const CEGUI::EventArgs& e);

	//! handle windows resize event
	bool HandleResize (const CEGUI::EventArgs& e);

	//! handle windows resize event
	bool HandleObjectClicked (const CEGUI::EventArgs& e);

	//! catch key event
	bool HandleEnterKey (const CEGUI::EventArgs& e);

	//! catch mouse enter event
	bool HandleMouseEnter (const CEGUI::EventArgs& e);

	//! process what is needed in the game GUI
	void Process();

protected:
	//! close dialog and inform actor
	void CloseDialog();

	//! open trading dialog
	void OpenTradeDialog();

	//! build dialog depending of the actor
	void BuildDialog();

	//! resize inventory
	void ResizeBox();


	//! add an object
	void AddItem(long Id, CEGUI::Window* parent);

	//! clean current items
	void CleanItems();

	//! set player money
	void RefreshMoney();

private:
	CEGUI::Window*			_myBox;
	CEGUI::Window*			_mytradeBox;
	int						_boxsize;
	int						_currentmoney;

	std::vector<CEGUI::Window*>	_inv_boxes;

	long					_current_dialoged_actor;

	std::vector<CEGUI::Window*>	_objects;

	//std::map<long, TraderItem>	_curr_inventory;
	//DialogHandlerPtr			_curr_Dialog;

	bool						_rebuildDialog;
};

#endif
