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
#include "GameEvents.h"
#include "InventoryHandler.h"
#include "SynchronizedTimeHandler.h"
#include "MessageBox.h"


#define _NB_BOX_CONTAINER_ 12

/***********************************************************
constructor
***********************************************************/
ContainerBox::ContainerBox(GameGUI * gamgui, int boxsize)
: _gamgui(gamgui), _boxsize(boxsize), _inventory_size(0)
{
	_currContainer.ContainerId = -1;
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
			CEGUI::Event::Subscriber (&ContainerBox::HandleCancel, this));


		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame/OK"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ContainerBox::HandleOk, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame/TakeAll"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ContainerBox::HandleTakeAll, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame/Cancel"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ContainerBox::HandleCancel, this));


		CEGUI::Window*	pane = CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame/ConScrollable");
		CEGUI::Window*	tmpwindow;
		for(int i=0; i<_NB_BOX_CONTAINER_; ++i)
		{
			int x = i / 3;
			int y = i % 3;

			tmpwindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
			tmpwindow->setArea(CEGUI::UDim(0,5+(_boxsize+2)*y), CEGUI::UDim(0,5+(_boxsize+2)*x), 
								CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize));
			pane->addChildWindow(tmpwindow);

			tmpwindow->subscribeEvent(
						CEGUI::Window::EventDragDropItemDropped,
						CEGUI::Event::Subscriber(&ContainerBox::handle_ItemDroppedInContainer, this));

			tmpwindow->setID(i);
			_cont_boxes.push_back(tmpwindow);
		}



		frw->show();
		_myBox->hide();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init ContainerBox: ") + ex.getMessage().c_str());
	}
}



	
/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ContainerBox::HandleOk (const CEGUI::EventArgs& e)
{
	CloseAndUpdate();
	return true;
}
	
/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ContainerBox::HandleTakeAll (const CEGUI::EventArgs& e)
{
	TakeAll();
	CloseAndUpdate();
	return true;
}

/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ContainerBox::HandleCancel (const CEGUI::EventArgs& e)
{
	Cancel();
	CloseAndUpdate();
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
drag and drop
***********************************************************/
bool ContainerBox::handle_ItemDroppedInContainer(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);

	// make sure we drop a valid item
	if((dd_args.dragDropItem->getChildCount() > 1) && dd_args.dragDropItem->getChildAtIdx(1)->getID() == 2)
	{
		switchfrominventorytocontainer(dd_args.dragDropItem->getID(), true);
	}

    return true;
}


/***********************************************************
drag and drop
***********************************************************/
bool ContainerBox::handle_ItemDroppedInInventory(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);

	// make sure we drop a valid item
	if((dd_args.dragDropItem->getChildCount() > 1) && dd_args.dragDropItem->getChildAtIdx(1)->getID() == 3)
	{
		switchfromcontainertoinventory(dd_args.dragDropItem->getID(), true);
	}

    return true;
}

/***********************************************************
drag and drop
***********************************************************/
bool ContainerBox::handle_ItemDroppedInContainerItem(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);

	// make sure we drop a valid item
	if((dd_args.dragDropItem->getChildCount() > 1) && dd_args.dragDropItem->getChildAtIdx(1)->getID() == 2)
	{
		switchfrominventorytocontainer(dd_args.dragDropItem->getID(), true);
	}

    return true;
}


/***********************************************************
drag and drop
***********************************************************/
bool ContainerBox::handle_ItemDroppedInInventoryItem(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);

	// make sure we drop a valid item
	if((dd_args.dragDropItem->getChildCount() > 1) && dd_args.dragDropItem->getChildAtIdx(1)->getID() == 3)
	{
		switchfromcontainertoinventory(dd_args.dragDropItem->getID(), true);
	}

    return true;
}





/***********************************************************
object clicked
***********************************************************/
bool ContainerBox::HandleContainerItemClicked (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	// use object
	if(dd_args.button == CEGUI::RightButton)
	{
		switchfromcontainertoinventory(dd_args.window->getID(), false);
	}

    return true;
}


/***********************************************************
object clicked
***********************************************************/
bool ContainerBox::HandleInventoryItemClicked (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	// use object
	if(dd_args.button == CEGUI::RightButton)
	{
		switchfrominventorytocontainer(dd_args.window->getID(), false);
	}

    return true;
}


