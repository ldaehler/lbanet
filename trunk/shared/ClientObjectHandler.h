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

#ifndef _LBA_NET_CLIENT_OBJECT_HANDLER_H_
#define _LBA_NET_CLIENT_OBJECT_HANDLER_H_

class ClientObject;

#include <map>


/***********************************************************************
 * Module:  ClientObjectHandler.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class ClientObjectHandler
 ***********************************************************************/
class ClientObjectHandler
{

public:
	// constructor
	ClientObjectHandler();

	// destructor
	~ClientObjectHandler();


	// do a process step
	 void Process();

	// add a client
	 void Addclient(unsigned int id, ClientObject * cl);

	// remove a client
	 void Removeclient(unsigned int id);

	// get a client
	 // return NULL if does not exist
	 ClientObject * Getclient(unsigned int id);

private:

	// channel map
	std::map<unsigned int, ClientObject *> _clients;
};


#endif