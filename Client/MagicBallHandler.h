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

#ifndef __LBA_NET_MAGICBALL_HANDLER_H__
#define __LBA_NET_MAGICBALL_HANDLER_H__

class NxActor;
class ActorUserData;
class Actor;

#include "PhysicCallbackBase.h"


class MagicBallHandler : public PhysicCallbackBase
{
public: 
	//! constructor
	MagicBallHandler()
		: _launched(false), _comeback(false), _owner(NULL)
	{}

	//! destructor
	virtual ~MagicBallHandler()
	{
		cleanPhys();
	}

	//! set ball owner
	void SetOwner(Actor * owner)
	{
		_owner = owner;
	}

	//! render
	void Render();

	//! launch the magic ball
	void Launch(float PosX, float PosY, float PosZ, float dirX, float dirZ);

	//! proccess magic ball
	void Process();

	//! clear the magic ball if launched (e.g we change map)
	void Clear();

	//! check if already launched
	bool Launched()
	{ return _launched;}

	//! callback function
	virtual void CallbackOnContact(int TouchedActorType, long TouchedActorIdx);

protected:
	//! clean physycall shape if needed
	void cleanPhys();

	//! make ball come back
	void BallComeBack();


private:
	bool	_launched;
	double	_lastlaunchtime;
	double	_lasttime;
	bool	_comeback;
	int		_touch_counter;

	float	_currX;
	float	_currY;
	float	_currZ;


	NxActor* _physH;
	ActorUserData * _physdata;

	Actor * _owner;
};



#endif