/***********************************************************
process what is needed in the game GUI
***********************************************************/
void ContainerBox::Process()
{
	// for new opened container
	LbaNet::ContainerInfo cinfo;
	if(ThreadSafeWorkpile::getInstance()->IsUpdatedContainer(cinfo))
	{
		if(_currContainer.ContainerId != cinfo.ContainerId)
		{
			// if a container is already open then close it
			if(_currContainer.ContainerId >= 0)
			{
				Cancel();
				CloseAndUpdate();
			}

			// if the container is locked by our player
			if(cinfo.LockedById == ThreadSafeWorkpile::getInstance()->GetPlayerId())
			{
				// store opening time
				_last_opened_time = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();

				_currContainer = cinfo;
				_currContainerCopy = cinfo;
				_serverupdate = ThreadSafeWorkpile::UpdateInvContainer();
				_serverupdate.containerid = cinfo.ContainerId;

				// update container part
				CleanContainer();
				LbaNet::ItemList::const_iterator itm = cinfo.Content.begin();
				LbaNet::ItemList::const_iterator endm = cinfo.Content.end();
				for(int cc=0;itm != endm && cc<_NB_BOX_CONTAINER_; ++itm, ++cc)
					AddContainerItem(itm->first, itm->second, _cont_boxes[cc]);


				//update inventory part
				RefreshInventory();

				Show();
			}
			else // if not tell the user the container is locked
			{
				CGMessageBox::getInstance()->Show("Container", "Container already in use by another player.");
			}
		}
	}

	// if container closed
	long containerid;
	bool forced;
	if(ThreadSafeWorkpile::getInstance()->HasOpenCloseContainer(containerid, forced))
	{
		if(forced)
		{
			Cancel();
			CloseAndUpdate();
		}
		else
		{
			if(_currContainer.ContainerId == containerid)
			{
				TakeAll();			
				CloseAndUpdate();
			}
			else
				ThreadSafeWorkpile::getInstance()->AskForContainerInfo(containerid);
		}
	}


	// if inventory updated
	if(InventoryHandler::getInstance()->HasInventoryUpdatedForContainer())
		RefreshInventory();


	//if container opened since more than 4min then close it
	if(_currContainer.ContainerId >= 0)
		if((SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble() - _last_opened_time) > 240000)
		{
			Cancel();
			CloseAndUpdate();
		}
}



/***********************************************************
close box and send update to server
***********************************************************/
void ContainerBox::CloseAndUpdate()
{
	_myBox->hide();
	if(_currContainer.ContainerId >= 0)
		ThreadSafeWorkpile::getInstance()->UpdateInvFromContainer(_serverupdate);

	_currContainer.ContainerId = -1;
}


/***********************************************************
cancel changes
***********************************************************/
void ContainerBox::Cancel()
{
	_serverupdate.Taken.clear();
	_serverupdate.Put.clear();
}

/***********************************************************
take all item from container
***********************************************************/
void ContainerBox::TakeAll()
{
	Cancel();
	_serverupdate.Taken = _currContainerCopy.Content;
}



/***********************************************************
add an item inside the container
***********************************************************/
void ContainerBox::AddContainerItem(long Id, int number, CEGUI::Window* parent)
{
	_cont_objects[Id] = AddInventoryItem(Id, number, parent, true);
}




