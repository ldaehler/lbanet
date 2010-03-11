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


#ifndef _LBANET_WORLD_TO_DISPLAY_OBJECT_SYNCHRONIZER_H_
#define _LBANET_WORLD_TO_DISPLAY_OBJECT_SYNCHRONIZER_H_

#include <boost/shared_ptr.hpp>

#include "PhysicalObjectHandlerBase.h"
#include "DisplayObjectHandlerBase.h"
#include "DynamicObject.h"

//*************************************************************************************************
//*                               class WorldToDisplayObjectSynchronizer
//*************************************************************************************************
/**
* @brief Class used to synchronize an object present in the physical world to the screen display
*
*/
class WorldToDisplayObjectSynchronizer : public DynamicObject
{
public:

	//! constructor
	WorldToDisplayObjectSynchronizer(boost::shared_ptr<PhysicalObjectHandlerBase> phH,
										boost::shared_ptr<DisplayObjectHandlerBase> disH);

	//! destructor
	virtual ~WorldToDisplayObjectSynchronizer();


	//! synchronization function - will typically be called on every frames
	virtual void Process(void);


protected:
	//! directly synchronize value between physic and display
	void StraightSync();

	//! synchronize value between physic and display with smoothing factor
	void SyncWithSmoothing();

	
	//! test for floating point equality within [-epsilon,+epsilon]
		inline bool equal(float a, float b)
	{
		static const float epsilon = 0.01f;  // we do not need very big precision to display object

		const float d = a - b;
		if (d<epsilon && d>-epsilon) 
			return true;
		else 
			return false;
	}


private:
	//keep track of the last synchronized position
	float	_lastDisplayPositionX; 
	float	_lastDisplayPositionY;
	float	_lastDisplayPositionZ;

	//keep track of the last synchronized rotation
	float	_lastDisplayRotationX;
	float	_lastDisplayRotationY;
	float	_lastDisplayRotationZ;

	//! handler to physical object
	boost::shared_ptr<PhysicalObjectHandlerBase> _phH;

	//! handler to display object
	boost::shared_ptr<DisplayObjectHandlerBase> _disH;
};


#endif
