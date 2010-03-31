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
constructor from stream
***********************************************************/
DisplayTransformation::DisplayTransformation(SerializerBase * stream)
{
	//get serialized translation
	translationX = stream->getFloat("translationX");
	translationY = stream->getFloat("translationY");
	translationZ = stream->getFloat("translationZ");

	//get serialized rotation
	rotation.X = stream->getFloat("rotationX");
	rotation.Y = stream->getFloat("rotationY");
	rotation.Z = stream->getFloat("rotationZ");
	rotation.W = stream->getFloat("rotationW");

	//get serialized scale
	scaleX = stream->getFloat("scaleX");
	scaleY = stream->getFloat("scaleY");
	scaleZ = stream->getFloat("scaleZ");
}


/***********************************************************
	destructor
***********************************************************/
DisplayTransformation::~DisplayTransformation()
{

}

/***********************************************************
serialize to network object
***********************************************************/
void DisplayTransformation::Serialize(SerializerBase * stream) const
{
	// serialize translation
	stream->addFloat(translationX, "translationX");
	stream->addFloat(translationY, "translationY");
	stream->addFloat(translationZ, "translationZ");

	// serialize rotation
	stream->addFloat(rotation.X, "rotationX");
	stream->addFloat(rotation.Y, "rotationY");
	stream->addFloat(rotation.Z, "rotationZ");
	stream->addFloat(rotation.W, "rotationW");

	//serialize scale
	stream->addFloat(scaleX, "scaleX");
	stream->addFloat(scaleY, "scaleY");
	stream->addFloat(scaleZ, "scaleZ");
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
	constructor from stream
***********************************************************/
DisplayInfo::DisplayInfo(SerializerBase * stream)
{
	// get header to tell if physics and display info are present
	bool hasDisplayDesc = stream->getBool("hasDisplayDesc");
	bool hasTransform = stream->getBool("hasTransform");	

	//get serialized transform
	if(hasTransform)
		Transform = boost::shared_ptr<DisplayTransformation>(new DisplayTransformation(stream));


	//get serialized description
	if(hasDisplayDesc)
	{
		//get type
		int dtype = stream->getInt("DisplayObjectType");
		DisplayObjectDescriptionBase * dobj = NULL;
		switch(dtype)
		{
			case 1:
				dobj = new OsgSimpleObjectDescription(stream);
			break;

			case 2:
				dobj = new OsgOrientedCapsuleDescription(stream);
			break;
		}

		DisplayDesc = boost::shared_ptr<DisplayObjectDescriptionBase>(dobj);
	}
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
boost::shared_ptr<DisplayObjectHandlerBase> DisplayInfo::BuildSelf(DisplayHandlerBase * disH) const
{
	if(DisplayDesc)
		return DisplayDesc->BuildSelf(Transform, disH);
	
	return boost::shared_ptr<DisplayObjectHandlerBase>();
}


/***********************************************************
serialize to network object
***********************************************************/
void DisplayInfo::Serialize(SerializerBase * stream) const
{
	// header to tel if physics and display info are present
	stream->addBool(DisplayDesc, "hasDisplayDesc");
	stream->addBool(Transform, "hasTransform");	

	//serialize transform
	if(Transform)
	{
		stream->startChildObjectList("Transform");
		Transform->Serialize(stream);
		stream->finishChildObjectList("Transform");
	}

	//serialize description
	if(DisplayDesc)
	{
		stream->startChildObjectList("DisplayDesc");
		stream->addInt(DisplayDesc->GetType(), "DisplayObjectType");
		DisplayDesc->Serialize(stream);	
		stream->finishChildObjectList("DisplayDesc");
	}
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
ObjectInfo::ObjectInfo(long oid, boost::shared_ptr<DisplayInfo> DInfo,
				boost::shared_ptr<PhysicalDescriptionBase> PInfo,
				bool Static, bool NoSmoothing)
	: DisInfo(DInfo), PhysInfo(PInfo), ForceNoSmoothing(NoSmoothing),
		IsStatic(Static), Id(oid)
{

}

/***********************************************************
	constructor from stream
***********************************************************/
ObjectInfo::ObjectInfo(SerializerBase * stream)
{
	//get serialized flags
	Id = stream->getLong("Id");
	ForceNoSmoothing = stream->getBool("ForceNoSmoothing");
	IsStatic = stream->getBool("IsStatic");


	// get header to tell if physics and display info are present
	bool hasPhysInfo = stream->getBool("hasPhysInfo");
	bool hasDisInfo = stream->getBool("hasDisInfo");


	// get serialized physic info
	if(hasPhysInfo)
	{
		stream->startChildObjectList("Physic");

		//get serialized object type
		int ptype = stream->getInt("PhysicalObjectType");
		PhysicalDescriptionBase * phyobj = NULL;
		switch(ptype)
		{
			//PhysicalDescriptionNoShape
			case 1: 
				phyobj = new PhysicalDescriptionNoShape(stream);
			break;

			//PhysicalDescriptionBox
			case 2: 
				phyobj = new PhysicalDescriptionBox(stream);
			break;

			//PhysicalDescriptionCapsule
			case 3: 
				phyobj = new PhysicalDescriptionCapsule(stream);
			break;

			//PhysicalDescriptionSphere
			case 4: 
				phyobj = new PhysicalDescriptionSphere(stream);
			break;

			//PhysicalDescriptionTriangleMesh
			case 5: 
				phyobj = new PhysicalDescriptionTriangleMesh(stream);
			break;
		}

		PhysInfo = boost::shared_ptr<PhysicalDescriptionBase>(phyobj); 

		stream->finishChildObjectList("Physic");
	}

	// get serialized display info
	if(hasDisInfo)
	{
		stream->startChildObjectList("Display");
		DisInfo = boost::shared_ptr<DisplayInfo>(new DisplayInfo(stream));
		stream->finishChildObjectList("Display");
	}
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
boost::shared_ptr<DynamicObject> ObjectInfo::BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine, int id,
														DisplayHandlerBase * disH) const
{
	boost::shared_ptr<PhysicalObjectHandlerBase> phH;
	boost::shared_ptr<DisplayObjectHandlerBase> disoH;
	if(PhysInfo)
		phH = PhysInfo->BuildSelf(_PEngine);
	if(DisInfo)
		disoH = DisInfo->BuildSelf(disH);

	if(!IsStatic && phH && disH)
		return boost::shared_ptr<DynamicObject>(new WorldToDisplayObjectSynchronizer(phH, disoH, id, ForceNoSmoothing));
	else
		return boost::shared_ptr<DynamicObject>(new StaticObject(phH, disoH, id));
}


/***********************************************************
serialize to network object
***********************************************************/
void ObjectInfo::Serialize(SerializerBase * stream) const
{
	//serialize flags
	stream->addLong(Id, "Id");
	stream->addBool(ForceNoSmoothing, "ForceNoSmoothing");
	stream->addBool(IsStatic, "IsStatic");


	// header to tel if physics and display info are present
	stream->addBool(PhysInfo, "hasPhysInfo");
	stream->addBool(DisInfo, "hasDisInfo");


	// serialize physic info
	if(PhysInfo)
	{
		stream->startChildObjectList("Physic");

		//serialize object type
		stream->addInt(PhysInfo->GetType(), "PhysicalObjectType");
		PhysInfo->Serialize(stream);

		stream->finishChildObjectList("Physic");
	}

	// serialize display info
	if(DisInfo)
	{
		stream->startChildObjectList("Display");
		DisInfo->Serialize(stream);
		stream->finishChildObjectList("Display");
	}
}




/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionBase::Serialize(SerializerBase * stream) const
{
	// serialize position
	stream->addFloat(positionX, "positionX");
	stream->addFloat(positionY, "positionY");
	stream->addFloat(positionZ, "positionZ");

	// serialize rotation
	stream->addFloat(rotation.X, "rotationX");
	stream->addFloat(rotation.Y, "rotationY");
	stream->addFloat(rotation.Z, "rotationZ");
	stream->addFloat(rotation.W, "rotationW");
}

/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionBase::PhysicalDescriptionBase(SerializerBase * stream)
{
	//get serialized position
	positionX = stream->getFloat("positionX");
	positionY = stream->getFloat("positionY");
	positionZ = stream->getFloat("positionZ");

	//get serialized rotation
	rotation.X = stream->getFloat("rotationX");
	rotation.Y = stream->getFloat("rotationY");
	rotation.Z = stream->getFloat("rotationZ");
	rotation.W = stream->getFloat("rotationW");
}



/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionNoShape::PhysicalDescriptionNoShape(SerializerBase * stream)
: PhysicalDescriptionBase(stream)
{
}



/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionWithShape::Serialize(SerializerBase * stream) const
{
	//call parent serialize
	PhysicalDescriptionBase::Serialize(stream);

	//serialize type
	stream->addInt(type, "PhysicalShapeType");

	// serialize density of the object
	stream->addFloat(density, "density");
}

/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionWithShape::PhysicalDescriptionWithShape(SerializerBase * stream)
: PhysicalDescriptionBase(stream)
{
	//get serialized type
	type = stream->getInt("PhysicalShapeType");

	//get serialized density of the object
	density = stream->getFloat("density");
}



/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionBox::Serialize(SerializerBase * stream) const
{
	//call parent serialize
	PhysicalDescriptionWithShape::Serialize(stream);

	//serialize size
	stream->addFloat(sizeX, "sizeX");
	stream->addFloat(sizeY, "sizeY");
	stream->addFloat(sizeZ, "sizeZ");
}	

/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionBox::PhysicalDescriptionBox(SerializerBase * stream)
: PhysicalDescriptionWithShape(stream)
{
	//get serialized size
	sizeX = stream->getFloat("sizeX");
	sizeY = stream->getFloat("sizeY");
	sizeZ = stream->getFloat("sizeZ");
}


/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionCapsule::Serialize(SerializerBase * stream) const
{
	//call parent serialize
	PhysicalDescriptionWithShape::Serialize(stream);

	//serialize size
	stream->addFloat(radius, "radius");
	stream->addFloat(height, "height");
}	

/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionCapsule::PhysicalDescriptionCapsule(SerializerBase * stream)
: PhysicalDescriptionWithShape(stream)
{
	//get serialized size
	radius = stream->getFloat("radius");
	height = stream->getFloat("height");
}


/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionSphere::Serialize(SerializerBase * stream) const
{
	//call parent serialize
	PhysicalDescriptionWithShape::Serialize(stream);

	//serialize size
	stream->addFloat(radius, "radius");
}	

/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionSphere::PhysicalDescriptionSphere(SerializerBase * stream)
: PhysicalDescriptionWithShape(stream)
{
	//get serialized size
	radius = stream->getFloat("radius");
}


/***********************************************************
serialize to network object
***********************************************************/
void PhysicalDescriptionTriangleMesh::Serialize(SerializerBase * stream) const
{
	//call parent serialize
	PhysicalDescriptionWithShape::Serialize(stream);

	//serialize filename
	stream->addString(MeshInfoDataFileName, "MeshInfoDataFileName");
}	


/***********************************************************
constructor from stream
***********************************************************/
PhysicalDescriptionTriangleMesh::PhysicalDescriptionTriangleMesh(SerializerBase * stream)
: PhysicalDescriptionWithShape(stream)
{
	//get serialized filename
	MeshInfoDataFileName = stream->getString("MeshInfoDataFileName");
}


/***********************************************************
serialize to network object
***********************************************************/
void OsgSimpleObjectDescription::Serialize(SerializerBase * stream) const
{
	//serialize filename
	stream->addString(_filename, "OsgFilename");
}

/***********************************************************
constructor from stream
***********************************************************/
OsgSimpleObjectDescription::OsgSimpleObjectDescription(SerializerBase * stream)
{
	//get serialized filename
	_filename = stream->getString("OsgFilename");
}



/***********************************************************
serialize to network object
***********************************************************/
void OsgOrientedCapsuleDescription::Serialize(SerializerBase * stream) const
{
	//serialize size
	stream->addFloat(_height, "height");
	stream->addFloat(_radius, "radius");

	//serialize color
	stream->addFloat(_colorR, "colorR");
	stream->addFloat(_colorG, "colorG");
	stream->addFloat(_colorB, "colorB");
	stream->addFloat(_colorA, "colorA");
}


/***********************************************************
constructor from stream
***********************************************************/
OsgOrientedCapsuleDescription::OsgOrientedCapsuleDescription(SerializerBase * stream)
{
	//get serialized size
	_height = stream->getFloat("height");
	_radius = stream->getFloat("radius");

	//get serialized color
	_colorR = stream->getFloat("colorR");
	_colorG = stream->getFloat("colorG");
	_colorB = stream->getFloat("colorB");
	_colorA = stream->getFloat("colorA");
}



/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DisplayObjectHandlerBase> OsgSimpleObjectDescription::BuildSelf(
															boost::shared_ptr<DisplayTransformation> Tr,
															DisplayHandlerBase * disH) const
{
	if(disH)
		disH->CreateSimpleObject(_filename, Tr);

	return boost::shared_ptr<DisplayObjectHandlerBase> ();
}




/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DisplayObjectHandlerBase> OsgOrientedCapsuleDescription::BuildSelf(
															boost::shared_ptr<DisplayTransformation> Tr,
															DisplayHandlerBase * disH) const
{
	if(disH)
		disH->CreateCapsuleObject(_radius, _height, _colorR, _colorG, _colorB, _colorA, Tr);

	return boost::shared_ptr<DisplayObjectHandlerBase> ();
}
