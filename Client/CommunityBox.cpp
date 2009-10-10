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

#include "CommunityBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "ThreadSafeWorkpile.h"
#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyComListItem : public CEGUI::ListboxTextItem
{
public:
    MyComListItem (const CEGUI::String& text) 
		: CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};


/***********************************************************
constructor
***********************************************************/
CommunityBox::CommunityBox(GameGUI * gamgui)
: _gamgui(gamgui)
{
}


/***********************************************************
destructor
***********************************************************/
CommunityBox::~CommunityBox()
{
	try
	{
		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("CommunityFrame"));
		frw->subscribeEvent (CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&CommunityBox::HandleClose, this));

		CEGUI::UVector2 vec = frw->getPosition();
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.PosX", vec.d_x.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.PosY", vec.d_y.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.SizeX", frw->getWidth().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.SizeY", frw->getHeight().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.OffsetPosX", vec.d_x.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.OffsetPosY", vec.d_y.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.OffsetSizeX", frw->getWidth().d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Communitybox.OffsetSizeY", frw->getHeight().d_offset);
		ConfigurationManager::GetInstance()->SetBool("Gui.Communitybox.Visible", frw->isVisible());
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception destructor community box: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
initalize the box
***********************************************************/
void CommunityBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "community.layout" );
		Root->addChildWindow(_myBox);

		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("CommunityFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&CommunityBox::HandleClose, this));

		float PosX, PosY, SizeX, SizeY, OPosX, OPosY, OSizeX, OSizeY;
		bool Visible;
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.PosX", PosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.PosY", PosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.SizeX", SizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.SizeY", SizeY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.OffsetPosX", OPosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.OffsetPosY", OPosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.OffsetSizeX", OSizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Communitybox.OffsetSizeY", OSizeY);
		ConfigurationManager::GetInstance()->GetBool("Gui.Communitybox.Visible", Visible);
		frw->setPosition(CEGUI::UVector2(CEGUI::UDim(PosX, OPosX), CEGUI::UDim(PosY, OPosY)));
		frw->setWidth(CEGUI::UDim(SizeX, OSizeX));
		frw->setHeight(CEGUI::UDim(SizeY, OSizeY));

		if(Visible)
			frw->show();
		else
			frw->hide();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init community box: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool CommunityBox::HandleClose (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	//_gamgui->ShowComIcon();
	return true;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void CommunityBox::Show()
{
	if(_myBox->isVisible())
		_myBox->hide();
	else
	{
		_myBox->show();
		_myBox->activate();
	}
}




/***********************************************************
add people online
***********************************************************/
void CommunityBox::AddOnline(const std::string & listname, const std::string &_online,
							 const std::string &_status, const std::string &color)
{
	if(listname == "online")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/onlinelist"));

		std::string dis = "[colour='" + color + "']" + _online;
		if(_status != "")
			dis += " (" + _status + ")";

		std::map<std::string, size_t>::iterator itmap = _onlines.find(_online);
		if(itmap != _onlines.end())
		{
			CEGUI::ListboxItem *it = lb->getListboxItemFromIndex(itmap->second);
			if(it)
			{
				it->setText(dis);
				lb->invalidate();
			}
		}
		else
		{
			CEGUI::ListboxItem *it = new MyComListItem(dis);
			lb->addItem(it);
			_onlines[_online] = lb->getItemIndex(it);
		}
	}

	if(listname == "IRC")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/IRClist"));

		CEGUI::ListboxItem *it = lb->findItemWithText(_online, NULL);
		if(it == NULL)
			lb->addItem(new MyComListItem(_online));
	}
}

/***********************************************************
remove people online
***********************************************************/
void CommunityBox::RemoveOnline(const std::string & listname, const std::string &_offline)
{
	if(listname == "online")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/onlinelist"));



		std::map<std::string, size_t>::iterator itmap = _onlines.find(_offline);
		if(itmap != _onlines.end())
		{
			CEGUI::ListboxItem *it = lb->getListboxItemFromIndex(itmap->second);
			if(it)
				lb->removeItem(it);

			_onlines.erase(itmap);
		}
	}

	if(listname == "IRC")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/IRClist"));

		CEGUI::ListboxItem *it = lb->findItemWithText(_offline, NULL);
		if(it != NULL)
			lb->removeItem(it);
	}
}


/***********************************************************
used to process text to add
***********************************************************/
void CommunityBox::Process()
{
	std::vector<ThreadSafeWorkpile::JoinEvent> data;
	ThreadSafeWorkpile::getInstance()->GetJoinEventData(data);

	std::vector<ThreadSafeWorkpile::JoinEvent>::const_iterator it = data.begin();
	std::vector<ThreadSafeWorkpile::JoinEvent>::const_iterator end = data.end();
	for(; it != end; ++it)
	{
		if(it->Clear)
		{
			ClearList(it->ListName);
		}
		else
		{
			if(it->Joined)
				AddOnline(it->ListName, it->Nickname, it->Status, it->Color);
			else
				RemoveOnline(it->ListName, it->Nickname);
		}
	}
}


/***********************************************************
clear the list
***********************************************************/
void CommunityBox::ClearList(const std::string & listname)
{
	if(listname == "online")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/onlinelist"));
		lb->resetList();
	}

	if(listname == "IRC")
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Community/IRClist"));
		lb->resetList();
	}
}