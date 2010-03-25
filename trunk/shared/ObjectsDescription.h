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


#ifndef __LBA_NET_OBJECTS_DESCRIPTION_H__
#define __LBA_NET_OBJECTS_DESCRIPTION_H__

#include <boost/shared_ptr.hpp>
#include "PhysicalObjectHandlerBase.h"
#include "DisplayObjectHandlerBase.h"
#include "CommonTypes.h"

class PhysXEngine;
class DynamicObject;


/***********************************************************************
This class describe the tranformation applied by the display object 
before being drwan on the screen
 ***********************************************************************/
class DisplayTransformation
{
public:
	//! constructor
	DisplayTransformation();

	//! destructor
	~DisplayTransformation();


public:
	// translations
	float translationX;
	float translationY;
	float translationZ;

	//rotations
	LbaQuaternion rotation;

	//scaling
	float scaleX;
	float scaleY;
	float scaleZ;
};


/***********************************************************************
This is the base class describing the object to be drawn
 ***********************************************************************/
class DisplayObjectDescriptionBase
{
public:
	//! constructor
	DisplayObjectDescriptionBase(){}

	//! destructor
	virtual ~DisplayObjectDescriptionBase(){}

	//! build description into dynamic object
	virtual boost::shared_ptr<DisplayObjectHandlerBase> BuildSelf(boost::shared_ptr<DisplayTransformation> Tr) const = 0;

};





/***********************************************************************
This is the base class describing a simple OSG object loaded from file
See definition in OsgObjectHandler.cpp
 ***********************************************************************/
class OsgSimpleObjectDescription : public DisplayObjectDescriptionBase
{
public:
	//! constructor
	OsgSimpleObjectDescription(const std::string & filename)
		: _filename(filename)
	{}

	//! destructor
	virtual ~OsgSimpleObjectDescription(){}

	//! build description into dynamic object
	virtual boost::shared_ptr<DisplayObjectHandlerBase> BuildSelf(boost::shared_ptr<DisplayTransformation> Tr) const;


private:
	const std::string _filename;
};





/***********************************************************************
This is the base class describing an Oriented Capsule
See definition in OsgObjectHandler.cpp
 ***********************************************************************/
class OsgOrientedCapsuleDescription : public DisplayObjectDescriptionBase
{
public:
	//! constructor
	OsgOrientedCapsuleDescription(float height, float radius, 
									float colorR, float colorG, float colorB, float colorA)
		: _height(height), _radius(radius), _colorR(colorR), 
					_colorG(colorG), _colorB(colorB), _colorA(colorA)
	{}

	//! destructor
	virtual ~OsgOrientedCapsuleDescription(){}


	//! build description into dynamic object
	virtual boost::shared_ptr<DisplayObjectHandlerBase> BuildSelf(boost::shared_ptr<DisplayTransformation> Tr) const;


private:
	float _height;
	float _radius;

	float _colorR;
	float _colorG;
	float _colorB;
	float _colorA;
};





/***********************************************************************
This class regroup the object display description and transformation
 ***********************************************************************/
class DisplayInfo
{
public:
	//! constructor
	DisplayInfo(boost::shared_ptr<DisplayTransformation> Tr, 
					boost::shared_ptr<DisplayObjectDescriptionBase> Ds);

	//! destructor
	~DisplayInfo();


	//! build description into dynamic object
	boost::shared_ptr<DisplayObjectHandlerBase> BuildSelf();


public:
	boost::shared_ptr<DisplayTransformation>		Transform;
	boost::shared_ptr<DisplayObjectDescriptionBase> DisplayDesc;
};




/***********************************************************************
This base class describe the physicall information of an object
E.g. position in world - shape of the phisycall object
 ***********************************************************************/
class PhysicalDescriptionBase
{
public:
	//! constructor
	PhysicalDescriptionBase(float posX, float posY, float posZ, const LbaQuaternion &rot);

	//! destructor
	virtual ~PhysicalDescriptionBase();

	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const = 0;



public:
	//position of the object in the world
	float positionX;
	float positionY;
	float positionZ;

	// rotation applied to the shape
	LbaQuaternion rotation;
};


