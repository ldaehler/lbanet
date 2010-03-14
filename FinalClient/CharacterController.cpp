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
#include "DynamicObject.h"
#include "PhysXEngine.h"


/***********************************************************
	Constructor
***********************************************************/
CharacterController::CharacterController(boost::shared_ptr<PhysXEngine> pEngine)
:	_up_key_pressed(false),_down_key_pressed(false),
	_right_key_pressed(false),_left_key_pressed(false),
	_current_direction(0, 0, 1), _pEngine(pEngine)
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
void CharacterController::SetCharacter(boost::shared_ptr<DynamicObject> charac)
{
	_character = charac;
}



/***********************************************************
start a move from keyboard input
***********************************************************/
void CharacterController::StartMove(int moveDirection)
{
	if(moveDirection == 1 || moveDirection == 2)
		if(_up_key_pressed || _down_key_pressed)
			return;

	if(moveDirection == 3 && _left_key_pressed)
		return;

	if(moveDirection == 4 && _right_key_pressed)
		return;


	switch(moveDirection)
	{
		case 1:
			_up_key_pressed = true;
		break;
		case 2:
			_down_key_pressed = true;
		break;
		case 3:
			_left_key_pressed = true;
			_right_key_pressed = false;
		break;
		case 4:
			_left_key_pressed = false;
			 _right_key_pressed = true;
		break;
	}
	
}


/***********************************************************
stop a move from keyboard input
***********************************************************/
void CharacterController::StopMove(int moveDirection)
{
	switch(moveDirection)
	{
		case 1:
			_up_key_pressed = false;
		break;
		case 2:
			_down_key_pressed = false;
		break;
		case 3:
			_left_key_pressed = false;
		break;
		case 4:
			 _right_key_pressed = false;
		break;
	}
}

/***********************************************************
do action from keyboard input
***********************************************************/
void CharacterController::DoAction()
{
	if(!_character)
		return;

	boost::shared_ptr<PhysicalObjectHandlerBase> phys = _character->GetPhysicalObject();
	if(!phys)
		return;

}


/***********************************************************
process function
***********************************************************/
void CharacterController::Process(double tnow, float tdiff)
{
	if(!_character)
		return;

	boost::shared_ptr<PhysicalObjectHandlerBase> phys = _character->GetPhysicalObject();
	if(!phys)
		return;

	//if right key pressed
	if(_right_key_pressed)
	{
		LbaQuaternion rot;
		phys->GetRotation(rot);
		rot.AddRotation(- tdiff*100.0f, LbaVec3(0, 1, 0));
		phys->RotateTo(rot);

		_current_direction = rot.GetDirection(LbaVec3(0, 0, 1));
	}

	//if left key pressed
	if(_left_key_pressed)
	{
		LbaQuaternion rot;
		phys->GetRotation(rot);
		rot.AddRotation(tdiff*100.0f, LbaVec3(0, 1, 0));
		phys->RotateTo(rot);

		_current_direction = rot.GetDirection(LbaVec3(0, 0, 1));
	}

	//if up/down key
	float speed = 0.0f;
	if(_up_key_pressed)
		speed = 5.0f;

	if(_down_key_pressed)
		speed = -5.0f;

	LbaVec3 Gravity;
	_pEngine->GetGravity(Gravity);
	phys->Move(_current_direction.x*speed* tdiff, Gravity.y/20, _current_direction.z*speed * tdiff);
}