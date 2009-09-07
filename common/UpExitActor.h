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


#if !defined(__LbaNetModel_1_UpExitActor_h)
#define __LbaNetModel_1_UpExitActor_h

#include "ActivableActor.h"


/***********************************************************************
 * Module:  ExitActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class UpExitActor : public ActivableActor
{
public:
	//! constructor
	UpExitActor(float activationdistance, int direction);

	//! destructor
	virtual ~UpExitActor();

	//! accessors
	int GetDir()
	{return _direction;}

	void SetDir(int dir)
	{_direction = dir;}

protected:
	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation);

private:
	int _direction;
};

#endif