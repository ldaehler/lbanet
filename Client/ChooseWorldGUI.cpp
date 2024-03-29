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

#include "ChooseWorldGUI.h"
#include <CEGUI.h>
#include "LogHandler.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MusicHandler.h"


class MyListItemCW : public CEGUI::ListboxTextItem
{
public:
    MyListItemCW (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};


/***********************************************************
	Constructor
***********************************************************/
ChooseWorldGUI::ChooseWorldGUI()
{
}


/***********************************************************
	Destructor
***********************************************************/
ChooseWorldGUI::~ChooseWorldGUI()
{
}


/***********************************************************
init function
***********************************************************/
void ChooseWorldGUI::Initialize(void)
{
	try
	{
		_root = CEGUI::WindowManager::getSingleton().loadWindowLayout( "ChooseWorldWindow.layout" );

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("CWGoB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChooseWorldGUI::HandleConnect, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("CWCancelB"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChooseWorldGUI::HandleCancel, this));

		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("ChooseWorldList"));
		lb->subscribeEvent (CEGUI::Listbox::EventSelectionChanged,
			CEGUI::Event::Subscriber (&ChooseWorldGUI::HandleWorldSelected, this));

		lb->subscribeEvent (CEGUI::Window::EventKeyDown,
			CEGUI::Event::Subscriber (&ChooseWorldGUI::HandleEnterKey, this));

		CEGUI::WindowManager::getSingleton().getWindow("CWLBaNetLogo")->disable();
		CEGUI::WindowManager::getSingleton().getWindow("CWLBaNetLogoCenter")->disable();

		static_cast<CEGUI::FrameWindow *>(
			CEGUI::WindowManager::getSingleton().getWindow("CWWIndowFrame"))->setDragMovingEnabled(false);

		static_cast<CEGUI::FrameWindow *>(
			CEGUI::WindowManager::getSingleton().getWindow("CWWIndowFrame"))->setRollupEnabled(false);



	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init login gui: ") + ex.getMessage().c_str());
		_root = NULL;
	}
}




/***********************************************************
handle connect button event
***********************************************************/
bool ChooseWorldGUI::HandleConnect(const CEGUI::EventArgs& e)
{
	if(_selectedworld != "")
	{
		std::string samples = "Data/Samples/lba2launcherblob.wav";
		MusicHandler::getInstance()->PlaySample(samples, 0);
		ThreadSafeWorkpile::getInstance()->AddEvent(new ChangeWorldEvent(_selectedworld, _selectedworldFile));
	}

	return true;
}


/***********************************************************
handle cancel button event
***********************************************************/
bool ChooseWorldGUI::HandleCancel (const CEGUI::EventArgs& e)
{
	ThreadSafeWorkpile::getInstance()->AddEvent(new GuiExitEvent());
	return true;
}


/***********************************************************
set the list of available worlds
***********************************************************/
void ChooseWorldGUI::SetWorldList(const std::vector<WorldDesc> &list)
{
	_wlist = list;

	try
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("ChooseWorldList"));
		if(lb)
		{
			std::vector<WorldDesc>::const_iterator it = _wlist.begin();
			std::vector<WorldDesc>::const_iterator end = _wlist.end();
			for(int cc=0; it != end; ++it, ++cc)
			{
				MyListItemCW * item = new MyListItemCW(it->WorldName);
				lb->addItem(item);
				if(cc == 2)
					lb->setItemSelectState(item, true);
			}
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init the world list: ") + ex.getMessage().c_str());
		_root = NULL;
	}
}


/***********************************************************
handle world selected event
***********************************************************/
bool ChooseWorldGUI::HandleWorldSelected (const CEGUI::EventArgs& e)
{
	try
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("ChooseWorldList"));
		if(lb)
		{
			size_t idx = lb->getItemIndex(lb->getFirstSelectedItem());
			if(idx < _wlist.size())
			{
				CEGUI::MultiLineEditbox * eb = static_cast<CEGUI::MultiLineEditbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("ChooseWorldDescription"));
				if(eb)
				{
					eb->setText(_wlist[idx].Description);
				}

				_selectedworld = _wlist[idx].WorldName;
				_selectedworldFile = _wlist[idx].FileName;
			}
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init the world list: ") + ex.getMessage().c_str());
		_root = NULL;
	}

	return true;
}



/***********************************************************
called to infrom the gui that it is displayed
***********************************************************/
void ChooseWorldGUI::Displayed()
{
	CEGUI::WindowManager::getSingleton().getWindow("ChooseWorldList")->activate();
}


/***********************************************************
catch key event
***********************************************************/
bool ChooseWorldGUI::HandleEnterKey (const CEGUI::EventArgs& e)
{
	const CEGUI::KeyEventArgs& we =
    static_cast<const CEGUI::KeyEventArgs&>(e);

	if(we.scancode == CEGUI::Key::Return)
	{	
		return HandleConnect(e);
	}

	return false;
}
