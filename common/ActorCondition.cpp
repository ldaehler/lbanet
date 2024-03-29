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

#include "ActorCondition.h"


/***********************************************************
check if the condition is true or not
***********************************************************/
bool ActorActivatedCondition::Passed()
{
#ifndef _LBANET_SERVER_SIDE_
	return _actH->ActorActivated(_ActorId, _activatinggroup, _MapName);
#else
	// there is no way to know if the actor is activated on server side
	// so we have to trust the client in that case
	return true;
#endif
}