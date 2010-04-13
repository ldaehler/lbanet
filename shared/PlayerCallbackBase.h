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

#ifndef _LBA_NET_PLAYER_CALLBACK_BASE_H_
#define _LBA_NET_PLAYER_CALLBACK_BASE_H_

#include <boost/shared_ptr.hpp>

struct Input;
class PhysicalObjectHandlerBase;

/***********************************************************************
 * Module:  PlayerCallbackBase.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: base class to handle player inputs
 ***********************************************************************/
class PlayerCallbackBase
{

public:
	// constructor
	PlayerCallbackBase(){}

	// destructor
	virtual ~PlayerCallbackBase(){}


	// called when we get new friend in list
	virtual void inputUpdated(unsigned int time, const Input & newinput) = 0;


	// set physcial character
	virtual void SetPhysicalCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac) = 0;
};

#endif

