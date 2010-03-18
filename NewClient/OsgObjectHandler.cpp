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

#include "OsgObjectHandler.h"
#include "ObjectsDescription.h"
#include "OSGHandler.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>


/***********************************************************
Constructor
***********************************************************/
OsgObjectHandler::OsgObjectHandler(osg::ref_ptr<osg::MatrixTransform> OsgObject)
: _OsgObject(OsgObject), _posX(0), _posY(0), _posZ(0)
{
	UpdateMatrix();
}

/***********************************************************
destructor
***********************************************************/
OsgObjectHandler::~OsgObjectHandler()
{

}


/***********************************************************
set object position in the world
***********************************************************/
void OsgObjectHandler::SetPosition(float X, float Y, float Z)
{
	_posX = X;
	_posY = Y; 
	_posZ = Z;
	UpdateMatrix();
}

/***********************************************************
set object rotation on X axis
***********************************************************/
void OsgObjectHandler::SetRotation(const LbaQuaternion& Q)
{
	_Q = Q;
	UpdateMatrix();
}

/***********************************************************
update matrix
***********************************************************/
void OsgObjectHandler::UpdateMatrix()
{
	if(_OsgObject)
	{
		osg::Matrixd Trans;
		osg::Matrixd Rotation;

		Trans.makeTranslate(_posX, _posY, _posZ);
		Rotation.makeRotate(osg::Quat(_Q.X, _Q.Y, _Q.Z, _Q.W));

		_OsgObject->setMatrix(Rotation * Trans);
	}
}


/***********************************************************
destroy function - clear the object content
***********************************************************/
void OsgObjectHandler::Destroy(void)
{
	OsgHandler::getInstance()->RemoveActorNode(_OsgObject);
}



/***********************************************************
set the object to be followed by the camera
***********************************************************/
void OsgObjectHandler::SetCameraFollow(void)
{
	OsgHandler::getInstance()->SetCameraFollowingNode(_OsgObject);
}



/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DisplayObjectHandlerBase> OsgSimpleObjectDescription::BuildSelf(
															boost::shared_ptr<DisplayTransformation> Tr) const
{
	osg::ref_ptr<osg::Node> resnode = OsgHandler::getInstance()->LoadOSGFile(_filename);

	if(Tr)
	{
		osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();
		transform->setPosition(osg::Vec3d(Tr->translationX, Tr->translationY, Tr->translationZ));
		transform->setAttitude(osg::Quat(Tr->rotation.X, Tr->rotation.Y, Tr->rotation.Z, Tr->rotation.W));
		transform->setScale(osg::Vec3d(Tr->scaleX, Tr->scaleY, Tr->scaleZ));

		transform->addChild(resnode);
		resnode = transform;
	}
	
	osg::ref_ptr<osg::MatrixTransform> mat = OsgHandler::getInstance()->AddActorNode(resnode);
	return boost::shared_ptr<DisplayObjectHandlerBase>(new OsgObjectHandler(mat));
}




/***********************************************************
build description into dynamic object
***********************************************************/
boost::shared_ptr<DisplayObjectHandlerBase> OsgOrientedCapsuleDescription::BuildSelf(
															boost::shared_ptr<DisplayTransformation> Tr) const
{
	osg::ref_ptr<osg::Group> resnode = new osg::Group();

	// create capsule
	osg::ref_ptr<osg::Geode> capsuleGeode(new osg::Geode());
	osg::ref_ptr<osg::Capsule> caps(new osg::Capsule(osg::Vec3(0,0,0),_radius,_height));
	osg::ref_ptr<osg::ShapeDrawable> capsdraw = new osg::ShapeDrawable(caps);
	capsdraw->setColor(osg::Vec4(_colorR, _colorG, _colorB, _colorA));
	capsuleGeode->addDrawable(capsdraw);
	resnode->addChild(capsuleGeode);


	// create orientation line
	osg::Geode* lineGeode = new osg::Geode();
	osg::Geometry* lineGeometry = new osg::Geometry();
	lineGeode->addDrawable(lineGeometry); 

	osg::Vec3Array* lineVertices = new osg::Vec3Array();
	lineVertices->push_back( osg::Vec3( 0, 0, 0) );
	lineVertices->push_back( osg::Vec3(0, 5, 0) );
	lineGeometry->setVertexArray( lineVertices ); 

	osg::DrawElementsUInt* dunit = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	dunit->push_back(0);
	dunit->push_back(1);
	lineGeometry->addPrimitiveSet(dunit); 
	resnode->addChild(lineGeode);


	if(Tr)
	{
		osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();
		transform->setPosition(osg::Vec3d(Tr->translationX, Tr->translationY, Tr->translationZ));
		transform->setAttitude(osg::Quat(Tr->rotation.X, Tr->rotation.Y, Tr->rotation.Z, Tr->rotation.W));
		transform->setScale(osg::Vec3d(Tr->scaleX, Tr->scaleY, Tr->scaleZ));

		transform->addChild(resnode);
		resnode = transform;
	}
	
	osg::ref_ptr<osg::MatrixTransform> mat = OsgHandler::getInstance()->AddActorNode(resnode);
	return boost::shared_ptr<DisplayObjectHandlerBase>(new OsgObjectHandler(mat));
}



