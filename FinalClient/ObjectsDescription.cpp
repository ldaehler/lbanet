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
	Constructor
***********************************************************/
PhysicalDescriptionWithShape::PhysicalDescriptionWithShape(float posX, float posY, float posZ,
														int Otype, float Odensity,
														float rotX, float rotY, float rotZ)
: PhysicalDescriptionBase(posX, posY, posZ), type(Otype), density(Odensity),
			rotationX(rotX), rotationY(rotY), rotationZ(rotZ)
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
												float rotX, float rotY, float rotZ,
												float sX, float sY, float sZ)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rotX, rotY, rotZ),
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
	Constructor
***********************************************************/
PhysicalDescriptionCapsule::PhysicalDescriptionCapsule(float posX, float posY, float posZ,
														int Otype, float Odensity,
														float rotX, float rotY, float rotZ,
														float rad, float ht)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rotX, rotY, rotZ),
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
	Constructor
***********************************************************/
PhysicalDescriptionSphere::PhysicalDescriptionSphere(float posX, float posY, float posZ,
														int Otype, float Odensity,
														float rotX, float rotY, float rotZ,
														float rad)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rotX, rotY, rotZ),
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
	Constructor
***********************************************************/
PhysicalDescriptionTriangleMesh::PhysicalDescriptionTriangleMesh(float posX, float posY, float posZ,
																	const std::string &FileName)
	:PhysicalDescriptionWithShape(posX, posY, posZ, 1, 0, 0, 0, 0),
		MeshInfoDtaFileName(FileName)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionTriangleMesh::~PhysicalDescriptionTriangleMesh()
{

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