/***********************************************************
add an item inside the container
***********************************************************/
std::pair<CEGUI::Window*, CEGUI::Window*> ContainerBox::AddInventoryItem(long Id, int number, 
																		 CEGUI::Window* parent, 
																		bool tocontainer)
{
	CEGUI::Window*	tmp = CEGUI::WindowManager::getSingleton().createWindow("DragContainer");
	tmp->setArea(CEGUI::UDim(0,0), CEGUI::UDim(0,0), CEGUI::UDim(1, 0), CEGUI::UDim(1, 0));
	tmp->setID(Id);

	CEGUI::Window*	tmp2 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	tmp2->setArea(CEGUI::UDim(0,5), CEGUI::UDim(0,12), CEGUI::UDim(0, _boxsize-10), CEGUI::UDim(0, _boxsize-20));

	std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(Id);
	tmp2->setProperty("Image", "set:" + imagesetname + " image:full_image");
	tmp2->setProperty("MousePassThroughEnabled", "True");



	CEGUI::Window*	tmp3 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	tmp3->setArea(CEGUI::UDim(0,2), CEGUI::UDim(0,4), CEGUI::UDim(1, -1), CEGUI::UDim(0, 14));
	tmp3->setProperty("FrameEnabled", "False");
	tmp3->setProperty("BackgroundEnabled", "False");
	tmp3->setProperty("MousePassThroughEnabled", "True");
	tmp3->setAlwaysOnTop(true);
	tmp3->setProperty("Font" , "contourfont");

	std::stringstream strs;
	strs<<number;
	tmp3->setText(strs.str().c_str());
	if(tocontainer)
		tmp3->setID(3);
	else
		tmp3->setID(2);


	CEGUI::String tmpstr((const unsigned char *)InventoryHandler::getInstance()->GetItemDescription(Id).c_str());
	tmp->setProperty("Tooltip", tmpstr);
	tmp->addChildWindow(tmp2);
	tmp->addChildWindow(tmp3);
	parent->addChildWindow(tmp);


    tmp->subscribeEvent(
		CEGUI::Window::EventMouseEnters,
				CEGUI::Event::Subscriber(&ContainerBox::HandleInventoryEnter, this));


	if(tocontainer)
	{
		tmp->subscribeEvent(
					CEGUI::Window::EventDragDropItemDropped,
					CEGUI::Event::Subscriber(&ContainerBox::handle_ItemDroppedInContainerItem, this));
	
		tmp->subscribeEvent (CEGUI::Window::EventMouseClick,
								CEGUI::Event::Subscriber (&ContainerBox::HandleContainerItemClicked, this));
	}
	else
	{
		tmp->subscribeEvent(
					CEGUI::Window::EventDragDropItemDropped,
					CEGUI::Event::Subscriber(&ContainerBox::handle_ItemDroppedInInventoryItem, this));
	
		tmp->subscribeEvent (CEGUI::Window::EventMouseClick,
								CEGUI::Event::Subscriber (&ContainerBox::HandleInventoryItemClicked, this));
	}


	return std::make_pair<CEGUI::Window*, CEGUI::Window*>(tmp, tmp3);
}



/***********************************************************
clean container items
***********************************************************/
void ContainerBox::CleanContainer()
{
	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator it = _cont_objects.begin();
	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator end = _cont_objects.end();
	for(;it != end; ++it)
		it->second.first->destroy();

	_cont_objects.clear();
}



/***********************************************************
resize inventory
***********************************************************/
void ContainerBox::ResizeInventory(int newsize)
{
	if(_inventory_size == newsize)
		return;

	_inventory_size = newsize;
	for(size_t i=0; i<_inv_boxes.size(); ++i)
		_inv_boxes[i]->destroy();

	_inv_boxes.clear();


	CEGUI::Window*	pane = CEGUI::WindowManager::getSingleton().getWindow("ContainerFrame/InvScrollable");
	CEGUI::Window*	tmpwindow;
	for(int i=0; i<_inventory_size; ++i)
	{
		int x = i / 4;
		int y = i % 4;

		tmpwindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
		tmpwindow->setArea(CEGUI::UDim(0,5+(_boxsize+2)*y), CEGUI::UDim(0,5+(_boxsize+2)*x),
							CEGUI::UDim(0, _boxsize), CEGUI::UDim(0, _boxsize));
		pane->addChildWindow(tmpwindow);

        tmpwindow->subscribeEvent(
					CEGUI::Window::EventDragDropItemDropped,
					CEGUI::Event::Subscriber(&ContainerBox::handle_ItemDroppedInInventory, this));

		tmpwindow->setID(i);
		_inv_boxes.push_back(tmpwindow);
	}

	CleanInventory();
}

/***********************************************************
update inventory
***********************************************************/
void ContainerBox::UpdateInventory(std::vector<std::pair<long, int> > inv)
{
	for(size_t i=0; i< _inventory_data.size(); ++i)
	{
		if(_inventory_windows[i].first != NULL)
		{
			if((inv[i].first < 0) || (inv[i].first != _inventory_data[i].first))
			{
				_inventory_windows[i].first->destroy();
				_inventory_windows[i].first = NULL;
				_inventory_windows[i].second = NULL;
			}
		}

		if((inv[i].first >= 0))
		{
			if(_inventory_windows[i].first == NULL)
				_inventory_windows[i] = AddInventoryItem(inv[i].first, inv[i].second, _inv_boxes[i], false);
			else
			{
				std::stringstream strs;
				strs<<inv[i].second;
				_inventory_windows[i].second->setText(strs.str().c_str());
			}
		}
	}

	_inventory_data = inv;
}


/***********************************************************
clean inventory
***********************************************************/
void ContainerBox::CleanInventory()
{
	std::vector<std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator it = _inventory_windows.begin();
	std::vector<std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator end = _inventory_windows.end();
	for(;it != end; ++it)
		it->first->destroy();

	_inventory_windows.clear();
	_inventory_data.clear();

	for(int i=0; i<_inventory_size; ++i)
	{
		_inventory_windows.push_back(std::make_pair<CEGUI::Window*, CEGUI::Window*>(NULL, NULL));
		_inventory_data.push_back(std::make_pair<long, int>(-1, 0));
	}
}


