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


#include "PermissionsVerifierServant.h"


/***********************************************************
constructor
***********************************************************/
PermissionsVerifierServant::PermissionsVerifierServant(SharedData * shd)
:_shd(shd)
{

}

/***********************************************************
check user permission
***********************************************************/
bool PermissionsVerifierServant::checkPermissions(const std::string& userId, const std::string& passwd,
												  std::string& reason, const Ice::Current&) const
{
	if(userId == "")
	{
		reason = "Please provide username.";
		return false;
	}

	bool res = _shd->TryLogin(userId);
	if(!res)
		reason = "Username already logged in.";

	return res;
}