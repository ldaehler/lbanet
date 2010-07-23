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


#include "MailboxActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
MailboxActor::MailboxActor(float activationdistance, int activationtype)
: ActivableActor(activationdistance, activationtype), _activated(false)
{

}


/***********************************************************
	Destructor
***********************************************************/
MailboxActor::~MailboxActor()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_activated)
	{
		ThreadSafeWorkpile::getInstance()->AddEvent(new CloseMailEvent());
	}
#endif
}

/***********************************************************
process activation
***********************************************************/
void MailboxActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	ThreadSafeWorkpile::getInstance()->AskPMs();
	_activated = true;
#endif
}


/***********************************************************
check zone activation
***********************************************************/
int MailboxActor::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
									MainPlayerHandler  * _mph, bool DirectActivation)
{

#ifndef _LBANET_SERVER_SIDE_
	if(_activated)
	{
		float distX = _posX-PlayerPosX;
		float distY = _posY-PlayerPosY;
		float distZ = _posZ-PlayerPosZ;

		double distance = (distX * distX) + (distY * distY) + (distZ * distZ);
		if(distance > _activationdistance)
		{
			ThreadSafeWorkpile::getInstance()->AddEvent(new CloseMailEvent());
			_activated = false;
		}
	}
#endif


	return 0;
}