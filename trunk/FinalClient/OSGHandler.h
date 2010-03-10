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


#ifndef _LBANET_OSG_HANDLER_H_
#define _LBANET_OSG_HANDLER_H_

#include <string>
#include <osg/ref_ptr>

namespace osg
{
	class Node;
}

namespace osgViewer
{
	class Viewer;
}

//*************************************************************************************************
//*                               class OsgHandler
//*************************************************************************************************
/**
* @brief Class takign care of the OSG 3D engine
*
*/
class OsgHandler
{
public:

	// singleton pattern
	static OsgHandler * getInstance();

	//! destructor
	~OsgHandler();

	//! initialize
	void Initialize(const std::string &WindowName);

	//! change screen resolution
	void Resize(int resX, int resY);

	//! toggle fullscreen or windowed mode
	void ToggleFullScreen(bool Fullscreen);
	
	//! set if the view is perspective or ortho
	void TogglePerspectiveView(bool Perspective);

	//! set camera distance
	void SetCameraDistance(float distance);

	//! set camera zenit
	void SetCameraZenit(float zenit);

	//! set camera target
	void SetCameraTarget(float TargetX, float TargetY, float TargetZ);

	//! clear all nodes of the display tree
	//! typically called when changing map
	void EmptyDisplayTree();

	//! set the current map to display
	void SetMap(osg::ref_ptr<osg::Node> mapnode);

protected:
	//! constructor
	OsgHandler();

	//! set or reset screen
	void ResetScreen();

private:
	// singleton
	static OsgHandler * _singletonInstance;

	
	// screen info
	bool	_isFullscreen;
	int		_resX;
	int		_resY;

	// camera info
	bool	_isPerspective;
	double	_targetx;
	double	_targety;
	double	_targetz;
	double	_fov;
	double	_distance;
	double	_zenit;


	// osg handlers
	osg::ref_ptr<osgViewer::Viewer>	_viewer;
};


#endif