/***********************************************************
refresh inventory
***********************************************************/
void ContainerBox::RefreshInventory()
{
	ResizeInventory(InventoryHandler::getInstance()->GetInventorySize());
	UpdateInventory(InventoryHandler::getInstance()->GetInventoryVector());
}



/***********************************************************
add an item from container to inventory
return the number taken
***********************************************************/
int ContainerBox::AddItemFromContainerToInventory(long Id, int number)
{
	int itemplace = 0;
	// check if we already have an item of this type
	for(; itemplace < (int)_inventory_data.size(); ++itemplace)
	{
		if(_inventory_data[itemplace].first == Id)
			break;
	}

	// if not check for an empty space in inventory
	if(itemplace == (int)_inventory_data.size())
	{
		// look for an empty space
		for(itemplace=0; itemplace < (int)_inventory_data.size(); ++itemplace)
		{
			if(_inventory_data[itemplace].first == -1)
				break;
		}

		// return if inventory is full
		if(itemplace == (int)_inventory_data.size())
			return 0;

		// add the item in the empty space
		_inventory_data[itemplace].first = Id;
		_inventory_windows[itemplace] = AddInventoryItem(Id, 0, _inv_boxes[itemplace], false);
	}

	int max = InventoryHandler::getInstance()->GetItemMax(Id);
	int current = _inventory_data[itemplace].second;
	int taken = std::min((max-current), number);

	if(taken > 0)
	{
		_inventory_data[itemplace].second = (current+taken);
		std::stringstream strs;
		strs<<(current+taken);
		_inventory_windows[itemplace].second->setText(strs.str().c_str());
	}

	return taken;
}



/***********************************************************
add an item from container to inventory
return the number taken
***********************************************************/
int ContainerBox::AddItemFromInventoryToContainer(long Id, int number)
{
	// check if the type is allowed to be dropped
	int type = InventoryHandler::getInstance()->GetItemType(Id);
	if(type != 1 && type != 6 && type != 8)
		return 0;


	// check if we already have an item of this type
	LbaNet::ItemList::iterator itc = _currContainer.Content.find(Id);
	if(itc == _currContainer.Content.end())
	{
		// if not check for an empty space in inventory	
		CEGUI::Window*  parent = FindFirstContainerEmptySpace();

		// if container full then return
		if(parent == NULL)
			return 0;

		_currContainer.Content[Id] = 0;
		itc = _currContainer.Content.find(Id);

		// add container item
		AddContainerItem(Id, 0, parent);
	}


	// update numbers
	int max = InventoryHandler::getInstance()->GetItemMax(Id);
	int current = itc->second;
	int taken = std::min((max-current), number);

	if(taken > 0)
	{
		itc->second = (current+taken);
		std::stringstream strs;
		strs<<(current+taken);
		_cont_objects[Id].second->setText(strs.str().c_str());
	}

	return taken;
}


/***********************************************************
switch item from container to inventory
***********************************************************/
void ContainerBox::switchfromcontainertoinventory(long Id, bool full)
{
	LbaNet::ItemList::iterator itcontent = _currContainer.Content.find(Id);
	if(itcontent != _currContainer.Content.end())
	{
		int taken = AddItemFromContainerToInventory(Id, (full ? itcontent->second : 1));
		if(taken > 0)
		{
			std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator itcont = 
																_cont_objects.find(itcontent->first);
			itcontent->second -= taken;
			if(itcontent->second <= 0)
			{
				if(itcont != _cont_objects.end())
				{
					itcont->second.first->destroy();
					_cont_objects.erase(itcont);
				}
				_currContainer.Content.erase(itcontent);
			}
			else
			{
				if(itcont != _cont_objects.end())
				{	
					std::stringstream strs;
					strs<<itcontent->second;
					itcont->second.second->setText(strs.str().c_str());
				}
			}

			UpdateTakenPut(Id, -taken);
		}
	}
}


