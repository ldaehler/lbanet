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

#ifndef __LBA_EXTERNAL_PLAYER__
#define __LBA_EXTERNAL_PLAYER__

#include <math.h>

#include "MagicBallHandler.h"

namespace LbaNet
{
	struct ActorInfo;
	struct ActorLifeInfo;
	struct LaunchInfo;
}
class Player;
class ActorUserData;
class NxActor;

class ExternalReckon
{
public:

	double			_time;
	float			_posX;
	float			_posY;
	float			_posZ;
	float			_rotation;

	//float			_velocityX;
	//float			_velocityY;
	//float			_velocityZ;
	float			_velocityR;


	float			_predicted_posX;
	float			_predicted_posY;
	float			_predicted_posZ;
	float			_predicted_rotation;


	// set reckon value
	void Set(const double & time, float posX, float posY, float posZ,
				float rotation/*, float velocityX, float velocityY,
				float velocityZ*/, float velocityR)
	{
		_time = time;

		_posX = posX;
		_posY = posY;
		_posZ = posZ;
		_predicted_posX = posX;
		_predicted_posY = posY;
		_predicted_posZ = posZ;

		_rotation = rotation;

		//_velocityX = velocityX;
		//_velocityY = velocityY;
		//_velocityZ = velocityZ;
		_velocityR = velocityR;
	}


	// calculate reackon prediction on each tick
	void Update(float vX, float vY, float vZ)
	{
		_predicted_posX += vX;
		_predicted_posY += vY;
		_predicted_posZ += vZ;
	}

	// calculate reackon prediction on each tick
	void UpdateRot(const double & currtime)
	{
		float tdiff = (float)(currtime - _time);

		_predicted_rotation = _rotation + (_velocityR*tdiff);

		if(_predicted_rotation > 360)
			_predicted_rotation = _predicted_rotation - 360;
		if(_predicted_rotation < 0)
			_predicted_rotation = 360 + _predicted_rotation;
	}
	

};


/*
************************************************************************************************************************
*                                                 class ExternalPlayer
************************************************************************************************************************
*/
class ExternalPlayer
{
public:
	//constructor
	ExternalPlayer(const LbaNet::ActorInfo & ainfo, float animationSpeed);

	//destructor
	~ExternalPlayer();


	// do all check to be done when idle
	int Process(double tnow, float tdiff);

	// update with external info
	void Update(const LbaNet::ActorInfo & ainfo);

	// update with external info
	void UpdateLife(const LbaNet::ActorLifeInfo & ainfo);

	// draw the object
    void draw(int RoomCut);

	Player * GetPlayer()
	{return _renderer;}

	// magic ball played
	void MagicBallPlayed(const LbaNet::LaunchInfo & linfo);
	void MagicBallComeback();

protected:
	void CalculateVelocity(float rotation);

private:
	double			_last_update;

	float 			_velocityX;
	float 			_velocityY;
	float 			_velocityZ;
	float 			_velocityR;

	bool			_forward;
	bool			_collisionx;
	bool			_collisionz;

	ExternalReckon	_dr;
	Player *		_renderer;

	ActorUserData * _usdata;
	NxActor*		_physH;

	MagicBallHandler _magicballH;
};


#endif