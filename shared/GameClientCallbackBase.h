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

#ifndef _LBA_NET_GAME_CLIENT_CALLBACK_BASE_H_
#define _LBA_NET_GAME_CLIENT_CALLBACK_BASE_H_


#include <string>
#include <boost/shared_ptr.hpp>

class ObjectInfo;
class PhysicalObjectHandlerBase;
struct Input;


/***********************************************************************
 * Module:  GameClientCallbackBase.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: GameClientCallbackBase
 ***********************************************************************/
class GameClientCallbackBase
{

public:
	// constructor
	GameClientCallbackBase(){}

	// destructor
	virtual ~GameClientCallbackBase(){}

	// add an actor object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase>
			AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer) = 0;

	// remove an actor object
	virtual void RemObject(unsigned int id) = 0;

	// get last player inputs
	virtual Input GetLastPlayerInput() = 0;

	//apply inputs
	virtual void ApplyInputs(unsigned int time, const Input & in) = 0;
};

#endif

