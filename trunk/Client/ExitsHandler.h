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

#if !defined(__LbaNetModel_1_ExitsHandler_h)
#define __LbaNetModel_1_ExitsHandler_h

class LBA_MAP_GL;
class PhysicHandler;

#include <string>
#include <map>
#include "WorldInfo.h"


/***********************************************************************
 * Module:  ExitsHandler.h
 * Author:  Vivien
 * Modified: dimanche 12 juillet 2009 21:09:21
 * Purpose: Declaration of the class ExitsHandler
 ***********************************************************************/
class ExitsHandler
{
public:
	//! constructor
   ExitsHandler();

	//! destructor
   ~ExitsHandler();

   //! load new map
   void LoadMap(const MapInfo * mapinfo);

   //! render
   void Render();

   // check if the main actor is exiting the room
	// return the number of the new room if applicable
	bool CheckIfExitRoom(float currX, float currY, float currZ,
							std::string & NewRoomName,
							std::string & NewSpawningPoint,
							float &Xoffset, float &Yoffset, float &Zoffset);


	//! render editor part
	void RenderEditor();


	// display map extis
	void DisplayExits(bool display);

protected:
	// display exit zones on the screen
	void DisplayExitZones();

	// display spawning on the screen
	void DisplaySpawnings();

	// display exit zones on the screen
	void DisplayDetailsExitZones();

private:

	const MapInfo *			_mapinfo;
	bool					_display_exits;
};

#endif