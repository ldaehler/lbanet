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

#include "ConnectionHandler.h"
#include "LogHandler.h"

#include <zoidcom.h>



/***********************************************************
	Constructor
***********************************************************/
ConnectionHandler::ConnectionHandler(const std::string & LogFileName)
: _zcom(NULL)
{
	Init(LogFileName);
}

/***********************************************************
	destructor
***********************************************************/
ConnectionHandler::~ConnectionHandler()
{
	if(_zcom)
		delete _zcom;
}


/***********************************************************
	init the class
***********************************************************/
void ConnectionHandler::Init(const std::string & LogFileName)
{
	_zcom = new ZoidCom(LogFileName.c_str());
	if(!_zcom->Init())
	{
		LogHandler::getInstance()->LogToFile("Problem initializing Zoidcom.", 2);
	}

}
