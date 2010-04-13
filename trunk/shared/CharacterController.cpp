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

#include "CharacterController.h"
#include "PhysicalObjectHandlerBase.h"
#include "PhysXEngine.h"


#include <iostream>

/***********************************************************
	Constructor
***********************************************************/
CharacterController::CharacterController(boost::shared_ptr<PhysXEngine> pEngine)
:	_current_direction(0, 0, 1), _pEngine(pEngine), _isGhost(false)
{

}


/***********************************************************
	Destructor
***********************************************************/
CharacterController::~CharacterController()
{

}


/***********************************************************
	Set character to control
***********************************************************/
void CharacterController::SetCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac, bool AsGhost)
{
	_character = charac;
	_isGhost = AsGhost;
}




/***********************************************************
	apply inputs
***********************************************************/
void CharacterController::ApplyInputs(const Input & in)
{
	_lastInputs = in;
}


/***********************************************************
process function
***********************************************************/
void CharacterController::Process(unsigned int tnow, float tdiff)
{
	if(!_character)
		return;

	if(_isGhost)
	{
		float speedX = 0.0f;
		float speedY = 0.0f;
		float speedZ = 0.0f;

		//if right key pressed
		if(_lastInputs.right)
			speedX = 10.0f;

		//if left key pressed
		if(_lastInputs.left)
			speedX = -10.0f;

		//if up key pressed
		if(_lastInputs.up)
			speedZ = -10.0f;

		//if down key pressed
		if(_lastInputs.down)
			speedZ = 10.0f;

		_character->Move(tnow, speedX, speedY, speedZ);
	}
	else
	{
		//if right key pressed
		if(_lastInputs.right)
		{
			LbaQuaternion rot;
			_character->GetRotation(rot);
			rot.AddRotation(-tdiff*100.0f, LbaVec3(0, 1, 0));
			_character->RotateTo(tnow, rot);

			_current_direction = rot.GetDirection(LbaVec3(0, 0, 1));
		}

		//if left key pressed
		if(_lastInputs.left)
		{
			LbaQuaternion rot;
			_character->GetRotation(rot);
			rot.AddRotation(tdiff*100.0f, LbaVec3(0, 1, 0));
			_character->RotateTo(tnow, rot);

			_current_direction = rot.GetDirection(LbaVec3(0, 0, 1));
		}

		//if up/down key
		float speed = 0.0f;
		if(_lastInputs.up)
			speed = 5.0f;

		if(_lastInputs.down)
			speed = -5.0f;

		LbaVec3 Gravity;
		_pEngine->GetGravity(Gravity);
		_character->Move(tnow, _current_direction.x*speed, Gravity.y, _current_direction.z*speed);
	}
}