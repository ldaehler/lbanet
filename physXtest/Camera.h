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


#ifndef _LBANET_CAMERA_H_
#define _LBANET_CAMERA_H_


class Actor;

//*************************************************************************************************
//*                               class Camera
//*************************************************************************************************
/**
* @brief Class taking care of the opengl scene camera
*
*/
class Camera
{
public:

	// constructor
	Camera();

	// reset camera position
	void ResetPosition();

	//! set actor attached to the camera
	void SetAttachedActor(Actor * act);

	//! process
	void Process();

	// accessors
	void SetTarget(double x, double y, double z);
	void SetDistance(double dist);
	void SetZenit(double zenit);
	void SetAzimut(double azimut);

	void DeltaUpdateTarget(double x, double y, double z);
	void DeltaUpdateDistance(double dist);
	void DeltaUpdateZenit(double zenit);
	void DeltaUpdateAzimut(double azimut);

	double GetTargetX();
	double GetTargetY();
	double GetTargetZ();
	double GetFOV();
	double GetDistance();
	double GetZenit();
	double GetAzimut();

	// tell if it is a perspective view or not
	bool IsPerspective();

	// set perspective
	void SetPerspective(bool Perspective);

private:
	// class members
	double _targetx;
	double _targety;
	double _targetz;
	double _fov;
	double _distance;
	double _zenit;
	double _azimut;
	double _size;

	bool _perpective;

	Actor * _attached_actor;
	double	_lastactX;
	double	_lastactY;
	double	_lastactZ;
	bool	_movecamera;
};


#endif
