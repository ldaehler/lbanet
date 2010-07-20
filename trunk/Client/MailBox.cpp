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

#include "MailBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "LogHandler.h"
#include "ThreadSafeWorkpile.h"


// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItemmail : public CEGUI::ListboxTextItem
{
public:
    MyListItemmail (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};




/***********************************************************
util function
***********************************************************/
void replaceAll(std::string& context, const std::string& from, const std::string& to)
{
    size_t lookHere = 0;
    size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != std::string::npos)
    {
          context.replace(foundHere, from.size(), to);
          lookHere = foundHere + to.size();
    }
}

/***********************************************************
constructor
***********************************************************/
MailBox::MailBox()
{

}


/***********************************************************
destructor
***********************************************************/
MailBox::~MailBox()
{

}


/***********************************************************
initalize the box
***********************************************************/
void MailBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "mailbox.layout" );
		Root->addChildWindow(_myBox);
		_myBox->hide();


		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("Mailbox"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&MailBox::HandleQuit, this));


		CEGUI::MultiColumnList * mlist = static_cast<CEGUI::MultiColumnList *> (
			CEGUI::WindowManager::getSingleton().getWindow("Mailbox/list"));
		mlist->setSelectionMode(CEGUI::MultiColumnList::CellSingle);


		mlist->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged,
									CEGUI::Event::Subscriber (&MailBox::Handlelistselected, this));
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init MailBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
display the chatbox on screen
***********************************************************/
void MailBox::Show(const std::vector<LbaNet::PMInfo> &pms)
{
	try
	{
		CEGUI::MultiColumnList * mlist = static_cast<CEGUI::MultiColumnList *> (
			CEGUI::WindowManager::getSingleton().getWindow("Mailbox/list"));

		mlist->resetList();

		if(mlist)
		{
			for(size_t i=0; i<pms.size(); ++i)
			{
				mlist->addRow(i);
				{
				CEGUI::String tmp("[image='set:mailb_trash image:full_image' MousePassThroughEnabled='True']");
				CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
				itm->setTooltipText("delete");
				itm->setID(pms[i].PMId);
				mlist->setItem(itm, 1, i);
				
				}

				if(pms[i].Opened)
				{
					CEGUI::String tmp("[image='set:mailb_notnew image:full_image']");
					CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
					itm->setTooltipText("read");
					itm->setID(i);
					mlist->setItem(itm, 2, i);
				}
				else
				{
					CEGUI::String tmp("[image='set:mailb_new image:full_image']");
					CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
					itm->setTooltipText("unread");
					itm->setID(i);
					mlist->setItem(itm, 2, i);
				}

				{
				CEGUI::String tmp((const unsigned char *)pms[i].Date.c_str());
				CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
				itm->setTooltipText("wamba");
				itm->setID(i);
				mlist->setItem(itm, 3, i);
				}
				{
				CEGUI::String tmp((const unsigned char *)pms[i].FromName.c_str());
				CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
				itm->setID(i);
				mlist->setItem(itm, 4, i);
				}
				{
				std::string txt = pms[i].Text.substr(0, 25);
				replaceAll(txt, "\n", "");
				txt +="...";
				CEGUI::String tmp((const unsigned char *)txt.c_str());
				CEGUI::ListboxTextItem * itm = new MyListItemmail(tmp);
				itm->setID(i);
				mlist->setItem(itm, 5, i);
				}
			}
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init MailBox Show: ") + ex.getMessage().c_str());
	}

	if(!_myBox->isVisible())
		_myBox->show();
}


/***********************************************************
handle close button event
***********************************************************/
bool MailBox::HandleQuit(const CEGUI::EventArgs& e)
{
	_myBox->hide();
	return true;
}


/***********************************************************
handle list selected event
***********************************************************/
bool MailBox::Handlelistselected(const CEGUI::EventArgs& e)
{
	try
	{
		CEGUI::MultiColumnList * mlist = static_cast<CEGUI::MultiColumnList *> (
			CEGUI::WindowManager::getSingleton().getWindow("Mailbox/list"));

		if(mlist)
		{
			CEGUI::ListboxItem* li = mlist->getFirstSelectedItem();
			if(li)
			{
				long mid = (long)li->getID();

				if(li->getTooltipText() == "delete")
				{
					ThreadSafeWorkpile::getInstance()->DeletePM(mid);
				}
			}
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init MailBox Handlelistselected: ") + ex.getMessage().c_str());
	}

	return true;
}