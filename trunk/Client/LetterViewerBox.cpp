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

#include "LetterViewerBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "InventoryHandler.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>


/***********************************************************
constructor
***********************************************************/
LetterViewerBox::LetterViewerBox(GameGUI * gamgui)
: _gamgui(gamgui)
{

}


/***********************************************************
destructor
***********************************************************/
LetterViewerBox::~LetterViewerBox()
{

}


/***********************************************************
initalize the box
***********************************************************/
void LetterViewerBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "letterviewer.layout" );
		Root->addChildWindow(_myBox);

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("LetterViewerGoB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&LetterViewerBox::HandleOK, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("LetterViewerDestroyB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&LetterViewerBox::HandleCancel, this));

		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("LetterViewerWIndowFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&LetterViewerBox::HandleOK, this));

		_myBox->hide();

		InventoryHandler::getInstance()->SetLetterViewer(this);
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init LetterEditorBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
display the chatbox on screen
***********************************************************/
void LetterViewerBox::Show(long LetterId, const std::string & date, const std::string & from, 
							const std::string & subject, const std::string & message)
{
	_LetterId = LetterId;

	try
	{
		CEGUI::String str = "";
		if(date != "")
		{
			try
			{
				boost::posix_time::ptime pt = boost::posix_time::time_from_string(date);
				typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;
				boost::posix_time::ptime converted = local_adj::utc_to_local(pt);
				str += CEGUI::String((const unsigned char *)boost::posix_time::to_simple_string(converted).c_str());
				str += " - ";
			}
			catch(...){}
		}
		str += "Letter from ";
		str += CEGUI::String((const unsigned char *)from.c_str());
		CEGUI::WindowManager::getSingleton().getWindow("LetterViewerWIndowFrame")->setText(str);

		CEGUI::String strsub((const unsigned char *)subject.c_str());
		CEGUI::WindowManager::getSingleton().getWindow("LetterViewerSubjectEdit")->setText(strsub);

		CEGUI::String strmess((const unsigned char *)message.c_str());
		CEGUI::WindowManager::getSingleton().getWindow("LetterViewerMessage")->setText(strmess);


		if(!_myBox->isVisible())
			_myBox->show();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception LetterViewerBox Show: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
handle connect button event
***********************************************************/
bool LetterViewerBox::HandleOK(const CEGUI::EventArgs& e)
{
	_myBox->hide();
	_LetterId = -1;
	return true;
}

/***********************************************************
handle connect button event
***********************************************************/
bool LetterViewerBox::HandleCancel (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	ThreadSafeWorkpile::getInstance()->DestroyItem(_LetterId);
	_LetterId = -1;
	return true;
}
