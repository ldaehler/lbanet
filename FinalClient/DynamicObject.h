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

#if !defined(__LbaNetModel_1_DynamicObject_h)
#define __LbaNetModel_1_DynamicObject_h


#include "PhysicalObjectHandlerBase.h"
#include "DisplayObjectHandlerBase.h"

/***********************************************************************
 * Module:  DynamicObject.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Base class for any object needed update at each frame
 ***********************************************************************/
class DynamicObject
{
public:
	//!constructor
	DynamicObject(boost::shared_ptr<PhysicalObjectHandlerBase> phH,
					boost::shared_ptr<DisplayObjectHandlerBase> disH)
		: _phH(phH), _disH(disH)
	{}

	//!destructor
   virtual ~DynamicObject(){}

	//! process function - will be called at each frame
	virtual void Process(void) = 0;

	//! destroy function - clear the object content
	virtual void Destroy(void) = 0;

	//! get physical object
	boost::shared_ptr<PhysicalObjectHandlerBase> GetPhysicalObject()
	{ return _phH;}

	//! get display object
	boost::shared_ptr<DisplayObjectHandlerBase> GetDisplayObject()
	{ return _disH;}

protected:
	
	//! handler to physical object
	boost::shared_ptr<PhysicalObjectHandlerBase> _phH;

	//! handler to display object
	boost::shared_ptr<DisplayObjectHandlerBase> _disH;
};

#endif