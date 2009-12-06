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

#if !defined(__LbaNetModel_1_InventoryHandler_h)
#define __LbaNetModel_1_InventoryHandler_h



#include <string>
#include <vector>
#include <list>
#include <map>
#include "WorldInfo.h"
#include <IceUtil/Mutex.h>

struct ActionFromInventory
{
	bool ChangeStance;
	int NewStance;
	bool ChangeWeapon;
	int NewWeapon;
};

class LetterEditorBox;
class LetterViewerBox;
class NPCDialogBox;

/***********************************************************************
 * Module:  InventoryHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class InventoryHandler
 ***********************************************************************/
class InventoryHandler
{
public:

	// singleton pattern
	static InventoryHandler * getInstance();

	// check if the item is created by user
	static bool InventoryItemIsUserCreated(long id)
	{
		return (id >= 10000000);
	}


	//! destructor
	~InventoryHandler();

	//! set inventory database
	void SetInventoryDb(const std::map<long, ItemInfo> & db);

	// called when and object from the inventory is used
	ActionFromInventory ItemUsed(long ObjectId, bool LifeFull, bool ManaFull);


	//! set current inventory
	void SetCurrentInventory(const std::map<long, std::pair<int, int> > & db, int inventorysize);

	//! get current inventory
	std::map<long, std::pair<int, int> > GetCurrentInventory(int &inventorysize);

	//! update item position in inventory
	void UpdateItemPosition(long ObjectId, int position);

	//! gui ask if inventory has updated
	bool HasInventoryUpdated();

	//! gui ask if inventory has updated
	bool HasInventoryUpdatedForContainer();

	//! get item info
	std::string GetItemDescription(long ObjectId);

	//! set shorcut
	void SetShortcut(const std::vector<long> & shorcut);

	//! set shorcut
	std::vector<long> GetShortcut();

	//! gui ask if inventory has updated
	bool HasShorcutUpdated();

	//! set shorcut
	void SetShortcut(int place, long itid );

	//! get updated items
	void GetUpdatedItem(std::vector<std::pair<long, int> > & items);

	//! update inventory item
	void UpdateInventoryItem(long ObjectId, int NewCount);

	//! get inventory size
	int GetInventorySize();

	//! get inventory
	std::vector<std::pair<long, int> > GetInventoryVector();

	//! get the max number an item can have given its ID
	int GetItemMax(long id);

	//! get the type of item
	int GetItemType(long id);

	//! set letter editor
	void SetLetterEditor(LetterEditorBox * leditor);

	//! set letter editor
	void SetLetterViewer(LetterViewerBox * lviewer);

	//! update db with info
	void UpdateUserCreatedItemInfo(long Id, const std::string & from, const std::string & date,
									const std::string & subject, const std::string & message);

	//! get the number of item in inventory
	int GetItemNumber(long id);

	//! get the price of item
	int GetItemPrice(long id);

	//! buy item
	void BuyItem(long actorid, long itemid);
	
	//! get money item id
	long GetMoneyItemId()
	{return 8;}

protected:
	//! constructor
	InventoryHandler();


private:
	std::map<long, ItemInfo>				_db;
	std::map<long, std::pair<int, int> >	_currInventory;
	std::vector<long>						_shorcuts;
	int										_inventorysize;

	bool									_inventoryUpdated;
	bool									_shorcutUpdated;
	bool									_inventoryUpdatedContainer;


	IceUtil::Mutex							m_mutex_inv;
	IceUtil::Mutex							m_mutex_shortcut;

	std::vector<std::pair<long, int> >		m_updated_items;

	LetterEditorBox *						m_leditor;
	LetterViewerBox *						m_lviewer;
	NPCDialogBox *							m_ldialog;

	// singleton
	static InventoryHandler *				_singletonInstance;
};

#endif