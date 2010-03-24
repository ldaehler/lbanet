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

#include "ObjectsDescription.h"
#include "PhysXObjectHandlers.h"
#include "WorldToDisplayObjectSynchronizer.h"
#include "StaticObject.h"

/***********************************************************
	Constructor
***********************************************************/
DisplayTransformation::DisplayTransformation()
:translationX(0), translationY(0), translationZ(0),
	scaleX(1),	scaleY(1), scaleZ(1)
{
}

/***********************************************************
	destructor
***********************************************************/
DisplayTransformation::~DisplayTransformation()
{

}



/***********************************************************
	Constructor
***********************************************************/
DisplayInfo::DisplayInfo(boost::shared_ptr<DisplayTransformation> Tr, 
					boost::shared_ptr<DisplayObjectDescriptionBase> Ds)
	: Transform(Tr), DisplayDesc(Ds)
{

}


/***********************************************************
	Destructor
***********************************************************/
DisplayInfo::~DisplayInfo()
{

}



/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DisplayObjectHandlerBase> DisplayInfo::BuildSelf()
{
	if(DisplayDesc)
		return DisplayDesc->BuildSelf(Transform);
	
	return boost::shared_ptr<DisplayObjectHandlerBase>();
}




/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionBase::PhysicalDescriptionBase(float posX, float posY, float posZ, 
												 const LbaQuaternion &rot)
: 	positionX(posX), positionY(posY), positionZ(posZ), rotation(rot)
{

}


/***********************************************************
	Destructor
***********************************************************/
PhysicalDescriptionBase::~PhysicalDescriptionBase()
{

}





/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionNoShape::PhysicalDescriptionNoShape(float posX, float posY, float posZ, 
													   const LbaQuaternion &rot)
: PhysicalDescriptionBase(posX, posY, posZ, rot)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionNoShape::~PhysicalDescriptionNoShape()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionNoShape::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine) const
{
	return boost::shared_ptr<PhysicalObjectHandlerBase>( 
				new SimplePhysicalObjectHandler(positionX, positionY, positionZ, LbaQuaternion()));
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionWithShape::PhysicalDescriptionWithShape(float posX, float posY, float posZ,
															int Otype, float Odensity,
															const LbaQuaternion &rot)
: PhysicalDescriptionBase(posX, posY, posZ, rot), type(Otype), density(Odensity)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionWithShape::~PhysicalDescriptionWithShape()
{

}



/***********************************************************
	Constructor
***********************************************************/
ObjectInfo::ObjectInfo(boost::shared_ptr<DisplayInfo> DInfo,
				boost::shared_ptr<PhysicalDescriptionBase> PInfo,
				bool Static, bool NoSmoothing)
	: DisInfo(DInfo), PhysInfo(PInfo), ForceNoSmoothing(NoSmoothing),
		IsStatic(Static)
{

}

/***********************************************************
	destructor
***********************************************************/
ObjectInfo::~ObjectInfo()
{

}


/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DynamicObject> ObjectInfo::BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine, int id) const
{
	boost::shared_ptr<PhysicalObjectHandlerBase> phH;
	boost::shared_ptr<DisplayObjectHandlerBase> disH;
	if(PhysInfo)
		phH = PhysInfo->BuildSelf(_PEngine);
	if(DisInfo)
		disH = DisInfo->BuildSelf();

	if(!IsStatic && phH && disH)
		return boost::shared_ptr<DynamicObject>(new WorldToDisplayObjectSynchronizer(phH, disH, id, ForceNoSmoothing));
	else
		return boost::shared_ptr<DynamicObject>(new StaticObject(phH, disH, id));
}