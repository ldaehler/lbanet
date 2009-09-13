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

#include "Camera.h"

#include <iostream>
#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

/***********************************************************
constructor
***********************************************************/
Camera::Camera()
: _size(80.), _perpective(true)
{
	SetTarget(32., 0, 32.);
	SetDistance(30);
	SetZenit(30);
	SetAzimut(0);
}

/***********************************************************
accessors
***********************************************************/
void Camera::SetTarget(double x, double y, double z)
{
	_targetx = x;
	_targety = y;
	_targetz = z;

	if(_targetx < -10)
		_targetx = -10;
	if(_targety < -10)
		_targety = -10;
	if(_targetz < -10)
		_targetz = -10;

	//if(_targetx > 70)
	//	_targetx = 70;
	//if(_targety > 70)
	//	_targety = 70;
	//if(_targetz > 70)
	//	_targetz = 70;

}

void Camera::SetDistance(double dist)
{
	_distance = dist;
	int maxdistance = 150;
	if(!IsPerspective())
		maxdistance = 1000;

	if(_distance < 10)
		_distance = 10;
	if(_distance > maxdistance)
		_distance = maxdistance;

	_fov=atan(_size/2./_distance)*180./M_PI;
}

void Camera::SetZenit(double zenit)
{
	_zenit = zenit;

	if(_zenit < 10)
		_zenit = 10;
	if(_zenit > 70)
		_zenit = 70;
}

void Camera::SetAzimut(double azimut)
{
	_azimut = azimut;

	if(_azimut < -45)
		_azimut = -45;
	if(_azimut > 90)
		_azimut = 90;
}


void Camera::DeltaUpdateTarget(double x, double y, double z)
{
	SetTarget(_targetx+x, _targety+y,_targetz+z);
}

void Camera::DeltaUpdateDistance(double dist)
{
	SetDistance(_distance+dist);
}

void Camera::DeltaUpdateZenit(double zenit)
{
	SetZenit(_zenit+zenit);
}

void Camera::DeltaUpdateAzimut(double azimut)
{
	SetAzimut(_azimut+azimut);
}



double Camera::GetTargetX()
{
	return _targetx;
}

double Camera::GetTargetY()
{
	return _targety;
}

double Camera::GetTargetZ()
{
	return _targetz;
}

double Camera::GetFOV()
{
	return _fov;
}

double Camera::GetDistance()
{
	return _distance;
}

double Camera::GetZenit()
{
	return _zenit;
}

double Camera::GetAzimut()
{
	return _azimut;
}



// tell if it is a perspective view or not
bool Camera::IsPerspective()
{
	return _perpective;
}

// set perspective
void Camera::SetPerspective(bool Perspective)
{
	_perpective = Perspective;
}