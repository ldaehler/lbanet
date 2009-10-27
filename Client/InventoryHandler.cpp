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
#include "InventoryHandler.h"
#include "ImageSetHandler.h"
#include "ThreadSafeWorkpile.h"

InventoryHandler* InventoryHandler::_singletonInstance = NULL;


/***********************************************************
singleton pattern
***********************************************************/
InventoryHandler * InventoryHandler::getInstance()
{
    if(!_singletonInstance)
        _singletonInstance = new InventoryHandler();

	return _singletonInstance;
}


/***********************************************************
	Constructor
***********************************************************/
InventoryHandler::InventoryHandler()
: _inventoryUpdated(false), _shorcutUpdated(false)
{

}


/***********************************************************
	Destructor
***********************************************************/
InventoryHandler::~InventoryHandler()
{

}


/***********************************************************
set inventory database
***********************************************************/
void InventoryHandler::SetInventoryDb(const std::map<long, ItemInfo> & db)
{
	_db = db;
	std::vector<std::string> files;
	std::map<long, ItemInfo>::iterator it =	_db.begin();
	std::map<long, ItemInfo>::iterator end = _db.end();
	for(; it != end; ++it)
		files.push_back(it->second.filename);

	ImageSetHandler::GetInstance()->SetInventoryFiles(files);
}


/***********************************************************
called when and object from the inventory is used
***********************************************************/
ActionFromInventory InventoryHandler::ItemUsed(long ObjectId, bool LifeFull, bool ManaFull)
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);

	ActionFromInventory res;
	res.ChangeStance = false;
	res.ChangeWeapon = false;

	std::map<long, std::pair<int, int> >::iterator it = _currInventory.find(ObjectId);

	// if we do not have this object in inventory - return without doing any thing
	if(it == _currInventory.end())
		return res;

	std::map<long, ItemInfo>::iterator itdb =	_db.find(ObjectId);

	// if item not in db - return without doing any thing
	if(itdb == _db.end())
		return res;

	// do action depending of the item type
	switch(itdb->second.type)
	{
		case 1: // consummable item - inform the server and decrease count
			{
				if(LifeFull && itdb->second.valueA == 1)
					return res; // do not take potion since we have full life

				if(ManaFull && itdb->second.valueA == 2)
					return res; // do not take potion since we have full mana

				if(LifeFull && ManaFull && itdb->second.valueA == 3)
					return res; // do not take potion since we have full life & mana

				//inform the server
				ThreadSafeWorkpile::getInstance()->InventoryObjectUsed(ObjectId);
			}
			break;
		case 2: // key item - dont do anything
			break;
		case 3: // monture item - ride it
			res.ChangeStance = true;
			res.NewStance = itdb->second.valueA;
			break;
		case 4: // weapon item - equip it
			res.ChangeWeapon = true;
			res.NewWeapon = itdb->second.valueA;
			break;
		case 5: // quest item - dont do anything
			break;
		case 6: // other item - dont do anything
			break;
	}

	return res;
}



/***********************************************************
set current inventory
***********************************************************/
void InventoryHandler::SetCurrentInventory(const std::map<long, std::pair<int, int> > & db, int inventorysize)
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);

	_currInventory = db;
	_inventoryUpdated = true;
	_inventorysize = inventorysize;
}

/***********************************************************
get current inventory
***********************************************************/
std::map<long, std::pair<int, int> > InventoryHandler::GetCurrentInventory(int &inventorysize)
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);
	inventorysize = _inventorysize;
	return _currInventory;
}


/***********************************************************
update item position in inventory
***********************************************************/
void InventoryHandler::UpdateItemPosition(long ObjectId, int position)
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);

	std::map<long, std::pair<int, int> >::iterator it = _currInventory.find(ObjectId);
	if(it != _currInventory.end())
		it->second.second = position;
}


/***********************************************************
gui ask if inventory has updated
***********************************************************/
bool InventoryHandler::HasInventoryUpdated()
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);

	bool res = _inventoryUpdated;
	_inventoryUpdated = false;
	return res;
}



/***********************************************************
get item info
***********************************************************/
std::string InventoryHandler::GetItemDescription(long ObjectId)
{
	std::map<long, ItemInfo>::iterator itdb = _db.find(ObjectId);	
	if(itdb != _db.end())
		return itdb->second.Description;
	else
		return "";
}


/***********************************************************
set shorcut
***********************************************************/
void InventoryHandler::SetShortcut(const std::vector<long> & shorcut)
{
	IceUtil::Mutex::Lock lock(m_mutex_shortcut);
	_shorcuts = shorcut;
	_shorcutUpdated = true;
}

/***********************************************************
set shorcut
***********************************************************/

std::vector<long> InventoryHandler::GetShortcut()
{
	IceUtil::Mutex::Lock lock(m_mutex_shortcut);
	return _shorcuts;
}

/***********************************************************
gui ask if inventory has updated
***********************************************************/
bool InventoryHandler::HasShorcutUpdated()
{
	IceUtil::Mutex::Lock lock(m_mutex_shortcut);

	bool res = _shorcutUpdated;
	_shorcutUpdated = false;
	return res;
}


/***********************************************************
set shorcut
***********************************************************/
void InventoryHandler::SetShortcut(int place, long itid )
{
	IceUtil::Mutex::Lock lock(m_mutex_shortcut);
	_shorcuts[place] = itid;
}


/***********************************************************
get updated items
***********************************************************/
void InventoryHandler::GetUpdatedItem(std::vector<std::pair<long, int> > & items)
{
	items.clear();
	items.swap(m_updated_items);
}


/***********************************************************
update inventory item
***********************************************************/
void InventoryHandler::UpdateInventoryItem(long ObjectId, int NewCount)
{
	IceUtil::Mutex::Lock lock(m_mutex_inv);
	m_updated_items.push_back(std::make_pair<long, int>(ObjectId, NewCount));

	std::map<long, std::pair<int, int> >::iterator it = _currInventory.find(ObjectId);
	if(it == _currInventory.end())
	{
		//update item count
		it->second.first = NewCount;

		if(it->second.first  <= 0)
			_currInventory.erase(it);
	}
	else
	{
		_currInventory[ObjectId] = std::make_pair<int, int>(NewCount, -1);
	}
}