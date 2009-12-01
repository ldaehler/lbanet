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

#include "DialogBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"
#include "ImageSetHandler.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"


// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyDialogItem : public CEGUI::ListboxTextItem
{
public:
    MyDialogItem (const CEGUI::String& text, bool SelectionQuitDialog) 
		: CEGUI::ListboxTextItem(text), _SelectionQuitDialog(SelectionQuitDialog)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }

	bool _SelectionQuitDialog;
};


/***********************************************************
constructor
***********************************************************/
NPCDialogBox::NPCDialogBox(GameGUI * gamgui)
: _gamgui(gamgui), _current_dialoged_actor(-1)
{



}


/***********************************************************
destructor
***********************************************************/
NPCDialogBox::~NPCDialogBox()
{

}


/***********************************************************
initalize the box
***********************************************************/
void NPCDialogBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "dialog.layout" );
		Root->addChildWindow(_myBox);


		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleClose, this));


		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));
		lb->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
						CEGUI::Event::Subscriber (&NPCDialogBox::Handlelbelected, this));
		_myBox->hide();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init DialogBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool NPCDialogBox::HandleClose (const CEGUI::EventArgs& e)
{
	CloseDialog();
	return true;
}


/***********************************************************
close dialog and inform actor
***********************************************************/
void NPCDialogBox::CloseDialog()
{
	_myBox->hide();
	_current_dialoged_actor  = -1;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void NPCDialogBox::ShowDialog(long ActorId, const std::string &ActorName, bool Show)
{
	if(Show)
	{
		if(_current_dialoged_actor == ActorId)
			return;

		if(_current_dialoged_actor >= 0)
			CloseDialog();

		static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame"))->setText("Dialog with "+ActorName);

		BuildDialog(ActorId);
		_myBox->show();
		_current_dialoged_actor = ActorId;
	}
	else
	{
		if(_current_dialoged_actor == ActorId)
			CloseDialog();
	}
}



/***********************************************************
build dialog depending of the actor
***********************************************************/
void NPCDialogBox::BuildDialog(long ActorId)
{
	CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/multiline")
										->setText("Good day, what can I do for you?");


	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));

	lb->resetList();

	CEGUI::ListboxItem *it = new MyDialogItem("Good bye. (End conversation)", true);
	lb->addItem(it);

}



/***********************************************************
handle listbox selected
***********************************************************/
bool NPCDialogBox::Handlelbelected(const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));

	const MyDialogItem * it = static_cast<const MyDialogItem *>(lb->getFirstSelectedItem());
	if(it)
	{
		if(it->_SelectionQuitDialog)
			CloseDialog();
	}

	return true;
}