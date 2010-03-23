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

#ifndef _LBA_NET_CLIENT_LIST_HANDLER_BASE_H_
#define _LBA_NET_CLIENT_LIST_HANDLER_BASE_H_


#include <string>

/***********************************************************************
 * Module:  ClientListHandlerBase.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: base class to receive connection/disconnection event from other clients
 ***********************************************************************/
class ClientListHandlerBase
{

public:
	// constructor
	ClientListHandlerBase(){}

	// destructor
	virtual ~ClientListHandlerBase(){}

	// new client connected
	virtual void Connected(unsigned int id, const std::string & Name, 
							const std::string & Status, const std::string & Color) = 0;

	// client disconnected
	virtual void Disconnected(unsigned int id) = 0;

	// client changed status
	virtual void ChangedStatus(unsigned int id, const std::string & Status, const std::string & Color) = 0;


	// return the name given a client id
	virtual std::string GetName(unsigned int id) = 0;

};

#endif

