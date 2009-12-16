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

#if !defined(__LbaNetModel_1_GameGUI_h)
#define __LbaNetModel_1_GameGUI_h

#include "GUI.h"
#include <string>
#include <list>

#include "ChatBox.h"
#include "CommunityBox.h"
#include "TeleportBox.h"
#include "EditorBox.h"
#include "InventoryBox.h"
#include "ShortcutBox.h"
#include "ContainerBox.h"
#include "LetterEditorBox.h"
#include "LetterViewerBox.h"
#include "DialogBox.h"
#include "WorldInfo.h"

namespace CEGUI
{
	class Window;
	class EventArgs;
}
struct TPInfo;
class Actor;

/***********************************************************************
 * Module:  GameGUI.h
 * Author:  Vivien
 * Modified: samedi 11 juillet 2009 12:48:06
 * Purpose: Declaration of the class GameGUI
 ***********************************************************************/
class GameGUI : public GUI
{
public:
	//! constructor
	GameGUI();

	//! destructor
	~GameGUI();

	//! init function
	virtual void Initialize(void);


	//! handle send button event
	bool HandleChatIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleComIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleTeleIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleQuitIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleSoundIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleChangeWorldIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleOptionIconClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleCloseTextClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleInventoryClicked (const CEGUI::EventArgs& e);

	//! handle send button event
	bool HandleShortcutClicked (const CEGUI::EventArgs& e);

	// process what is needed in the game GUI
	virtual void Process();

	//! called to infrom the gui that it is displayed
	void Displayed(){}

	//! set the list of teleport places
	void SetTeleportList(const std::map<std::string, TPInfo> &_lists);

	//! refresh sound button
	void RefreshSOundButton();

	//! set the current map of the game
	void SetCurrentMap(const std::string & WorldName, const std::string & MapName)
	{
		_cb.SetCurrentMap(WorldName, MapName);
		if(_editb)
			_editb->ChangeMap(WorldName, MapName);
	}

	// focus the chatbox
	void FocusChatbox(bool focus);

	//! set irc thread
	void SetIrcThread(IrcThread * IT);

	//! display game text
	bool DisplayGameText(long textid, bool show);

	//! set actors
	void SetActors(std::map<long, Actor *> * Lactors, std::map<long, Actor *> * Eactors)
	{if(_editb) _editb->SetActors(Lactors, Eactors);}

	//! set player name
	void SetPlayerName(const std::string & name);

	//! refreshthe gui
	void Refresh();

	// display inventory
	void ShowInventory();

	// show dialog with NPC
	void ShowDialog(long ActorId, const std::string &ActorName, const std::string & WelcomeSentence, 
						bool IsTrader, bool Show,
						const std::map<long, TraderItem> &inventory);

private:
	ChatBox			_cb;
	CommunityBox	_comb;
	TeleportBox		_telb;
	InventoryBox	_invb;
	ShortcutBox		_shortb;
	ContainerBox	_containerb;
	LetterEditorBox _lettereditb;
	LetterViewerBox _letterviewb;
	NPCDialogBox	_dialogb;

	EditorBox *		_editb;
};

#endif