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

#include "LetterEditorBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "InventoryHandler.h"

/***********************************************************
constructor
***********************************************************/
LetterEditorBox::LetterEditorBox(GameGUI * gamgui)
: _gamgui(gamgui)
{

}


/***********************************************************
destructor
***********************************************************/
LetterEditorBox::~LetterEditorBox()
{

}


/***********************************************************
initalize the box
***********************************************************/
void LetterEditorBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "lettereditor.layout" );
		Root->addChildWindow(_myBox);

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("LEditGoB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&LetterEditorBox::HandleOK, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("LEditCancelB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&LetterEditorBox::HandleCancel, this));

		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("LetterEditorWIndowFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&LetterEditorBox::HandleCancel, this));

		_myBox->hide();

		InventoryHandler::getInstance()->SetLetterEditor(this);
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init LetterEditorBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
display the chatbox on screen
***********************************************************/
void LetterEditorBox::Show()
{
	try
	{
		if(!_myBox->isVisible())
		{
			CEGUI::WindowManager::getSingleton().getWindow("LetterEditorSubjectEdit")->setText("");
			CEGUI::WindowManager::getSingleton().getWindow("LetterEditorMessage")->setText("");
			_myBox->show();
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception LetterEditorBox Show: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
handle connect button event
***********************************************************/
bool LetterEditorBox::HandleOK(const CEGUI::EventArgs& e)
{
	try
	{
		_myBox->hide();
		ThreadSafeWorkpile::WrittenLetter wl;
		wl.subject = CEGUI::WindowManager::getSingleton().getWindow("LetterEditorSubjectEdit")->getText().c_str();
		wl.message = CEGUI::WindowManager::getSingleton().getWindow("LetterEditorMessage")->getText().c_str();
		ThreadSafeWorkpile::getInstance()->AddWrittenLetter(wl);
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception LetterEditorBox HandleOK: ") + ex.getMessage().c_str());
	}

	return true;
}

/***********************************************************
handle connect button event
***********************************************************/
bool LetterEditorBox::HandleCancel (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	return true;
}
