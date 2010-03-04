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

#include "PlayerHandler.h"
#include "SynchronizedTimeHandler.h"
#include "PhysXEngine.h"
#include "NxVec3.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <math.h>


#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif



/***********************************************************
	Constructor
***********************************************************/
PlayerHandler::PlayerHandler(float speed, unsigned int controllerid)
: _up_key_pressed(false),_down_key_pressed(false),
	_right_key_pressed(false),_left_key_pressed(false),
	_pageup_key_pressed(false), _pagedown_key_pressed(false),
	_velocityX(0), _velocityY(0), _velocityZ(0)
	,_velocityR(0), _corrected_velocityX(0), _corrected_velocityY(0)
	, _corrected_velocityZ(0), _currrotation(0),
	_speed(speed), _controllerid(controllerid)
{

}


/***********************************************************
	Destructor
***********************************************************/
PlayerHandler::~PlayerHandler()
{
}



/***********************************************************
do all check to be done when idle
***********************************************************/
int PlayerHandler::Process(double tnow, float tdiff)
{
	_velocityR = 0;
	_corrected_velocityX = 0;
	_corrected_velocityY = 0;
	_corrected_velocityZ = 0;


	//if right key pressed
	if(_right_key_pressed)
	{
		_currrotation -= 60.f * tdiff;
		_velocityR = -60.f;
	}

	//if left key pressed
	if(_left_key_pressed)
	{
		_currrotation += 60.f * tdiff;
		_velocityR = 60.f;
	}

	if(_currrotation >= 360)
		_currrotation = _currrotation - 360;
	if(_currrotation < 0)
		_currrotation = 360 + _currrotation;

	// if we are flying - check up/down input
	if(_pageup_key_pressed)
		_corrected_velocityY = 0.02f * tdiff;

	if(_pagedown_key_pressed)
		_corrected_velocityY = -0.02f * tdiff;


	// if up/down key
	if(_up_key_pressed || _down_key_pressed)
	{
		CalculateVelocity(_up_key_pressed);

		_corrected_velocityX = _velocityX * tdiff;
		_corrected_velocityZ = _velocityZ * tdiff;
	}
	else
	{
		CalculateVelocity(_up_key_pressed, true, 0);
	}

	NxVec3 vectmp;
	PhysXEngine::getInstance()->GetGravity(vectmp);
	_corrected_velocityY = vectmp.y * tdiff;

	PhysXEngine::getInstance()->MoveCharacter(_controllerid, NxVec3(_corrected_velocityX, 
													_corrected_velocityY, _corrected_velocityZ));


	return 0;
}



/***********************************************************
player moves
***********************************************************/
void PlayerHandler::PlayerStartMove(int moveDirection)
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
player moves
***********************************************************/
void PlayerHandler::PlayerStopMove(int moveDirection)
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
player moves
***********************************************************/
void PlayerHandler::PlayerStartMoveUpDown(bool Up)
{
	// if we are flying
	if(Up)
	{
		_pagedown_key_pressed = false;
		_pageup_key_pressed = true;
	}
	else
	{
		_pageup_key_pressed = false;
		_pagedown_key_pressed = true;
	}
}


/***********************************************************
player moves
***********************************************************/
void PlayerHandler::PlayerStopMoveUpDown(bool Up)
{
	if(Up)
		_pageup_key_pressed = false;
	else
		_pagedown_key_pressed = false;
}



/***********************************************************
get actor moving speed
***********************************************************/
float PlayerHandler::GetMovingSpeed()
{
	return _speed;
}



/***********************************************************
recalculate actor velocity
***********************************************************/
void PlayerHandler::CalculateVelocity(bool MoveForward, bool ManualSpeed, float speed)
{
	float halfM = GetMovingSpeed() * (MoveForward ? -1.0f : 0.5f);
	if(ManualSpeed)
		halfM = speed;


	int nbA = ((int)_currrotation) / 90;
	int modA = ((int)_currrotation) % 90;


	float radA =  M_PI * (modA) / 180.0f;


	if(nbA == 0)
	{
		_velocityX = sin(radA) * -halfM;
		_velocityZ = cos(radA) * -halfM;
	}
	if(nbA == 1)
	{
		_velocityX = cos((float)radA) * -halfM;
		_velocityZ = sin((float)radA) * halfM;
	}
	if(nbA == 2)
	{
		_velocityX = sin(radA) * halfM;
		_velocityZ = cos(radA) * halfM;
	}
	if(nbA == 3)
	{
		_velocityX = cos((float)radA) * halfM;
		_velocityZ = sin((float)radA) * -halfM;
	}
}

