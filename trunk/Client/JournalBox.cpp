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

#include "JournalBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>

#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "QuestHandler.h"


// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyTreeItem : public CEGUI::TreeItem
{
public:
	MyTreeItem (const CEGUI::String& text, const std::vector<CEGUI::String> & Desc, long QuestId = -1) 
		: CEGUI::TreeItem(text), Description(Desc)
    {
		if(Desc.size() > 0)
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }

	long QuestId;
	std::vector<CEGUI::String> Description;
};


/***********************************************************
constructor
***********************************************************/
JournalBox::JournalBox()
{



}


/***********************************************************
destructor
***********************************************************/
JournalBox::~JournalBox()
{
	try
	{
		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("JournalFrame"));

		CEGUI::UVector2 vec = frw->getPosition();
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.PosX", vec.d_x.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.PosY", vec.d_y.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.SizeX", frw->getWidth().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.SizeY", frw->getHeight().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.OffsetPosX", vec.d_x.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.OffsetPosY", vec.d_y.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.OffsetSizeX", frw->getWidth().d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.JournalBox.OffsetSizeY", frw->getHeight().d_offset);
		ConfigurationManager::GetInstance()->SetBool("Gui.JournalBox.Visible", frw->isVisible());
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception destructor Inventorybox: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
initalize the box
***********************************************************/
void JournalBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "questbook.layout" );
		Root->addChildWindow(_myBox);


		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("JournalFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&JournalBox::HandleClose, this));

		CEGUI::Tree * tree = static_cast<CEGUI::Tree *> (
			CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questtab/Tree"));

		tree->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&JournalBox::HandleQuestTreeSelected, this));


		CEGUI::Tree * tree2 = static_cast<CEGUI::Tree *> (
			CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questdonetab/Tree"));

		tree2->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&JournalBox::HandleQuestDoneTreeSelected, this));


		float PosX, PosY, SizeX, SizeY, OPosX, OPosY, OSizeX, OSizeY;
		bool Visible;
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.PosX", PosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.PosY", PosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.SizeX", SizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.SizeY", SizeY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.OffsetPosX", OPosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.OffsetPosY", OPosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.OffsetSizeX", OSizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.JournalBox.OffsetSizeY", OSizeY);
		ConfigurationManager::GetInstance()->GetBool("Gui.JournalBox.Visible", Visible);
		frw->setPosition(CEGUI::UVector2(CEGUI::UDim(PosX, OPosX), CEGUI::UDim(PosY, OPosY)));
		frw->setWidth(CEGUI::UDim(SizeX, OSizeX));
		frw->setHeight(CEGUI::UDim(SizeY, OSizeY));

		if(Visible)
			frw->show();
		else
			frw->hide();


		static_cast<CEGUI::TabControl *> (
			CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab"))->setSelectedTab("Root/JournalWin/tab/questtab");
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init InventoryBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool JournalBox::HandleClose (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	return true;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void JournalBox::Show()
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
process what is needed in the game GUI
***********************************************************/
void JournalBox::Process()
{
	if(ThreadSafeWorkpile::getInstance()->QuestBookUpdateNeeded())
		RebuildBook();
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool JournalBox::HandleQuestTreeSelected (const CEGUI::EventArgs& e)
{
	CEGUI::Tree * tree = static_cast<CEGUI::Tree *> (
		CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questtab/Tree"));

	const MyTreeItem * it = static_cast<const MyTreeItem *>(tree->getFirstSelectedItem());

	if(it)
	{
		CEGUI::Window* descwin = 
			CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questtab/Description");

		bool firsttime = true;
		descwin->setText("");

		for(size_t i=0; i<it->Description.size(); ++i)
		{
			if(firsttime)
			{
				firsttime = false;
				descwin->setText(it->Description[i]);
			}
			else
				descwin->appendText(it->Description[i]);
		}
	}
	

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool JournalBox::HandleQuestDoneTreeSelected (const CEGUI::EventArgs& e)
{
	CEGUI::Tree * tree = static_cast<CEGUI::Tree *> (
		CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questdonetab/Tree"));

	const MyTreeItem * it = static_cast<const MyTreeItem *>(tree->getFirstSelectedItem());

	if(it)
	{
		CEGUI::Window* descwin = 
			CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questdonetab/Description");

		bool firsttime = true;
		descwin->setText("");

		for(size_t i=0; i<it->Description.size(); ++i)
		{
			if(firsttime)
			{
				firsttime = false;
				descwin->setText(it->Description[i]);
			}
			else
				descwin->appendText(it->Description[i]);
		}
	}
	

	return true;
}



/***********************************************************
call to regenerate the quest book display
***********************************************************/
void JournalBox::RebuildBook()
{
	CEGUI::Tree * tree = static_cast<CEGUI::Tree *> (
		CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questtab/Tree"));
	tree->resetList();

	CEGUI::Tree * tree2 = static_cast<CEGUI::Tree *> (
		CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questdonetab/Tree"));
	tree2->resetList();

	CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questtab/Description")->setText("");
	CEGUI::WindowManager::getSingleton().getWindow("Root/JournalWin/tab/questdonetab/Description")->setText("");

	std::vector<long> started = QuestHandler::getInstance()->GetQuestsStarted();
	std::vector<long> finished = QuestHandler::getInstance()->GetQuestsFinished();

	// rebuild quests
	{
		std::map<std::string, CEGUI::TreeItem *> tmpmap;

		for(size_t i=0; i<started.size(); ++i)
		{
			QuestInfo qi = QuestHandler::getInstance()->GetQuestInfo(started[i]);
			if(qi.Visible)
			{
				if(qi.QuestArea == "")
					qi.QuestArea = "General";

				std::vector<CEGUI::String> listt;
				CreateTextList(qi.Description, listt);

				CEGUI::TreeItem * titm = new MyTreeItem((const unsigned char *)qi.Tittle.c_str(), listt, qi.Id);


				std::map<std::string, CEGUI::TreeItem *>::iterator itm = tmpmap.find(qi.QuestArea);
				if(itm != tmpmap.end())
				{
					itm->second->addItem(titm);
				}
				else
				{
					std::vector<CEGUI::String> empty;
					CEGUI::TreeItem * root = new MyTreeItem((const unsigned char *)qi.QuestArea.c_str(), empty);
					tree->addItem(root);
					root->addItem(titm);
					tmpmap[qi.QuestArea] = root;
				}
			}
		}
	}


	// rebuild quests done
	{
		std::map<std::string, CEGUI::TreeItem *> tmpmap;

		for(size_t i=0; i<finished.size(); ++i)
		{
			QuestInfo qi = QuestHandler::getInstance()->GetQuestInfo(finished[i]);
			if(qi.Visible)
			{
				if(qi.QuestArea == "")
					qi.QuestArea = "General";

				std::vector<CEGUI::String> listt;
				CreateTextList(qi.Description, listt);

				CEGUI::TreeItem * titm = new MyTreeItem((const unsigned char *)qi.Tittle.c_str(), listt, qi.Id);


				std::map<std::string, CEGUI::TreeItem *>::iterator itm = tmpmap.find(qi.QuestArea);
				if(itm != tmpmap.end())
				{
					itm->second->addItem(titm);
				}
				else
				{
					std::vector<CEGUI::String> empty;
					CEGUI::TreeItem * root = new MyTreeItem((const unsigned char *)qi.QuestArea.c_str(), empty);
					tree2->addItem(root);
					root->addItem(titm);
					tmpmap[qi.QuestArea] = root;
				}
			}
		}
	}

}



/***********************************************************
create description list
***********************************************************/
void JournalBox::CreateTextList(std::string text, std::vector<CEGUI::String> & list)
{
	int idxs = 0;
	while((idxs = text.find(" @ ")) != std::string::npos)
	{
		std::string tmp = text.substr(0, idxs);
		if(tmp == "")
			tmp = "\n";

		list.push_back((const unsigned char *)tmp.c_str());

		while(((idxs+4) < (int)text.size()) && (text[idxs+3] == '@') && (text[idxs+4] == ' '))
		{
			list.push_back("\n");
			idxs+= 2;
		}

		text = text.substr(idxs+3);
	}


	list.push_back((const unsigned char *)text.c_str());
}
