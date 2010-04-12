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

#include "RoomCutController.h"
#include "DynamicObject.h"
#include "PhysXEngine.h"
#include "OSGHandler.h"

#include <iostream>

/***********************************************************
	Constructor
***********************************************************/
RoomCutController::RoomCutController(boost::shared_ptr<PhysXEngine> pEngine)
:	_pEngine(pEngine)
{

}


/***********************************************************
	Destructor
***********************************************************/
RoomCutController::~RoomCutController()
{

}


/***********************************************************
	Set character to control
***********************************************************/
void RoomCutController::SetCharacter(boost::shared_ptr<DynamicObject> charac, bool AsGhost)
{
	_character = charac;
	_isGhost = AsGhost;
}



/***********************************************************
process function
***********************************************************/
void RoomCutController::Process()
{
	if(_isGhost)
		return;

	if(!_character)
		return;

	boost::shared_ptr<PhysicalObjectHandlerBase> phys = _character->GetPhysicalObject();
	if(!phys)
		return;

	float PositionX, PositionY, PositionZ;
	phys->GetPosition(PositionX, PositionY, PositionZ);
	float roofcut = _pEngine->CheckForRoof(PositionX, PositionY, PositionZ);
	OsgHandler::getInstance()->SetClipPlane(roofcut-1);
}