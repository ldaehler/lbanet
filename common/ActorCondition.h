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


#if !defined(__LbaNetModel_1_ActorCondition_h)
#define __LbaNetModel_1_ActorCondition_h

#include "ConditionBase.h"
#include "ActorHandlerBase.h"

#include <string>

/***********************************************************************
 * Module:  ActorActivatedCondition.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class InventoryCondition
 *********************************************************************/
class ActorActivatedCondition : public ConditionBase
{
public:
	//! constructor
	//! activating group is the group that the actiavating agent belows:
	//! 0 -> everybody;  1 -> player; 2 -> other actors
	//! mapname gives the map where the actor should be located
	ActorActivatedCondition(long ActorId, int activatinggroup, const std::string & MapName,
								ActorHandlerBase *  actH)
		: _ActorId(ActorId), _activatinggroup(activatinggroup), _MapName(MapName), _actH(actH)
	{}

	//! destructor
	virtual ~ActorActivatedCondition(){}

	//! check if the condition is true or not
	virtual bool Passed();

private:
	long _ActorId;
	int _activatinggroup;
	std::string _MapName;
	ActorHandlerBase *  _actH;
};

#endif