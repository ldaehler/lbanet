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

#include "OSGHandler.h"
#include "LogHandler.h"

#include <osgViewer/Viewer>



#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


OsgHandler* OsgHandler::_singletonInstance = NULL;

/***********************************************************
singleton pattern
***********************************************************/
OsgHandler * OsgHandler::getInstance()
{
    if(!_singletonInstance)
        _singletonInstance = new OsgHandler();

	return _singletonInstance;
}


/***********************************************************
constructor
***********************************************************/
OsgHandler::OsgHandler()
: _isFullscreen(false), _resX(800), _resY(600),
	_isPerspective(true), _targetx(0), _targety(0), _targetz(0),
	_viewer(NULL)
{
	SetCameraDistance(30);
	SetCameraZenit(30);
}


/***********************************************************
destructor
***********************************************************/
OsgHandler::~OsgHandler()
{

}




/***********************************************************
initialize
***********************************************************/
void OsgHandler::Initialize(const std::string &WindowName)
{
	LogHandler::getInstance()->LogToFile("Initializing graphics window...");
	_viewer = new osgViewer::Viewer();

    // create the window to draw to.
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 10;
    traits->y = 10;
    traits->width = _resX;
    traits->height = _resY;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
	traits->windowName = "OSGTest";
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
    if (!gw)
    {
		LogHandler::getInstance()->LogToFile("Error: unable to create graphics window.");
        return;
    }

    _viewer->getCamera()->setGraphicsContext(gc.get());
	_viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));
    _viewer->getCamera()->setViewport(0,0,_resX,_resY);

	ResetScreen();
	LogHandler::getInstance()->LogToFile("Initializing of graphics window done.");
}


/***********************************************************
change screen resolution
***********************************************************/
void OsgHandler::Resize(int resX, int resY)
{
	_resX = resX;
	_resY = resY;

	if(_viewer)
	{
		ResetScreen();
		_viewer->getCamera()->setViewport(0,0,_resX,_resY);
	}
}


/***********************************************************
toggle fullscreen or windowed mode
***********************************************************/
void OsgHandler::ToggleFullScreen(bool Fullscreen)
{
	_isFullscreen = Fullscreen;
	ResetScreen();
}


/***********************************************************
set or reset screen
***********************************************************/
void OsgHandler::ResetScreen()
{
	if(!_viewer)
		return;

	LogHandler::getInstance()->LogToFile("Resetting screen resolution...");

	// iterate over all windows
	osgViewer::Viewer::Windows windows;
    _viewer->getWindows(windows);
    for(osgViewer::Viewer::Windows::iterator itr = windows.begin();  itr != windows.end(); ++itr)
    {
		osgViewer::GraphicsWindow *window = *itr;

		osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();
		if (wsi == NULL) 
		{
			LogHandler::getInstance()->LogToFile("Error, no WindowSystemInterface available, cannot toggle window resolution change.");
			return;
		}

		unsigned int    screenWidth;
		unsigned int    screenHeight;
		wsi->getScreenResolution(*(window->getTraits()), screenWidth, screenHeight);

		if(_isFullscreen)
		{
			window->setWindowDecoration(false);
			window->setWindowRectangle(0, 0, screenWidth, screenHeight);
		}
		else
		{
			window->setWindowDecoration(true);
			window->setWindowRectangle((screenWidth - _resX) / 2, (screenHeight - _resY) / 2, _resX, _resY);
		}
    }
}


/***********************************************************
set if the view is perspective or ortho
***********************************************************/
void OsgHandler::TogglePerspectiveView(bool Perspective)
{
	_isPerspective = Perspective;
}


/***********************************************************
set camera distance
***********************************************************/
void OsgHandler::SetCameraDistance(float distance)
{
	_distance = distance;
	int maxdistance = 150;
	if(_isPerspective)
		maxdistance = 1000;

	if(_distance < 10)
		_distance = 10;
	if(_distance > maxdistance)
		_distance = maxdistance;

	_fov=atan(40./_distance)*180./M_PI;
}


/***********************************************************
set camera zenit
***********************************************************/
void OsgHandler::SetCameraZenit(float zenit)
{
	_zenit = zenit;

	if(_zenit < 10)
		_zenit = 10;
	if(_zenit > 70)
		_zenit = 70;
}


/***********************************************************
set camera target
***********************************************************/
void OsgHandler::SetCameraTarget(float TargetX, float TargetY, float TargetZ)
{
	_targetx = TargetX;
	_targety = TargetY;
	_targetz = TargetZ;
}


/***********************************************************
clear all nodes of the display tree
typically called when changing map
***********************************************************/
void OsgHandler::EmptyDisplayTree()
{

}


/***********************************************************
set the current map to display
***********************************************************/
void OsgHandler::SetMap(osg::ref_ptr<osg::Node> mapnode)
{

}

