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


#include "ContainerActor.h"
#include <IceUtil/Time.h>
#include "Randomizer.h"

/***********************************************************
	Constructor
***********************************************************/
ContainerActor::ContainerActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ)
: ZoneActivableActor(ZoneSizeX, ZoneSizeY, ZoneSizeZ)
{

}


/***********************************************************
	Destructor
***********************************************************/
ContainerActor::~ContainerActor()
{

}


/***********************************************************
process zone activation
***********************************************************/
void ContainerActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
	float PlayerRotation)
{

}



/***********************************************************
set container loot list
***********************************************************/
void ContainerActor::SetLootList(const std::vector<ItemGroup> &  newList)
{
	_lootList = newList;

	if(_lootList.size() == 0)
	{
		ItemGroup gr;
		gr.lastSpawningTime = 0;
		gr.RespawningTime = 300000;
		gr.currpicked = -1;


		ItemGroupElement it1;
		it1.id = 1;
		it1.number = 1;
		it1.probability = 0.3f;
		gr.groupelements.push_back(it1);

		ItemGroupElement it2;
		it2.id = 2;
		it2.number = 1;
		it2.probability = 0.3f;
		gr.groupelements.push_back(it2);

		ItemGroupElement it3;
		it3.id = 3;
		it3.number = 2;
		it3.probability = 0.4f;
		gr.groupelements.push_back(it3);
	}
}

/***********************************************************
get current container content
***********************************************************/
const std::map<long, int> &  ContainerActor::GetCurrentContent()
{
	double currtime = IceUtil::Time::now().toMilliSecondsDouble();

	// go trough the loot table and populate the current content if necessary
	std::vector<ItemGroup>::iterator it = _lootList.begin();
	std::vector<ItemGroup>::iterator end = _lootList.end();
	for(int cc=0; it != end; ++it, ++cc)
	{
		// if the item group has been looted and need respawn
		if(it->currpicked < 0)
		{
			double timediff = currtime - it->lastSpawningTime;
			if(timediff > it->RespawningTime)
			{
				long itemdid = -1;
				int number = 0;

				// ramdomly choose the item to spawn
				double rnd = Randomizer::getInstance()->Rand();
				double currappend = 0;
				std::vector<ItemGroupElement>::iterator itelem = it->groupelements.begin();
				std::vector<ItemGroupElement>::iterator endelem = it->groupelements.end();
				for(; it != end; ++it)
				{
					itemdid = itelem->id;
					number = itelem->number;

					currappend += itelem->probability;
					if(currappend >= rnd)
						break;
				}

				if(itemdid >= 0)
				{
					// update container content
					UpdateContent(itemdid, number);

					//create a link between loot list and item content
					_linktolootlist[itemdid] = cc;
					it->currpicked = itemdid;
				}
			}
		}
	}

	return _currentContent;
}

/***********************************************************
update container content
***********************************************************/
void ContainerActor::UpdateContent(long itemid, int deltanumber)
{
	// if we add content
	if(deltanumber > 0)
	{
		std::map<long, int>::iterator itlocal = _currentContent.find(itemid);
		if(itlocal != _currentContent.end())
			itlocal->second += deltanumber;
		else
			_currentContent[itemid] = deltanumber;
	}
	else // if we remove content
	{
		std::map<long, int>::iterator itlocal = _currentContent.find(itemid);
		if(itlocal != _currentContent.end())
		{
			itlocal->second += deltanumber;
			if(itlocal->second <= 0)
			{
				// remove link
				_currentContent.erase(itlocal);

				// remove link to spawn list to tell that the item has been looted
				std::map<long, int>::iterator itlink = _linktolootlist.find(itemid);
				if(itlink != _linktolootlist.end())
				{
					_lootList[itlink->second].currpicked = -1;
					_linktolootlist.erase(itlink);
				}
			}
		}
	}
}
