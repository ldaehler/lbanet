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

#if !defined(__LbaNetModel_1_PlayerHandler_h)
#define __LbaNetModel_1_PlayerHandler_h

#include <math.h>
#include <vector>


/***********************************************************************
 * Module:  PlayerHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:54:27
 * Purpose: Declaration of the class PlayerHandler
 ***********************************************************************/
class PlayerHandler
{
public:
	//! constructor
	PlayerHandler(float speed, unsigned int controllerid);

	//! destructor
	~PlayerHandler();


	//! do all check to be done when idle
	int Process(double tnow, float tdiff);

	// player moves
	void PlayerStartMove(int moveDirection);
	void PlayerStopMove(int moveDirection);
	void PlayerStartMoveUpDown(bool Up);
	void PlayerStopMoveUpDown(bool Up);


protected:
	void CalculateVelocity(bool MoveForward, bool ManualSpeed=false, float speed=0);

	//get actor moving speed
	float GetMovingSpeed();


	// remember if a key is pressed
	bool			_up_key_pressed;
	bool			_down_key_pressed;
	bool			_right_key_pressed;
	bool			_left_key_pressed;
	bool			_pageup_key_pressed;
	bool			_pagedown_key_pressed;

	float			_velocityX;
	float			_velocityY;
	float			_velocityZ;
	float			_velocityR;

	float			_corrected_velocityX;
	float			_corrected_velocityY;
	float			_corrected_velocityZ;

	float			_currrotation;


	float			_speed;
	unsigned int	_controllerid;

};

#endif