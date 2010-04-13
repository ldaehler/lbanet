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

#ifndef _LBA_NET_SERVER_PLAYER_WRAP_H_
#define _LBA_NET_SERVER_PLAYER_WRAP_H_

#include "PlayerCallbackBase.h"
#include "CharacterController.h"

class PhysXEngine;
class PlayerInfoHandler;

/***********************************************************************
 * Module:  ServerDataHandler.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Read data from file
 ***********************************************************************/
class PlayerServerWrapper : public PlayerCallbackBase
{

public:
	// constructor
	PlayerServerWrapper(boost::shared_ptr<PhysXEngine> pEngine, boost::shared_ptr<PlayerInfoHandler> pinfo);


	// destructor
	~PlayerServerWrapper();


	// called when we get new friend in list
	virtual void inputUpdated(unsigned int time, const Input & newinput);

	// set physcial character
	virtual void SetPhysicalCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac);

private:
	CharacterController _controller;
	boost::shared_ptr<PhysXEngine> _pEngine;
	boost::shared_ptr<PlayerInfoHandler> _pinfo;
};

#endif