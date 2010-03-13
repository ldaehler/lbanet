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
#include "PhysXEngine.h"
#include "NxVec3.h"

/***********************************************************
	Constructor
***********************************************************/
DisplayTransformation::DisplayTransformation()
:translationX(0), translationY(0), translationZ(0),
	rotationX(0), rotationY(0), rotationZ(0),
	scaleX(0),	scaleY(0), scaleZ(0)
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
	Constructor
***********************************************************/
PhysicalDescriptionBase::PhysicalDescriptionBase(float posX, float posY, float posZ)
: 	positionX(posX), positionY(posY), positionZ(posZ)
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
PhysicalDescriptionNoShape::PhysicalDescriptionNoShape(float posX, float posY, float posZ)
: PhysicalDescriptionBase(posX, posY, posZ)
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
												boost::shared_ptr<PhysXEngine> _PEngine)
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
: PhysicalDescriptionBase(posX, posY, posZ), type(Otype), density(Odensity), rotation(rot)
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
PhysicalDescriptionBox::PhysicalDescriptionBox(float posX, float posY, float posZ,
												int Otype, float Odensity,
												const LbaQuaternion &rot,
												float sX, float sY, float sZ)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
			sizeX(sX), sizeY(sY), sizeZ(sZ)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionBox::~PhysicalDescriptionBox()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionBox::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine)
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateBox(NxVec3(positionX, positionY, positionZ), sizeX, sizeY, sizeZ, 
													density, type, udata.get());
		if(type != 3)
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorHandler(_PEngine, udata, act, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
		}
		else
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXDynamicActorHandler(_PEngine, udata, act, rotation));
		}
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacterBox(NxVec3(positionX, positionY, positionZ), 
															NxVec3(sizeX, sizeY, sizeZ), udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionCapsule::PhysicalDescriptionCapsule(float posX, float posY, float posZ,
														int Otype, float Odensity,
														const LbaQuaternion &rot,
														float rad, float ht)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
		radius(rad), height(ht)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionCapsule::~PhysicalDescriptionCapsule()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionCapsule::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine)
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateCapsule(NxVec3(positionX, positionY, positionZ), radius, height,
													density, type, udata.get());
		if(type != 3)
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorHandler(_PEngine, udata, act, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
		}
		else
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXDynamicActorHandler(_PEngine, udata, act, rotation));
		}
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacter(NxVec3(positionX, positionY, positionZ), 
															radius, height, udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionSphere::PhysicalDescriptionSphere(float posX, float posY, float posZ,
														int Otype, float Odensity,
														const LbaQuaternion &rot,
														float rad)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
		radius(rad)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionSphere::~PhysicalDescriptionSphere()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionSphere::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine)
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateSphere(NxVec3(positionX, positionY, positionZ), radius, 
													density, type, udata.get());
		if(type != 3)
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorHandler(_PEngine, udata, act, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
		}
		else
		{
			return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXDynamicActorHandler(_PEngine, udata, act, rotation));
		}
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacter(NxVec3(positionX, positionY, positionZ), 
															radius, 0, udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionTriangleMesh::PhysicalDescriptionTriangleMesh(float posX, float posY, float posZ,
																	const std::string &FileName)
	:PhysicalDescriptionWithShape(posX, posY, posZ, 1, 0, LbaQuaternion()),
		MeshInfoDataFileName(FileName)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionTriangleMesh::~PhysicalDescriptionTriangleMesh()
{

}



/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionTriangleMesh::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine)
{


	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	NxActor* actor = _PEngine->LoadTriangleMeshFile(NxVec3(positionX, positionY, positionZ), MeshInfoDataFileName,
														udata);

	return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorHandler(_PEngine, udata, actor, 
							boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
}

/***********************************************************
	Constructor
***********************************************************/
ObjectInfo::ObjectInfo(boost::shared_ptr<DisplayInfo> DInfo,
				boost::shared_ptr<PhysicalDescriptionBase> PInfo)
	: DisInfo(DInfo), PhysInfo(PInfo)
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
boost::shared_ptr<DynamicObject> ObjectInfo::BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine)
{

}