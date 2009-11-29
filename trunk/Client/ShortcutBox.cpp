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
: _gamgui(gamgui), _boxsize(boxsize), _moving(false), _moving_stances(false), _currentvisibility(0)
{
	mMousePosInWindow = new CEGUI::Vector2();
	mMousePosInWindow_stances = new CEGUI::Vector2();
}


/***********************************************************
destructor
***********************************************************/
ShortcutBox::~ShortcutBox()
{
	delete mMousePosInWindow;
	delete mMousePosInWindow_stances;

	try
	{
		ConfigurationManager::GetInstance()->SetInt("Gui.Shortcutbox.Visible", _currentvisibility);

		CEGUI::UVector2 vec = _myBox->getPosition();
		ConfigurationManager::GetInstance()->SetFloat("Gui.Shortcutbox.PosX", vec.d_x.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Shortcutbox.PosY", vec.d_y.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Shortcutbox.OffsetPosX", vec.d_x.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Shortcutbox.OffsetPosY", vec.d_y.d_offset);

		vec = _myStances->getPosition();
		ConfigurationManager::GetInstance()->SetFloat("Gui.Stancesbox.PosX", vec.d_x.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Stancesbox.PosY", vec.d_y.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Stancesbox.OffsetPosX", vec.d_x.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Stancesbox.OffsetPosY", vec.d_y.d_offset);
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

		_myStances = CEGUI::WindowManager::getSingleton().loadWindowLayout( "stance_bar.layout" );
		Root->addChildWindow(_myStances);




		for(unsigned int i=0; i<14; ++i)
		{
			CEGUI::Window *tmpwindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmpwindow->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,0), 
								CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize));
			tmpwindow->setID(i);	


			_inv_boxes.push_back(tmpwindow);

			CEGUI::Window*	tmp3 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmp3->setArea(CEGUI::UDim(0,2), CEGUI::UDim(0,4), CEGUI::UDim(1, -1), CEGUI::UDim(0, 14));
			tmp3->setProperty("FrameEnabled", "False");
			tmp3->setProperty("BackgroundEnabled", "False");
			tmp3->setProperty("MousePassThroughEnabled", "True");
			tmp3->setAlwaysOnTop(true);
			tmp3->setProperty("Font" , "contourfont");
			tmpwindow->addChildWindow(tmp3);


			if(i < 4)
			{
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseClick,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleChangeStance, this));

				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseButtonDown,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectPressedStances, this));
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseButtonUp,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectReleasedStances, this));

				tmpwindow->subscribeEvent(CEGUI::Window::EventMouseMove, 
										CEGUI::Event::Subscriber(&ShortcutBox::onWindowMove, this));

				std::stringstream strs;
				strs<<"F"<<i+1;
				tmp3->setText(strs.str().c_str());
				_myStances->addChildWindow(tmpwindow);
			}
			else
			{
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseClick,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectClicked, this));

				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseButtonDown,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectPressed, this));
				tmpwindow->subscribeEvent (CEGUI::Window::EventMouseButtonUp,
					CEGUI::Event::Subscriber (&ShortcutBox::HandleObjectReleased, this));

				tmpwindow->subscribeEvent(
							CEGUI::Window::EventDragDropItemDropped,
							CEGUI::Event::Subscriber(&ShortcutBox::handle_ItemDropped, this));

				tmpwindow->subscribeEvent(CEGUI::Window::EventMouseMove, 
										CEGUI::Event::Subscriber(&ShortcutBox::onWindowMove, this));

				std::stringstream strs;
				strs<<((i==13)?0:(i-3));
				tmp3->setText(strs.str().c_str());
				_myBox->addChildWindow(tmpwindow);
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


		float PosX, PosY, OPosX, OPosY;
		ConfigurationManager::GetInstance()->GetFloat("Gui.Shortcutbox.PosX", PosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Shortcutbox.PosY", PosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Shortcutbox.OffsetPosX", OPosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Shortcutbox.OffsetPosY", OPosY);
		_myBox->setPosition(CEGUI::UVector2(CEGUI::UDim(PosX, OPosX), CEGUI::UDim(PosY, OPosY)));

		ConfigurationManager::GetInstance()->GetFloat("Gui.Stancesbox.PosX", PosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Stancesbox.PosY", PosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Stancesbox.OffsetPosX", OPosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Stancesbox.OffsetPosY", OPosY);
		_myStances->setPosition(CEGUI::UVector2(CEGUI::UDim(PosX, OPosX), CEGUI::UDim(PosY, OPosY)));


		ConfigurationManager::GetInstance()->GetInt("Gui.Shortcutbox.Visible", _currentvisibility);
		RefreshVivsibleStuff();

		Root->subscribeEvent(CEGUI::Window::EventMouseMove, 
								CEGUI::Event::Subscriber(&ShortcutBox::onWindowMove, this));

		_myBox->subscribeEvent(CEGUI::Window::EventMouseMove, 
								CEGUI::Event::Subscriber(&ShortcutBox::onWindowMove, this));


	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init InventoryBox: ") + ex.getMessage().c_str());
	}
}




