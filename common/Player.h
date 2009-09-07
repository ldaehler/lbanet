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


#if !defined(__LbaNetModel_1_Player_h)
#define __LbaNetModel_1_Player_h

#include "LivingActor.h"


/***********************************************************************
 * Module:  Player.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:54:54
 * Purpose: Declaration of the class Player
 ***********************************************************************/
class Player : public LivingActor
{
public:
	//! constructor
	Player(float animationSpeed, bool MainPlayer = false);

	//! destructor
	~Player();

	//! render object
	virtual void Render(int RoomCut);

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);

private:

};

#endif