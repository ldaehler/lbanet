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


#include "TextActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
TextActor::TextActor(float activationdistance, long textid, int activationtype)
: _textid(textid), ActivableActor(activationdistance, activationtype)
{

}


/***********************************************************
	Destructor
***********************************************************/
TextActor::~TextActor()
{

}

/***********************************************************
process activation
***********************************************************/
void TextActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGameTextEvent(_textid));
#endif
}