/***********************************************************
display the chatbox on screen
***********************************************************/
void ShortcutBox::Show()
{
	++_currentvisibility;
	if(_currentvisibility > 3)
		_currentvisibility = 0;

	RefreshVivsibleStuff();
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
	for(int i=0; i<4; ++i)
	{
		int offset = i*(_boxsize+2);

		CEGUI::Window *tmpwindow = _inv_boxes[i];
		tmpwindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,offset), CEGUI::UDim(0,0)));
	}

	for(int i=4; i<14; ++i)
	{
		int offset = (i-4)*(_boxsize+2);
		CEGUI::Window *tmpwindow = _inv_boxes[i];
		tmpwindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,offset), CEGUI::UDim(0,0)));
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

	// make sure we drop a valid item
	if((dd_args.dragDropItem->getChildCount() > 1) && dd_args.dragDropItem->getChildAtIdx(1)->getID() == 1)
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
		tmp->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,12), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));
		
		std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(itemid);
		tmp->setProperty("Image", "set:"+imagesetname+" image:full_image");
		box->addChildWindow(tmp);
		tmp->setProperty("MousePassThroughEnabled", "True");

		CEGUI::String tmpstr((const unsigned char *)InventoryHandler::getInstance()->GetItemDescription(itemid).c_str());
		box->setProperty("Tooltip", tmpstr);


		box->subscribeEvent(
			CEGUI::Window::EventMouseEnters,
					CEGUI::Event::Subscriber(&ShortcutBox::HandleInventoryEnter, this));

		InventoryHandler::getInstance()->SetShortcut(box->getID()-4, itemid );
	}
}


/***********************************************************
refresh inventory images
***********************************************************/
void  ShortcutBox::Refresh()
{
	std::vector<CEGUI::Window*>::iterator itinvd = _inv_boxes.begin();
	std::vector<CEGUI::Window*>::iterator endinvd = _inv_boxes.end();
	for(; itinvd != endinvd; ++itinvd)
	{
		if ((*itinvd)->getChildCount() == 4)
		{
			std::map<unsigned int, long>::iterator itm = _map_box_itemid.find((*itinvd)->getID());
			if(itm != _map_box_itemid.end())
			{
				std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(itm->second);
				(*itinvd)->getChildAtIdx(3)->setProperty("Image", "set:" + imagesetname + " image:full_image");
			}
		}
	}
}

/***********************************************************
on window move
***********************************************************/
bool ShortcutBox::onWindowMove(const CEGUI::EventArgs& pEventArgs)
{

	if(_moving)
	{
		using namespace CEGUI;
		const MouseEventArgs &mouseEventArgs = static_cast<const MouseEventArgs&>(pEventArgs);
		Vector2 localMousePos = CoordConverter::screenToWindow(*_myBox, mouseEventArgs.position);
	   
		Vector2 offset(localMousePos - *mMousePosInWindow);

		UVector2 uOffset(cegui_absdim(PixelAligned(offset.d_x)),
						 cegui_absdim(PixelAligned(offset.d_y)));

		_myBox->setPosition(_myBox->getPosition() + uOffset );

		return true;
	}


	if(_moving_stances)
	{
		using namespace CEGUI;
		const MouseEventArgs &mouseEventArgs = static_cast<const MouseEventArgs&>(pEventArgs);
		Vector2 localMousePos = CoordConverter::screenToWindow(*_myStances, mouseEventArgs.position);
	   
		Vector2 offset(localMousePos - *mMousePosInWindow_stances);

		UVector2 uOffset(cegui_absdim(PixelAligned(offset.d_x)),
						 cegui_absdim(PixelAligned(offset.d_y)));

		_myStances->setPosition(_myStances->getPosition() + uOffset );

		return true;
	}

	return false;
}



/***********************************************************
handle windows resize event
***********************************************************/
bool ShortcutBox::HandleObjectPressed (const CEGUI::EventArgs& e)
{
	using namespace CEGUI;
	const MouseEventArgs mouseEventArgs = static_cast<const MouseEventArgs&>(e);
	*mMousePosInWindow = CEGUI::CoordConverter::screenToWindow(*_myBox, mouseEventArgs.position);
	_moving = true;
	return true;
}

/***********************************************************
handle windows resize event
***********************************************************/
bool ShortcutBox::HandleObjectReleased (const CEGUI::EventArgs& e)
{
	_moving = false;
	return true;
}


/***********************************************************
handle windows resize event
***********************************************************/
bool ShortcutBox::HandleObjectPressedStances (const CEGUI::EventArgs& e)
{
	using namespace CEGUI;
	const MouseEventArgs mouseEventArgs = static_cast<const MouseEventArgs&>(e);
	*mMousePosInWindow_stances = CEGUI::CoordConverter::screenToWindow(*_myStances, mouseEventArgs.position);
	_moving_stances = true;
	return true;
}

/***********************************************************
handle windows resize event
***********************************************************/
bool ShortcutBox::HandleObjectReleasedStances (const CEGUI::EventArgs& e)
{
	_moving_stances = false;
	return true;
}


/***********************************************************
refresh visible part depending of the visibility number
***********************************************************/
void ShortcutBox::RefreshVivsibleStuff()
{
	switch(_currentvisibility)
	{
		case 0:
			_myBox->show();
			_myStances->show();
		break;
		case 1:
			_myBox->show();
			_myStances->hide();
		break;
		case 2:
			_myBox->hide();
			_myStances->show();
		break;
		case 3:
			_myBox->hide();
			_myStances->hide();
		break;
	}
}


/***********************************************************
handle windows enter event
***********************************************************/
bool ShortcutBox::HandleInventoryEnter (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	unsigned int id = dd_args.window->getID();
	std::map<unsigned int, long>::iterator itm = _map_box_itemid.find(id);
	if(itm != _map_box_itemid.end())
	{
		CEGUI::Window* tmp = dd_args.window;
		std::string ttip = tmp->getProperty("Tooltip").c_str();
		if(ttip == "")
		{
			CEGUI::String tmpstr((const unsigned char *)InventoryHandler::getInstance()->GetItemDescription(itm->second).c_str());
			tmp->setProperty("Tooltip", tmpstr);
		}
	}

	return true;
}

