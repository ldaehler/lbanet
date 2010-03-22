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


#ifndef _LBA_NET_JOURNAL_BOX_
#define _LBA_NET_JOURNAL_BOX_

class GameGUI;

namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
	class String;
	class TreeItem;
}

#include <string>
#include <list>
#include <map>
#include <vector>


//*************************************************************************************************
//*                               class JournalBox
//*************************************************************************************************
/**
* @brief Class handling a JournalBox
*
*/
class JournalBox
{
 public:
	//! constructor
	 JournalBox();

	//! destructor
	virtual ~JournalBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! display the chatbox on screen
	void Show();

	//! handle windows closing event
	bool HandleClose (const CEGUI::EventArgs& e);

	//! process what is needed in the game GUI
	void Process();


	//! handle event when list is selected
	bool HandleQuestTreeSelected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleQuestDoneTreeSelected (const CEGUI::EventArgs& e);

protected:
	//! call to regenerate the quest book display
	void RebuildBook(bool reset);

	//! create description list
	void CreateTextList(std::string text, std::vector<CEGUI::String> & list);

	//! display description
	void DisplayDescription(const std::vector<CEGUI::String> & text, bool questdone);

private:
	CEGUI::Window*			_myBox;

	std::map<std::string, CEGUI::TreeItem *>	_mapquestarea;
	std::map<std::string, CEGUI::TreeItem *>	_mapquestdonearea;

	std::vector<std::string>					_open_tree_quests;
	std::vector<std::string>					_open_tree_done_quests;
	std::string									_selected_tree_quests;
	std::string									_selected_tree_done_quests;
	bool										_first_book_init;
};

#endif
