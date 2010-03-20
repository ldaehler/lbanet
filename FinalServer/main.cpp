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
#include <boost/shared_ptr.hpp>

#include "ConnectionHandler.h"
#include "server.h"


int main(int argc, char *argv[])
{
	// set up connection class
	boost::shared_ptr<ConnectionHandler> ConH = boost::shared_ptr<ConnectionHandler>(new ConnectionHandler());

	// server operates on internal port 1 and UDP port 8899
	boost::shared_ptr<Server> Serv = boost::shared_ptr<Server>(new Server(1, 8899));


	// zoidcom needs to get called regularly to get anything done so we enter the mainloop now
	while (1)
	{
		// processes incoming packets
		// all callbacks are generated from within the processInput calls
		Serv->ZCom_processInput( eZCom_NoBlock );

		// outstanding data will be packed up and sent from here
		Serv->ZCom_processOutput();

		// pause the program for a few milliseconds
		ZoidCom::Sleep(10);
	}
}
