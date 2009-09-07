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


#if !defined(__LbaNetModel_1_ZoneActivableActor_h)
#define __LbaNetModel_1_ZoneActivableActor_h

#include "Actor.h"


/***********************************************************************
 * Module:  ZoneActivableActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ZoneActivableActor : public Actor
{
public:
	//! constructor
	ZoneActivableActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ);

	//! destructor
	virtual ~ZoneActivableActor();

	//! activate an actor
	virtual bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation=true);

	//! render editor
	void RenderEditor();

	//!accessors
	float GetZoneX()
	{return _ZoneSizeX;}
	float GetZoneY()
	{return _ZoneSizeY;}
	float GetZoneZ()
	{return _ZoneSizeZ;}

	void SetZoneX(float v)
	{_ZoneSizeX = v;}
	void SetZoneY(float v)
	{_ZoneSizeY = v;}
	void SetZoneZ(float v)
	{_ZoneSizeZ = v;}

private:
	float _ZoneSizeX;
	float _ZoneSizeY;
	float _ZoneSizeZ;
};

#endif