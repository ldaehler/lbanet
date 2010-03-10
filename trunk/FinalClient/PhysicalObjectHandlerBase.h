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


#ifndef _LBANET_PHYSICAL_OBJECT_HANDLER_BASE_H_
#define _LBANET_PHYSICAL_OBJECT_HANDLER_BASE_H_


//*************************************************************************************************
//*                               class PhysicalObjectHandlerBase
//*************************************************************************************************
/**
* @brief Pure virtual class to get access to a physical object current transformation
* (e.g. translation, rotation, etc.)
*
*/
class PhysicalObjectHandlerBase
{
public:

	//! constructor
	PhysicalObjectHandlerBase(){}

	//! destructor
	virtual ~PhysicalObjectHandlerBase(){}

	//! get object position in the world
	virtual void GetPosition(float &X, float &Y, float &Z) = 0;

	//! get object rotation on X axis
	virtual float GetRotationX() = 0;

	//! get object rotation on X axis
	virtual float GetRotationY() = 0;

	//! get object rotation on X axis
	virtual float GetRotationZ() = 0;


	//! call to see if the object was resetted in the physical world
	//! (e.g. object has been teleported) in this case the synchronization process
	//! would directly set display object to the new position without smoothing
	virtual bool WasReseted() = 0;
};


#endif