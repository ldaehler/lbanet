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
#include "InventoryHandler.h"

#define _NB_BOX_TRADE_ 20


// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyDialogItem : public CEGUI::ListboxTextItem
{
public:
    MyDialogItem (const CEGUI::String& text, bool SelectionQuitDialog, bool SelectionTrade) 
		: CEGUI::ListboxTextItem(text), _SelectionQuitDialog(SelectionQuitDialog), _SelectionTrade(SelectionTrade)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }

	bool _SelectionQuitDialog;
	bool _SelectionTrade;
};


/***********************************************************
constructor
***********************************************************/
NPCDialogBox::NPCDialogBox(GameGUI * gamgui, int boxsize)
: _gamgui(gamgui), _current_dialoged_actor(-1), _boxsize(boxsize), _currentmoney(-1)
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
		_mytradeBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "trader.layout" );
		Root->addChildWindow(_myBox);
		Root->addChildWindow(_mytradeBox);


		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleClose, this));

		CEGUI::FrameWindow * frw2 = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogTraderFrame"));
		frw2->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleClose, this));

		frw2->subscribeEvent(
			CEGUI::FrameWindow::EventSized,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleResize, this));	


		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));
		lb->subscribeEvent(CEGUI::Listbox::EventMouseClick,
						CEGUI::Event::Subscriber (&NPCDialogBox::Handlelbelected, this));
		lb->subscribeEvent(CEGUI::Listbox::EventMouseMove,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleMouseEnter, this));

		_myBox->hide();
		_mytradeBox->hide();

		CEGUI::Window*	pane = CEGUI::WindowManager::getSingleton().getWindow("DialogTraderFrame/boxpart");
		CEGUI::Window*	tmpwindow;
		for(int i=0; i<_NB_BOX_TRADE_; ++i)
		{
			int x = i / 3;
			int y = i % 3;

			tmpwindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmpwindow->setArea(CEGUI::UDim(0,5+(_boxsize+2)*y), CEGUI::UDim(0,5+(_boxsize+2)*x), 
								CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize));
			pane->addChildWindow(tmpwindow);

			tmpwindow->setID(i);
			_inv_boxes.push_back(tmpwindow);
		}

		ResizeBox();

		frw->subscribeEvent(CEGUI::Window::EventKeyDown,
			CEGUI::Event::Subscriber (&NPCDialogBox::HandleEnterKey, this));
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
	_mytradeBox->hide();
	_current_dialoged_actor  = -1;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void NPCDialogBox::ShowDialog(long ActorId, const std::string &ActorName, const std::string & WelcomeSentence,
							  bool IsTrader, bool Show,
								const std::map<long, TraderItem> &inventory)
{
	if(Show)
	{
		if(_current_dialoged_actor == ActorId)
			return;

		if(_current_dialoged_actor >= 0)
			CloseDialog();

		static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DialogFrame"))->setText("Dialog with "+ActorName);

		_curr_inventory = inventory;
		BuildDialog(ActorId, WelcomeSentence, IsTrader);
		_myBox->show();
		_myBox->activate();
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
void NPCDialogBox::BuildDialog(long ActorId, const std::string & WelcomeSentence, bool IsTrader)
{
	CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/multiline")
										->setText(WelcomeSentence);


	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));

	lb->resetList();

	if(IsTrader)
	{
		CEGUI::ListboxItem *it = new MyDialogItem("Trade", false, true);
		lb->addItem(it);
	}

	CEGUI::ListboxItem *it = new MyDialogItem("Good bye. (End conversation)", true, false);
	lb->addItem(it);
	lb->setItemSelectState(it, true);
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

		if(it->_SelectionTrade)
			OpenTradeDialog();
	}

	return true;
}


/***********************************************************
open trading dialog
***********************************************************/
void NPCDialogBox::OpenTradeDialog()
{
	_myBox->hide();
	RefreshMoney();

	CleanItems();
	std::map<long, TraderItem>::iterator it = _curr_inventory.begin();
	for(size_t i=0; it != _curr_inventory.end(); ++i, ++it)
		 AddItem(it->first, _inv_boxes[i]);

	_mytradeBox->show();
}



/***********************************************************
handle windows resize event
***********************************************************/
bool NPCDialogBox::HandleResize (const CEGUI::EventArgs& e)
{
	ResizeBox();
	return false;
}


/***********************************************************
resize inventory
***********************************************************/
void NPCDialogBox::ResizeBox()
{
	CEGUI::Window*	win = CEGUI::WindowManager::getSingleton().getWindow("DialogTraderFrame/boxpart");
	CEGUI::Rect rect = win->getInnerRectClipper();
	float width = rect.getSize().d_width;
	int nbboxhori = (int)width / (_boxsize+2);

	for(int i=0; i<(int)_inv_boxes.size(); ++i)
	{
		int x = i / nbboxhori;
		int y = i % nbboxhori;

		_inv_boxes[i]->setPosition(CEGUI::UVector2(CEGUI::UDim(0,(_boxsize+2)*y), 
													CEGUI::UDim(0,(_boxsize+2)*x)));
	}
}


