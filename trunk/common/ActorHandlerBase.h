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


#if !defined(__LbaNetModel_1_ActorHandlerBase_h)
#define __LbaNetModel_1_ActorHandlerBase_h


/***********************************************************************
 * Module:  ActorHandlerBase.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class ActorHandlerBase
 *********************************************************************/
class ActorHandlerBase
{
public:
	//! constructor
	ActorHandlerBase(){}

	//! destructor
	virtual ~ActorHandlerBase(){}

	//! check if the actor is activated
	//! activating group is the group that the actiavating agent belows:
	//! 0 -> everybody;  1 -> player; 2 -> other actors
	//! mapname gives the map where the actor should be located
	virtual bool ActorActivated(long ActorId, int activatinggroup, 
									const std::string & MapName) = 0;

};

#endif