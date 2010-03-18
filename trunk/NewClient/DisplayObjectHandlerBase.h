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


#ifndef _LBANET_DISPLAY_OBJECT_HANDLER_BASE_H_
#define _LBANET_DISPLAY_OBJECT_HANDLER_BASE_H_

#include "CommonTypes.h"

//*************************************************************************************************
//*                               class DisplayObjectHandlerBase
//*************************************************************************************************
/**
* @brief Pure virtual class for handling the transformation of an object before displaying it
* (e.g. translation, rotation, etc.)
*
*/
class DisplayObjectHandlerBase
{
public:

	//! constructor
	DisplayObjectHandlerBase(){}

	//! destructor
	virtual ~DisplayObjectHandlerBase(){}

	//! set object position in the world
	virtual void SetPosition(float X, float Y, float Z) = 0;

	//! set object rotation on X axis
	virtual void SetRotation(const LbaQuaternion& Q) = 0;

	//! destroy function - clear the object content
	virtual void Destroy(void) = 0;

	//! set the object to be followed by the camera
	virtual void SetCameraFollow(void) = 0;
};


#endif
