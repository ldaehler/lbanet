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


#if !defined(__LbaNetModel_1_AreaSwitch_h)
#define __LbaNetModel_1_AreaSwitch_h

#include "ZoneActor.h"

/***********************************************************************
 * Module:  DoorActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class AreaSwitch : public ZoneActor
{
public:
	//! constructor
	AreaSwitch(float zoneSizeX, float zoneSizeY, float zoneSizeZ, long QuestToTriggerEnd);

	//! destructor
	virtual ~AreaSwitch();

	//! accessor
	long GetQuestToTriggerEnd()
	{ return _QuestToTriggerEnd; }

protected:
	//! process zone activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
		float PlayerRotation);

	//! process zone desactivation
	virtual void ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
		float PlayerRotation);


private:
	// ques to trigger end when activated
	long _QuestToTriggerEnd;
};

#endif