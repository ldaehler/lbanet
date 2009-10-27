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

#include "ShortcutBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>
#include "GameGUI.h"
#include "LogHandler.h"
#include "ConfigurationManager.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "ImageSetHandler.h"
#include "InventoryHandler.h"

/***********************************************************
constructor
***********************************************************/
ShortcutBox::ShortcutBox(GameGUI * gamgui, int boxsize)
: _gamgui(gamgui), _boxsize(boxsize)
{



}


/***********************************************************
destructor
***********************************************************/
ShortcutBox::~ShortcutBox()
{
	try
	{
		ConfigurationManager::GetInstance()->SetBool("Gui.Shortcutbox.Visible", _myBox->isVisible());
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception destructor ShortcutBox: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
initalize the box
***********************************************************/
void ShortcutBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "shortcut_bar.layout" );
		Root->addChildWindow(_myBox);




		for(unsigned int i=0; i<14; ++i)
		{
			CEGUI::Window *tmpwindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmpwindow->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,0), 
								CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize));
			tmpwindow->setID(i);	

			_myBox->addChildWindow(tmpwindow);
			_inv_boxes.push_back(tmpwindow);

			CEGUI::Window*	tmp3 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmp3->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,4), CEGUI::UDim(1, -1), CEGUI::UDim(0, 14));
			tmp3->setProperty("FrameEnabled", "False");
			tmp3->setProperty("BackgroundEnabled", "False");
			tmp3->setProperty("MousePassThroughEnabled", "True");
			tmp3->setAlwaysOnTop(true);
			tmpwindow->addChildWindow(tmp3);


			if(i < 4)
			{
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseClick,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleChangeStance, this));

				std::stringstream strs;
				strs<<"F"<<i+1;
				tmp3->setText(strs.str().c_str());
			}
			else
			{
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseClick,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectClicked, this));

				tmpwindow->subscribeEvent(
							CEGUI::Window::EventDragDropItemDropped,
							CEGUI::Event::Subscriber(&ShortcutBox::handle_ItemDropped, this));

				std::stringstream strs;
				strs<<((i==13)?0:(i-3));
				tmp3->setText(strs.str().c_str());
			}
		}

		Resize();


		_myBox->subscribeEvent(
			CEGUI::FrameWindow::EventSized,
			CEGUI::Event::Subscriber (&ShortcutBox::HandleResize, this));

		// create the 4 first buttons for stances
		{
			CEGUI::Window*	tmp = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
			//tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,17), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
			tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,7), CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize-10));
			tmp->setProperty("Image", "set:"+ImageSetHandler::GetInstance()->GetStanceImage(1)+" image:full_image");
			_inv_boxes[0]->addChildWindow(tmp);
			tmp->setProperty("MousePassThroughEnabled", "True");
			_inv_boxes[0]->setProperty("Tooltip", "Normal");

			tmp = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
			//tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,17), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
			tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,7), CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize-10));		
			tmp->setProperty("Image", "set:"+ImageSetHandler::GetInstance()->GetStanceImage(2)+" image:full_image");
			_inv_boxes[1]->addChildWindow(tmp);
			tmp->setProperty("MousePassThroughEnabled", "True");
			_inv_boxes[1]->setProperty("Tooltip", "Sportive");

			tmp = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
			//tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,17), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
			tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,8), CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize-9));
			tmp->setProperty("Image", "set:"+ImageSetHandler::GetInstance()->GetStanceImage(3)+" image:full_image");
			_inv_boxes[2]->addChildWindow(tmp);
			tmp->setProperty("MousePassThroughEnabled", "True");
			_inv_boxes[2]->setProperty("Tooltip", "Aggressive");

			tmp = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
			//tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,17), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
			tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,10), CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize-7));
			tmp->setProperty("Image", "set:"+ImageSetHandler::GetInstance()->GetStanceImage(4)+" image:full_image");
			_inv_boxes[3]->addChildWindow(tmp);
			tmp->setProperty("MousePassThroughEnabled", "True");
			_inv_boxes[3]->setProperty("Tooltip", "Discrete");
		}


		bool Visible;
		ConfigurationManager::GetInstance()->GetBool("Gui.Shortcutbox.Visible", Visible);
		if(Visible)
			_myBox->show();
		else
			_myBox->hide();
		
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init InventoryBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ShortcutBox::HandleClose (const CEGUI::EventArgs& e)
{
	_myBox->hide();
	return true;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void ShortcutBox::Show()
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
handle windows resize event
***********************************************************/
bool ShortcutBox::HandleResize (const CEGUI::EventArgs& e)
{
	Resize();
	return true;
}


/***********************************************************
called by resize of the screen to fix the boxes correctly
***********************************************************/
void ShortcutBox::Resize()
{
	float sizewin = _myBox->getInnerRectClipper().getSize().d_width;
	float offsety = sizewin - ((_boxsize+2)*14) - 10;
	offsety /= 2;

	for(int i=0; i<14; ++i)
	{
		int offset = i*(_boxsize+2);
		if(i > 3)
			offset += 10;

		CEGUI::Window *tmpwindow = _inv_boxes[i];
		tmpwindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,offsety+offset), CEGUI::UDim(0,0)));
	}
}