/***********************************************************
switch item from inventory to container
***********************************************************/
void ContainerBox::switchfrominventorytocontainer(long Id, bool full)
{
	std::vector<std::pair<long, int> >::iterator itinv = _inventory_data.begin();
	std::vector<std::pair<long, int> >::iterator endinv = _inventory_data.end();
	std::vector<std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator itinvwin = _inventory_windows.begin();
	for(; itinv != endinv; ++itinv, ++itinvwin)
	{
		if(itinv->first == Id)
		{
			int taken = AddItemFromInventoryToContainer(Id, (full ? itinv->second : 1));
			if(taken > 0)
			{
				itinv->second -= taken;
				if(itinv->second <= 0)
				{
					itinvwin->first->destroy();
					*itinvwin = std::make_pair<CEGUI::Window*, CEGUI::Window*>(NULL, NULL);
					*itinv = std::make_pair<long, int>(-1, 0);
				}
				else
				{
					std::stringstream strs;
					strs<<itinv->second;
					itinvwin->second->setText(strs.str().c_str());
				}

				UpdateTakenPut(Id, taken);
			}
		}
	}
}


/***********************************************************
update taken and put vectors with last change from user
***********************************************************/
void ContainerBox::UpdateTakenPut(long itid, int deltaupd)
{
	// nothing to do
	if(deltaupd == 0)
		return;

	// we take an item from container
	if(deltaupd < 0)
	{
		// check if we have same item in put vector
		LbaNet::ItemList::iterator itp = _serverupdate.Put.find(itid);
		if(itp != _serverupdate.Put.end())
		{
			int nb = itp->second;
			if(nb <= -deltaupd)
			{
				deltaupd += nb;
				_serverupdate.Put.erase(itp);
			}
			else
			{
				itp->second += deltaupd;
				deltaupd = 0;
			}

			// nothing more to do
			if(deltaupd == 0)
				return;
		}

		//now add item to taken part
		LbaNet::ItemList::iterator itt = _serverupdate.Taken.find(itid);
		if(itt != _serverupdate.Taken.end())
			itt->second += -deltaupd;
		else
			_serverupdate.Taken[itid] = -deltaupd;
	}
	else // we add an item to container
	{
		// check if we have same item in taken vector
		LbaNet::ItemList::iterator itt = _serverupdate.Taken.find(itid);
		if(itt != _serverupdate.Taken.end())
		{
			int nb = itt->second;
			if(nb <= deltaupd)
			{
				deltaupd -= nb;
				_serverupdate.Taken.erase(itt);
			}
			else
			{
				itt->second -= deltaupd;
				deltaupd = 0;
			}

			// nothing more to do
			if(deltaupd == 0)
				return;
		}

		//now add item to put part
		LbaNet::ItemList::iterator itp = _serverupdate.Put.find(itid);
		if(itp != _serverupdate.Put.end())
			itp->second += deltaupd;
		else
			_serverupdate.Put[itid] = deltaupd;
	}
}



/***********************************************************
find first empty space in inventory
***********************************************************/
CEGUI::Window*  ContainerBox::FindFirstContainerEmptySpace()
{
	for(size_t i=0; i<_cont_boxes.size(); ++i)
	{
		if(_cont_boxes[i]->getChildCount() == 2)
			return _cont_boxes[i];
	}

	return NULL;
}



/***********************************************************
refresh inventory images
***********************************************************/
void  ContainerBox::Refresh()
{
	if(_myBox->isVisible())
	{
		_myBox->hide();
		_myBox->show();
		_myBox->activate();
	}

	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator it = _cont_objects.begin();
	std::map<long, std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator end = _cont_objects.end();
	for(; it != end; ++it)
	{
		std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(it->first);
		it->second.first->getChildAtIdx(0)->setProperty("Image", "set:" + imagesetname + " image:full_image");
	}

	std::vector<std::pair<long, int> >::iterator itinvd = _inventory_data.begin();
	std::vector<std::pair<long, int> >::iterator endinvd = _inventory_data.end();
	std::vector<std::pair<CEGUI::Window*, CEGUI::Window*> >::iterator itinv = _inventory_windows.begin();
	for(; itinvd != endinvd; ++itinvd, ++itinv)
	{
		if(itinvd->first >= 0)
		{
			std::string imagesetname = ImageSetHandler::GetInstance()->GetInventoryImage(itinvd->first);
			itinv->first->getChildAtIdx(0)->setProperty("Image", "set:" + imagesetname + " image:full_image");
		}
	}
}


/***********************************************************
handle windows enter event
***********************************************************/
bool ContainerBox::HandleInventoryEnter (const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& dd_args = static_cast<const CEGUI::MouseEventArgs&>(e);

	unsigned int id = dd_args.window->getID();

	CEGUI::Window* tmp = dd_args.window;
	std::string ttip = tmp->getProperty("Tooltip").c_str();
	if(ttip == "")
	{
		CEGUI::String tmpstr((const unsigned char *)InventoryHandler::getInstance()->GetItemDescription(id).c_str());
		tmp->setProperty("Tooltip", tmpstr);
	}

	return true;
}