/***********************************************************
set player money
***********************************************************/
void NPCDialogBox::RefreshMoney()
{
	std::stringstream strs;
	strs<<"You have "<<_currentmoney<<" [image='set:"<<ImageSetHandler::GetInstance()->GetInventoryMiniImage(InventoryHandler::getInstance()->GetMoneyItemId())<<" image:full_image']";
	CEGUI::WindowManager::getSingleton().getWindow("DialogTraderFrame/kashes")->setText(strs.str());
}

/***********************************************************
set player money
***********************************************************/
void NPCDialogBox::AddItem(long Id, CEGUI::Window* parent)
{
	CEGUI::Window*	tmp = CEGUI::WindowManager::getSingleton().createWindow("DragContainer");
	tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,0), CEGUI::UDim(1, 0), CEGUI::UDim(1, 0));
	tmp->setID(Id);

	CEGUI::Window*	tmp2 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	tmp2->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,8), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));

	std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(Id);
	tmp2->setProperty("Image", "set:" + imagesetname + " image:full_image");
	tmp2->setProperty("MousePassThroughEnabled", "True");

	CEGUI::Window*	tmp3 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	tmp3->setArea(CEGUI::UDim(0,2), CEGUI::UDim(0,34), CEGUI::UDim(1, -1), CEGUI::UDim(0, 14));
	tmp3->setProperty("FrameEnabled", "False");
	tmp3->setProperty("BackgroundEnabled", "False");
	tmp3->setProperty("MousePassThroughEnabled", "True");
	tmp3->setProperty("Font" , "contourfont");
	tmp3->setAlwaysOnTop(true);
	std::stringstream strs2;
	strs2<<InventoryHandler::getInstance()->GetItemPrice(Id);
	tmp3->setText(strs2.str().c_str());

	std::stringstream strs;
	strs<<"<"<<InventoryHandler::getInstance()->GetItemPrice(Id)<<" kashes> "<<InventoryHandler::getInstance()->GetItemDescription(Id);
	CEGUI::String tmpstr((const unsigned char *)strs.str().c_str());
	tmp->setProperty("Tooltip", tmpstr);
	tmp->addChildWindow(tmp2);
	tmp->addChildWindow(tmp3);

    tmp->subscribeEvent (CEGUI::Window::EventMouseClick,
							CEGUI::Event::Subscriber (&NPCDialogBox::HandleObjectClicked, this));

	parent->addChildWindow(tmp);

	_objects.push_back(tmp);
}


/***********************************************************
clean current items
***********************************************************/
void NPCDialogBox::CleanItems()
{
	std::vector<CEGUI::Window*>::iterator it = _objects.begin();
	std::vector<CEGUI::Window*>::iterator end = _objects.end();
	for(;it != end; ++it)
		(*it)->destroy();

	_objects.clear();	
}

/***********************************************************
clean current items
***********************************************************/
bool NPCDialogBox::HandleObjectClicked (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	// use object
	if(dd_args.button == CEGUI::RightButton)
	{
		unsigned int id = dd_args.window->getID();
		InventoryHandler::getInstance()->BuyItem(_current_dialoged_actor, id);
	}


    return true;
}



/***********************************************************
process what is needed in the game GUI
***********************************************************/
void NPCDialogBox::Process()
{
	int money = InventoryHandler::getInstance()->GetItemNumber(InventoryHandler::getInstance()->GetMoneyItemId());
	if(money != _currentmoney)
	{
		_currentmoney = money;
		RefreshMoney();
	}

}



/***********************************************************
catch key event
***********************************************************/
bool NPCDialogBox::HandleEnterKey (const CEGUI::EventArgs& e)
{
	const CEGUI::KeyEventArgs& we =
    static_cast<const CEGUI::KeyEventArgs&>(e);

	if(we.scancode == CEGUI::Key::Space || we.scancode == CEGUI::Key::W)
	{
		CloseDialog();
		return true;
	}

	return false;
}



/***********************************************************
catch mouse enter event
***********************************************************/
bool NPCDialogBox::HandleMouseEnter (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("DialogFrame/listbox"));

	const CEGUI::MouseEventArgs& wine =
    static_cast<const CEGUI::MouseEventArgs&>(e);

	CEGUI::Point localPos(CEGUI::CoordConverter::screenToWindow(*lb, wine.position));
	CEGUI::ListboxItem * itm = lb->getItemAtPoint(localPos);
	if(itm)
		lb->setItemSelectState(itm, true);

	return true;
}