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


#ifndef _LBANET_MAIN_client_T_h_
#define _LBANET_MAIN_client_T_h_

class MainServerClient;

#include "GameServerCallbackBase.h"

/***********************************************************************
 * Module:  MainClientThread.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: thread used to contact main server
 ***********************************************************************/
class MainClientThread : public GameServerCallbackBase
{
public:

	//! constructor
	MainClientThread(const std::string & MainAddress, const std::string & GameServerName,
						const std::string & GameServerAddress, bool Advertize);

	//!destructor
	~MainClientThread();

	//! run function
	void Run();


	// received server address
	virtual void ReceivedAddress(const std::string & Server, const std::string & Address){}

	// received aknowledgement
	virtual void Aknowldeged(bool Ok);

private:
	MainServerClient *	_client;
	bool				_continue;

	std::string		_GameServerName;
	std::string		_GameServerAddress;
	bool			_Advertize;
};

#endif
