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

#if !defined(__LbaNet_Connection_Handler_h)
#define __LbaNet_Connection_Handler_h

class ZoidCom;
#include <string>

/***********************************************************************
 * Module:  ConnectionHandler.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Declaration of the class ConnectionHandler
 ***********************************************************************/
class ConnectionHandler
{
public:
	//! constructor
	ConnectionHandler(const std::string & LogFileName);

	//! destructor
	~ConnectionHandler();

protected:
	// init the class
	void Init(const std::string & LogFileName);

private:
	ZoidCom *	_zcom;
};

#endif