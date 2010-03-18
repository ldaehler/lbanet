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

#if !defined(__LbaNetModel_1_LbaNetModel_h)
#define __LbaNetModel_1_LbaNetModel_h

#include <string>
#include <vector>
#include <map>

#include "ObjectsDescription.h"
#include "DynamicObject.h"
#include <boost/shared_ptr.hpp>

class PhysXEngine;


/***********************************************************************
 * Module:  LbaNetModel.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 13:54:52
 * Purpose: Declaration of the class LbaNetModel
 ***********************************************************************/
class LbaNetModel
{
public:
	//! constructor
	LbaNetModel();

	//! destructor
	~LbaNetModel();


	//! set physic engine
	void SetPhysicEngine(boost::shared_ptr<PhysXEngine> & pEngine);

	//! do all check to be done when idle
	void Process();

	//! reset model with a new map
	void SetMap(const ObjectInfo &mapInfo, const LbaMainLightInfo &NewLightningInfo);

	//! add object to the scene
	void AddObject(long id, const ObjectInfo &desc);

	//! remove object from the scene
	void RemObject(long id);

	//! get object from the scene
	boost::shared_ptr<DynamicObject> GetObject(long id);

protected:
	//! clear current model before changing map
	void ClearModel(const LbaMainLightInfo &NewLightningInfo);

private:
	std::map<long, boost::shared_ptr<DynamicObject> >	_dynamicObjects;
	boost::shared_ptr<PhysXEngine>						_physicEngine;

	boost::shared_ptr<DynamicObject>					_currMap;
};

#endif