/***********************************************************************
This base class describe physicall object with no shape
 ***********************************************************************/
class PhysicalDescriptionNoShape : public PhysicalDescriptionBase
{
public:
	//! constructor
	PhysicalDescriptionNoShape(float posX, float posY, float posZ, const LbaQuaternion &rot);

	//! destructor
	virtual ~PhysicalDescriptionNoShape();

	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const;

	//nothing more to do here, the only thing we need is a position
};


/***********************************************************************
This base class describe physicall object with shape
 ***********************************************************************/
class PhysicalDescriptionWithShape : public PhysicalDescriptionBase
{
public:
	//! constructor
	PhysicalDescriptionWithShape(float posX, float posY, float posZ,
									int Otype, float Odensity,
									const LbaQuaternion &rot);

	//! destructor
	virtual ~PhysicalDescriptionWithShape();


public:

	// type of physical shape
	// 1= static
	// 2= kynematic
	// 3= dynamic
	// 4= character controller
	int type;

	// density of the object
	float density;


};


/***********************************************************************
This class describe a box shape
See definition in PhysXObjectHandlerBase.cpp
 ***********************************************************************/
class PhysicalDescriptionBox : public PhysicalDescriptionWithShape
{
public:
	//! constructor
	PhysicalDescriptionBox(float posX, float posY, float posZ,
									int Otype, float Odensity,
									const LbaQuaternion &rot,
									float sX, float sY, float sZ);

	//! destructor
	virtual ~PhysicalDescriptionBox();


	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const;

public:
	// size of the box from the box center
	float sizeX;
	float sizeY;
	float sizeZ;
};


/***********************************************************************
This class describe a capsule shape
See definition in PhysXObjectHandlerBase.cpp
 ***********************************************************************/
class PhysicalDescriptionCapsule : public PhysicalDescriptionWithShape
{
public:
	//! constructor
	PhysicalDescriptionCapsule(float posX, float posY, float posZ,
									int Otype, float Odensity,
									const LbaQuaternion &rot,
									float rad, float ht);

	//! destructor
	virtual ~PhysicalDescriptionCapsule();


	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const;

public:
	// radius of the capsule from capsule center
	float radius;

	// height of the capsule
	float height;
};


/***********************************************************************
This class describe a sphere shape
See definition in PhysXObjectHandlerBase.cpp
 ***********************************************************************/
class PhysicalDescriptionSphere : public PhysicalDescriptionWithShape
{
public:
	//! constructor
	PhysicalDescriptionSphere(float posX, float posY, float posZ,
									int Otype, float Odensity,
									const LbaQuaternion &rot,
									float rad);

	//! destructor
	virtual ~PhysicalDescriptionSphere();


	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const;

public:
	// radius of the sphere from capsule sphere
	float radius;
};


/***********************************************************************
This class describe a triangle mesh shape
Only use for static shape object with no rotation
See definition in PhysXObjectHandlerBase.cpp
 ***********************************************************************/
class PhysicalDescriptionTriangleMesh : public PhysicalDescriptionWithShape
{
public:
	//! constructor
	PhysicalDescriptionTriangleMesh(float posX, float posY, float posZ,
										const std::string &FileName);

	//! destructor
	virtual ~PhysicalDescriptionTriangleMesh();


	//! build description into a reald physic object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine) const;

public:
	// path of the file describing the mesh
	std::string MeshInfoDataFileName;
};




/***********************************************************************
This class describe an object physical shape and display shape
 ***********************************************************************/
class ObjectInfo
{
public:
	//! constructor
	ObjectInfo(boost::shared_ptr<DisplayInfo> DInfo,
				boost::shared_ptr<PhysicalDescriptionBase> PInfo,
				bool Static, bool NoSmoothing=false);

	//! destructor
	virtual ~ObjectInfo();

	//! build description into dynamic object
	boost::shared_ptr<DynamicObject> BuildSelf(boost::shared_ptr<PhysXEngine> _PEngine, int id) const;

public:
	boost::shared_ptr<DisplayInfo>				DisInfo;
	boost::shared_ptr<PhysicalDescriptionBase>	PhysInfo;
	bool										ForceNoSmoothing;
	bool										IsStatic;
};



#endif