/***********************************************************
handle click event
***********************************************************/
bool ShortcutBox::HandleChangeStance (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	if(dd_args.button == CEGUI::LeftButton)
	{
		ThreadSafeWorkpile::getInstance()->AddEvent(new ChangeStanceEvent(dd_args.window->getID()+1));
	}

	return true;
}


/***********************************************************
handle click event
***********************************************************/
bool ShortcutBox::HandleObjectClicked (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	unsigned int id = dd_args.window->getID();
	std::map<unsigned int, long>::iterator itm = _map_box_itemid.find(id);

	// use object
	if(dd_args.button == CEGUI::LeftButton)
	{
		if(itm != _map_box_itemid.end())
			ThreadSafeWorkpile::getInstance()->AddEvent(new InventoryObjectUsedEvent(itm->second));
	}

	// remove object 
	if(dd_args.button == CEGUI::RightButton)
	{
		if(dd_args.window->getChildCount() > 3)
		{
			dd_args.window->getChildAtIdx(3)->destroy();
			if(itm != _map_box_itemid.end())
				_map_box_itemid.erase(itm);

			InventoryHandler::getInstance()->SetShortcut(id-4, -1);
			dd_args.window->setProperty("Tooltip", "");
		}
	}

	return true;
}


/***********************************************************
drag and drop
***********************************************************/
bool ShortcutBox::handle_ItemDropped(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);


	SetShorcut(dd_args.window, dd_args.dragDropItem->getID());
	return true;
}



/***********************************************************
process what is needed in the game GUI
***********************************************************/
void ShortcutBox::Process()
{
	std::vector<int> scvec;
	ThreadSafeWorkpile::getInstance()->GetUsedShorcut(scvec);
	for(size_t i=0; i<scvec.size(); ++i)
	{
		int veci = scvec[i];
		unsigned int id = ((veci == 0)? 13: veci+3);
		std::map<unsigned int, long>::iterator itm = _map_box_itemid.find(id);
		if(itm != _map_box_itemid.end())
			ThreadSafeWorkpile::getInstance()->AddEvent(new InventoryObjectUsedEvent(itm->second));
	}


	if(InventoryHandler::getInstance()->HasShorcutUpdated())
	{
		std::vector<long> sh = InventoryHandler::getInstance()->GetShortcut();
		for(size_t i=0; i<sh.size(); ++i)
			SetShorcut(_inv_boxes[i+4], sh[i]);
	}
}


/***********************************************************
SetShorcut
***********************************************************/
void ShortcutBox::SetShorcut(CEGUI::Window* box, long itemid)
{
	// destroy old bounding
    if (box->getChildCount() == 4)
		box->getChildAtIdx(3)->destroy();


	_map_box_itemid[box->getID()] = itemid;

	// create new bounding
	if(itemid >= 0)
	{
		CEGUI::Window*	tmp = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
		tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,17), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
		
		std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(itemid);
		tmp->setProperty("Image", "set:"+imagesetname+" image:full_image");
		box->addChildWindow(tmp);
		tmp->setProperty("MousePassThroughEnabled", "True");
		box->setProperty("Tooltip", InventoryHandler::getInstance()->GetItemDescription(itemid));

		InventoryHandler::getInstance()->SetShortcut(box->getID()-4, itemid );
	}
}

