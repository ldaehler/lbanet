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

#include "LbaNetModel.h"
#include "LogHandler.h"
#include "PhysXEngine.h"
#include "OSGHandler.h"


/***********************************************************
	Constructor
***********************************************************/
LbaNetModel::LbaNetModel()
{
	LogHandler::getInstance()->LogToFile("Initializing model class...");

}


/***********************************************************
	Destructor
***********************************************************/
LbaNetModel::~LbaNetModel()
{
	// clear model
	ClearModel(LbaMainLightInfo());
}



/***********************************************************
set physic engine
***********************************************************/
void LbaNetModel::SetPhysicEngine(boost::shared_ptr<PhysXEngine> & pEngine)
{
	_physicEngine = pEngine;
}


/***********************************************************
do all check to be done when idle
***********************************************************/
void LbaNetModel::Process()
{
	// process all dynamic objects
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.begin();
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator end = _dynamicObjects.end();
	for(; it != end; ++it)
		it->second->Process();
}





/***********************************************************
reset model with a new map
***********************************************************/
void LbaNetModel::SetMap(const ObjectInfo &mapInfo, const LbaMainLightInfo &NewLightningInfo)
{
	// clear previous map if there was one
	ClearModel(NewLightningInfo);

	_currMap = mapInfo.BuildSelf(_physicEngine);
}


/***********************************************************
clear current model before changing map
***********************************************************/
void LbaNetModel::ClearModel(const LbaMainLightInfo &NewLightningInfo)
{
	//clear dynamic object of the current scene
	_dynamicObjects.clear();

	// clear current map
	_currMap.reset();


	// clear physic engine
	if(_physicEngine)
		_physicEngine->Clear();


	//clear display engine
	OsgHandler::getInstance()->ResetDisplayTree(NewLightningInfo);
}



/***********************************************************
add object to the scene
***********************************************************/
void LbaNetModel::AddObject(long id, const ObjectInfo &desc)
{
	// remove object of same id if already there
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
		RemObject(id);

	_dynamicObjects[id] = desc.BuildSelf(_physicEngine);
}

/***********************************************************
remove object from the scene
***********************************************************/
void LbaNetModel::RemObject(long id)
{
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
	{
		it->second->Destroy();
		_dynamicObjects.erase(it);
	}
}



/***********************************************************
remove object from the scene
***********************************************************/
boost::shared_ptr<DynamicObject> LbaNetModel::GetObject(long id)
{
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
	{
		return it->second;
	}

	return boost::shared_ptr<DynamicObject>();
}
