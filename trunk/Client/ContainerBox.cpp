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

#include "ContainerBox.h"
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

/***********************************************************
constructor
***********************************************************/
ContainerBox::ContainerBox(GameGUI * gamgui, int inventorysize, int boxsize)
: _gamgui(gamgui), _inventorysize(inventorysize), _boxsize(boxsize)
{



}


/***********************************************************
destructor
***********************************************************/
ContainerBox::~ContainerBox()
{
	try
	{
		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame"));
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception destructor Inventorybox: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
initalize the box
***********************************************************/
void ContainerBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "container.layout" );
		Root->addChildWindow(_myBox);


		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame"));
		frw->subscribeEvent (
			CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&ContainerBox::HandleClose, this));

		frw->subscribeEvent(
			CEGUI::FrameWindow::EventSized,
			CEGUI::Event::Subscriber (&ContainerBox::HandleResize, this));

		frw->show();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init ContainerBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ContainerBox::HandleClose (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	return true;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void ContainerBox::Show()
{
	_myBox->show();
	_myBox->activate();
}




/***********************************************************
handle windows resize event
***********************************************************/
bool ContainerBox::HandleResize (const CEGUI::EventArgs& e)
{
	ResizeBox();
	return false;
}


/***********************************************************
resize inventory
***********************************************************/
void ContainerBox::ResizeBox()
{
	CEGUI::Window*	win = CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame");
	CEGUI::Rect rect = win->getInnerRectClipper();
	float width = rect.getSize().d_width;
	int nbboxhori = (int)width / (_boxsize+2);

	for(int i=0; i<_inv_boxes.size(); ++i)
	{
		int x = i / nbboxhori;
		int y = i % nbboxhori;

		_inv_boxes[i]->setPosition(CEGUI::UVector2(CEGUI::UDim(0,(_boxsize+2)*y), 
													CEGUI::UDim(0,(_boxsize+2)*x)));
	}
}



/***********************************************************
drag and drop
***********************************************************/
bool ContainerBox::handle_ItemDropped(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

  //  // cast the args to the 'real' type so we can get access to extra fields
  //  const DragDropEventArgs& dd_args =
  //      static_cast<const DragDropEventArgs&>(args);

  //  if (dd_args.window->getChildCount() == 2)
  //  {
		//// add dragdrop item as child of target if target has no item already
		//dd_args.window->addChildWindow(dd_args.dragDropItem);

		//// Now we must reset the item position from it's 'dropped' location,
		//// since we're now a child of an entirely different window
		//dd_args.dragDropItem->setPosition(UVector2(UDim(0, 0),UDim(0, 0)));

		//InventoryHandler::getInstance()->UpdateItemPosition(dd_args.dragDropItem->getID(), dd_args.window->getID());
  //  }

    return true;
}



/***********************************************************
object clicked
***********************************************************/
bool ContainerBox::HandleObjectClicked (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	//// use object
	//if(dd_args.button == CEGUI::RightButton)
	//{
	//	unsigned int id = dd_args.window->getID();
	//	ThreadSafeWorkpile::getInstance()->AddEvent(new InventoryObjectUsedEvent(id));
	//}


    return true;
}



/***********************************************************
process what is needed in the game GUI
***********************************************************/
void ContainerBox::Process()
{
	//if(InventoryHandler::getInstance()->HasInventoryUpdated())
	//{
	//	CleanInventory();

	//	int inventorysize;
	//	std::map<long, std::pair<int, int> > currinv = InventoryHandler::getInstance()->GetCurrentInventory(inventorysize);
	//	
	//	ResizeInventory(inventorysize);
	//	
	//	std::map<long, std::pair<int, int> >::iterator it = currinv.begin();
	//	std::map<long, std::pair<int, int> >::iterator end = currinv.end();
	//	for(; it != end; ++it)
	//	{
	//		UpdateItem(it->first, InventoryHandler::getInstance()->GetItemDescription(it->first),
	//							it->second.first, it->second.second);
	//	}
	//}

	//std::vector<std::pair<long, int> > items;
	//InventoryHandler::getInstance()->GetUpdatedItem(items);
	//for(size_t i=0; i<items.size(); ++i)
	//	UpdateItem(items[i].first, InventoryHandler::getInstance()->GetItemDescription(items[i].first),
	//						items[i].second);
}



