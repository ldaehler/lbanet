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


#if !defined(__LbaNetModel_1_ZoneActor_h)
#define __LbaNetModel_1_ZoneActor_h

#include "Actor.h"
#include <set>

/***********************************************************************
 * Module:  ZoneActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ZoneActor : public Actor
{
public:
	//! constructor
	ZoneActor(float zoneSizeX, float zoneSizeY, float zoneSizeZ);

	//! destructor
	virtual ~ZoneActor();

	//! check zone activation
	virtual int ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								MainPlayerHandler  * _mph, bool DirectActivation=true);

	//! render editor part
	virtual void RenderEditor();

	//! accessors
	float GetZoneX()
	{return _zoneSizeX;}
	float GetZoneY()
	{return _zoneSizeY;}
	float GetZoneZ()
	{return _zoneSizeZ;}

	void SetZoneX(float zx)
	{_zoneSizeX = zx;}
	void SetZoneY(float zy)
	{_zoneSizeY = zy;}
	void SetZoneZ(float zz)
	{_zoneSizeZ = zz;}

	//! check if the actor need desactivation
	virtual bool NeedDesactivation(){return true;}


	// actor avtivate other actors
	virtual void ActorActivateActor(Actor * act);

	//! check if the actor is activated
	//! activating group is the group that the actiavating agent belows:
	//! 0 -> everybody;  1 -> player; 2 -> other actors
	virtual bool IsActivated(int activatinggroup);

protected:
	// used to get the zone center - depend of the actor type
	virtual float GetZoneCenterX(){return _posX;}
	virtual float GetZoneCenterY(){return _posY;}
	virtual float GetZoneCenterZ(){return _posZ;}

private:
	float	_zoneSizeX;
	float	_zoneSizeY;
	float	_zoneSizeZ;

	std::set<long>	_activatedactors;
};

#endif