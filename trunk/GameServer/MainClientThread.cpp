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

#include "MainClientThread.h"
#include "MainServerclient.h"

#define	_CUR_LBANET_SERVER_VERSION_ "v0.8"

/***********************************************************************
 * Constructor
 ***********************************************************************/
MainClientThread::MainClientThread(const std::string & MainAddress, const std::string & GameServerName,
						const std::string & GameServerAddress, bool Advertize)
	: _continue(true), _GameServerName(GameServerName), _GameServerAddress(GameServerAddress), 
		_Advertize(Advertize)
{
	_client = new MainServerClient(30, 200, this);
	_client->ConnectToServer(MainAddress, "GameServer", "GM2SVL2x", _CUR_LBANET_SERVER_VERSION_);
}



/***********************************************************************
 * destructor
 ***********************************************************************/
MainClientThread::~MainClientThread()
{
}

/***********************************************************************
 * run function
 ***********************************************************************/
void MainClientThread::Run()
{
	while(_continue)
	{
		_client->ZCom_processInput( eZCom_NoBlock );

		//process internal stuff
		_client->Process();

		// do advertizement
		if(_Advertize)
			_client->Advertize(_GameServerName, _GameServerAddress);
		else
			_client->Deadvertize(_GameServerName);


		// outstanding data will be packed up and sent from here
		_client->ZCom_processOutput();

		// sleep a bit
		ZoidCom::Sleep(200);
	}

	_client->CloseConnection();
	delete _client;
}



/***********************************************************************
 * received aknowledgement
 ***********************************************************************/
void MainClientThread::Aknowldeged(bool Ok)
{
	_continue = false;
}