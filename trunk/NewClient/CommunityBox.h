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


#ifndef _LBA_NET_COMMUNITY_BOX_
#define _LBA_NET_COMMUNITY_BOX_


namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
	class ListboxItem;
}

#include <string>
#include <list>
#include <vector>
#include <map>

//*************************************************************************************************
//*                               class CommunityBox
//*************************************************************************************************
/**
* @brief Class handling a CommunityBox box
*
*/
class CommunityBox
{
 public:
	//! constructor
	 CommunityBox();

	//! destructor
	virtual ~CommunityBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);


	//! handle event when the CommunityBox is closed
	bool HandleClose(const CEGUI::EventArgs& e);

	//! display the chatbox on screen
	void Show();

	//! used to process text to add
	void Process();

	//! handle event when add friend clicked
	bool HandleAddFriend(const CEGUI::EventArgs& e);

	//! handle event when remove friend clicked
	bool HandleRemoveFriend(const CEGUI::EventArgs& e);

	//! handle enter key
	bool HandleCPOk (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleCPCancel (const CEGUI::EventArgs& e);

	//! handle event when list is double clicked
	bool HandleListdblClick (const CEGUI::EventArgs& e);

	//! handle event when list is double clicked
	bool HandleConnecteddblClick (const CEGUI::EventArgs& e);


protected:

	//! add people online
	void AddOnline(const std::string & listname, const std::string &_online, 
						const std::string &_status, const std::string &color);

	//! remove people online
	void RemoveOnline(const std::string & listname, const std::string &_offline);

	//! clear the list
	void ClearList(const std::string & listname);


	//! return true if is friend
	bool IsFriend(const std::string & name);

	//! add people friend
	void UpdateFriend(const std::string & name);

	//! remove people friend
	void RemoveFriend(const std::string & name);

private:
	CEGUI::Window*			_myBox;
	CEGUI::Window*			_myChooseName;

	std::map<std::string, CEGUI::ListboxItem *> _onlines;

	std::map<std::string, CEGUI::ListboxItem *> _friends;
};

#endif
