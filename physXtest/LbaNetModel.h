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

#if !defined(__LbaNetModel_1_LbaNetModel_h)
#define __LbaNetModel_1_LbaNetModel_h


#include <string>
#include <vector>
#include <map>

class Camera;


/***********************************************************************
 * Module:  LbaNetModel.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 13:54:52
 * Purpose: Declaration of the class LbaNetModel
 ***********************************************************************/
class LbaNetModel
{
public:
	//! constructor
	LbaNetModel();

	//! destructor
	~LbaNetModel();

	//! init function
	void Initialize();

	//! set screen size
	void SetScreenSize(int screenX, int screenY);

	//! draw the current state on the screen
	void Draw();

	// update camera functions
	void DeltaUpdateZoom(bool ZoomIn);
	void DeltaUpdateZenit(double zenit);

private:
	Camera *				_camera;

	int						_windowWidth;
	int						_windowHeight;
};

#endif