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

#include "WorldToDisplayObjectSynchronizer.h"
#include <osg/Quat>

#define ABS(X) (((X)<0)?(-(X)):(X))

/***********************************************************
constructor
***********************************************************/
WorldToDisplayObjectSynchronizer::WorldToDisplayObjectSynchronizer(boost::shared_ptr<PhysicalObjectHandlerBase> phH,
																	boost::shared_ptr<DisplayObjectHandlerBase> disH,
																	bool NoSmoothing)
	: DynamicObject(phH, disH),
		_lastDisplayPositionX(0), _lastDisplayPositionY(0), _lastDisplayPositionZ(0), _NoSmoothing(NoSmoothing)
{

}

/***********************************************************
destructor
***********************************************************/
WorldToDisplayObjectSynchronizer::~WorldToDisplayObjectSynchronizer()
{
}


/***********************************************************
synchronization function - will typically be called on every frames
***********************************************************/
void WorldToDisplayObjectSynchronizer::Process(void)
{
	// if physical object was reseted then we do a straight sync - else do a smoothing sync
	if(_NoSmoothing || _phH->WasReseted())
		StraightSync();
	else
		SyncWithSmoothing();
}


/***********************************************************
directly synchronize value between physic and display
***********************************************************/
void WorldToDisplayObjectSynchronizer::StraightSync()
{
	// get value from physic object
	float posX, posY, posZ;
	LbaQuaternion Quat;
	_phH->GetPosition(posX, posY, posZ);
	_phH->GetRotation(Quat);


	// for each value test if they are equal with display value
	//if not move display value to the correct value
	bool positionchanged = false;
	if(!equal(posX, _lastDisplayPositionX))
	{
		positionchanged = true;
		_lastDisplayPositionX = posX;
	}

	if(!equal(posY, _lastDisplayPositionY))
	{
		positionchanged = true;
		_lastDisplayPositionY = posY;
	}

	if(!equal(posZ, _lastDisplayPositionZ))
	{
		positionchanged = true;
		_lastDisplayPositionZ = posZ;
	}

	// update displayed position if needed
	if(positionchanged)
		_disH->SetPosition(_lastDisplayPositionX, _lastDisplayPositionY, _lastDisplayPositionZ);


	// check rotations
	if(		!equal(Quat.X, _lastDisplayRotation.X)
		||	!equal(Quat.Y, _lastDisplayRotation.Y)
		||	!equal(Quat.Z, _lastDisplayRotation.Z)
		||	!equal(Quat.W, _lastDisplayRotation.W))
	{
		_lastDisplayRotation = Quat;
		_disH->SetRotation(_lastDisplayRotation);
	}
}

/***********************************************************
synchronize value between physic and display with smoothing factor
***********************************************************/
void WorldToDisplayObjectSynchronizer::SyncWithSmoothing()
{
	static const float	_tightnessPosition = 0.1f; //10% smoothing on position
	static const float	_tightnessRotation = 0.2f; //20% smoothing on rotation

	static const float	_maxPositionDelta = 2; //allow a max delta of 2 between physic and display for position
	static const float	_maxRotationDelta = 30; //allow a max delta of 30 between physic and display for rotation


	// get value from physic object
	float posX, posY, posZ;
	LbaQuaternion Quat;
	_phH->GetPosition(posX, posY, posZ);
	_phH->GetRotation(Quat);


	// for each value test if they are equal with display value
	//if not move display value smoothly to the correct value

	bool positionchanged = false;
	if(!equal(posX, _lastDisplayPositionX))
	{
		positionchanged = true;
		float delta = (posX-_lastDisplayPositionX);

		// if delta is too big set it to correct position otherwise smooth it
		if(ABS(delta) < _maxPositionDelta)
			_lastDisplayPositionX += delta * _tightnessPosition;
		else
			_lastDisplayPositionX = posX;
	}

	if(!equal(posY, _lastDisplayPositionY))
	{
		positionchanged = true;
		float delta = (posY-_lastDisplayPositionY);

		// if delta is too big set it to correct position otherwise smooth it
		if(ABS(delta) < _maxPositionDelta)
			_lastDisplayPositionY += delta * _tightnessPosition;
		else
			_lastDisplayPositionY = posY;
	}

	if(!equal(posZ, _lastDisplayPositionZ))
	{
		positionchanged = true;
		float delta = (posZ-_lastDisplayPositionZ);

		// if delta is too big set it to correct position otherwise smooth it
		if(ABS(delta) < _maxPositionDelta)
			_lastDisplayPositionZ += delta * _tightnessPosition;
		else
			_lastDisplayPositionZ = posZ;
	}

	// update displayed position if needed
	if(positionchanged)
		_disH->SetPosition(_lastDisplayPositionX, _lastDisplayPositionY, _lastDisplayPositionZ);


	// check rotations
	if(		!equal(Quat.X, _lastDisplayRotation.X)
		||	!equal(Quat.Y, _lastDisplayRotation.Y)
		||	!equal(Quat.Z, _lastDisplayRotation.Z)
		||	!equal(Quat.W, _lastDisplayRotation.W))
	{

		// check if interpolation works well here!
		osg::Quat quat1(Quat.X, Quat.Y, Quat.Z, Quat.W);

		osg::Quat quat2(_lastDisplayRotation.X, _lastDisplayRotation.Y,
								_lastDisplayRotation.Z, _lastDisplayRotation.W);

		quat2.slerp(_tightnessRotation, quat2, quat1);

		_lastDisplayRotation = LbaQuaternion(quat2.x(), quat2.y(), quat2.z(), quat2.w());
		_disH->SetRotation(_lastDisplayRotation);
	}
}


/***********************************************************
destroy function - clear the object content
***********************************************************/
void WorldToDisplayObjectSynchronizer::Destroy(void)
{
	_phH->Destroy();
	_disH->Destroy();
}