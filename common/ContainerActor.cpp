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

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#endif

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
#ifndef _LBANET_SERVER_SIDE_
	ThreadSafeWorkpile::getInstance()->OpenCloseContainer(_ID, false);
#endif
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
		it3.id = 8;
		it3.number = 2;
		it3.probability = 0.35f;
		gr.groupelements.push_back(it3);

		ItemGroupElement it4;
		it4.id = 3;
		it4.number = 1;
		it4.probability = 0.05f;
		gr.groupelements.push_back(it4);

		_lootList.push_back(gr);
	}
}

/***********************************************************
get current container content
***********************************************************/
const std::map<long, int> &  ContainerActor::GetCurrentContent()
{
	CleanOldItems();
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
				for(; itelem != endelem; ++itelem)
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
void ContainerActor::UpdateContent(long itemid, int deltanumber, bool resettime)
{
	double currtime = IceUtil::Time::now().toMilliSecondsDouble();

	// if we add content
	if(deltanumber > 0)
	{
		std::map<long, int>::iterator itlocal = _currentContent.find(itemid);
		if(itlocal != _currentContent.end())
			itlocal->second += deltanumber;
		else
			_currentContent[itemid] = deltanumber;

		_currentContentTime[itemid] = currtime;
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
					if(resettime)
						_lootList[itlink->second].lastSpawningTime = 0;
					else
						_lootList[itlink->second].lastSpawningTime = IceUtil::Time::now().toMilliSecondsDouble();
					_linktolootlist.erase(itlink);
				}
			}
		}
	}
}


/***********************************************************
clean old items
***********************************************************/
void ContainerActor::CleanOldItems()
{
	double currtime = IceUtil::Time::now().toMilliSecondsDouble();

	std::map<long, double>::iterator it = _currentContentTime.begin();
	while(it != _currentContentTime.end())
	{
		if((currtime - it->second) > 172800000) // if object older than 48h - destroy
		{
			UpdateContent(it->first, -_currentContent[it->first], true);
			_currentContentTime.erase(it);
			it = _currentContentTime.begin();
		}
		else
			++it;
	}
}
