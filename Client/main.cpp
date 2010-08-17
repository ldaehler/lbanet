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

#if defined(NDEBUG)
	#if defined(_MSC_VER)
		#include "SDL.h"
	#else
		#include "SDL/SDL.h"
	#endif
#endif

#include "RunApp.h"

#include "LogHandler.h"
#include "crashrpt.h"
#pragma comment(lib, "crashrpt.lib")


BOOL WINAPI CrashCallback(LPVOID lpvState)
{
	LogHandler::getInstance()->CloseFile();
	AddFile(lpvState, LogHandler::getInstance()->GetFilename().c_str(), "Lbanet general log");
	//AddFile(lpvState, LogHandler::getInstance()->GetGUIFilename().c_str(), "GUI log");
	return TRUE;
}

int main(int argc, char *argv[])
{
	// init crash reporter
	LPVOID chandler = Install(CrashCallback, NULL, NULL);

	//GenerateErrorReport(chandler, NULL);

	//launch application
	return RunApp::Run(argc, argv);
}






//TODO:

//- others actors and sprites should disapear if the room is cut to half

//-actor - check if it is possible to change fixed shadow on twinsen for dynamic one

//- play with light and fog
