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

#if !defined(__LbaNetModel_1_MapRenderer_h)
#define __LbaNetModel_1_MapRenderer_h

class LBA_MAP_GL;
class PhysicHandler;

#include <string>
#include <map>
#include "WorldInfo.h"
#include "MapRendererBase.h"


/***********************************************************************
 * Module:  MapRenderer.h
 * Author:  Vivien
 * Modified: dimanche 12 juillet 2009 21:09:21
 * Purpose: Declaration of the class MapRenderer
 ***********************************************************************/
class MapRenderer : public MapRendererBase
{
public:
	//! constructor
	MapRenderer(const std::string &filename, PhysicHandler * phH);

	//! destructor
	virtual ~MapRenderer();


	//! render
	virtual void Render();

	// cut the room at a certain Y to display only bottom
	virtual void SetMapYLimit(int YLimit);

	// flush the current map texture
	virtual void FlushTexture();

	//reload the current map texture
	virtual void ReloadTexture();


private:
	//! load new map
	bool LoadMap(const std::string &filename);

   //! cleanup
   void CleanUp();


private:
	LBA_MAP_GL *			_map_gl;
	int						_current_cut;
	std::string				_currentmap_file;

	PhysicHandler *			_phH;
};

#endif