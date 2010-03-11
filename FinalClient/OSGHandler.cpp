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
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include <osgShadow/ShadowedScene>
#include <osgShadow/StandardShadowMap>
#include <osgShadow/ShadowMap>

#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;


OsgHandler* OsgHandler::_singletonInstance = NULL;










/***********************************************************
handle inputs
***********************************************************/
bool UserInputsHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::PUSH):
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{			
				_mouse_Y = ea.getY();
				_right_button_pressed = true;
				return true;
			}
		}

		case(osgGA::GUIEventAdapter::RELEASE):
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				_right_button_pressed = false;
				return true;
			}
		}

		// update Zenit on mouse move with right button pressed
		case(osgGA::GUIEventAdapter::DRAG):
		{
			if(_right_button_pressed)
			{
				OsgHandler::getInstance()->DeltaUpdateCameraZenit(_mouse_Y-ea.getY());
				_mouse_Y = ea.getY();
				return true;
			}
		}

		// zoom on mouse scroll
		case(osgGA::GUIEventAdapter::SCROLL):
		{
			if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
				OsgHandler::getInstance()->DeltaUpdateCameraDistance(5);

			if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
				OsgHandler::getInstance()->DeltaUpdateCameraDistance(-5);
		}
		 


		default:
			return false;
	}
}







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
	_isPerspective(false), _targetx(0), _targety(0), _targetz(0),
	_viewer(NULL), _rootNode(NULL), _sceneRootNode(NULL), _mapNode(NULL),
	_viewportX(800), _viewportY(600)
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
void OsgHandler::Initialize(const std::string &WindowName, const std::string &DataPath)
{
	osgDB::setDataFilePathList(DataPath);

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
	traits->windowName = WindowName;
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
    if (!gw)
    {
		LogHandler::getInstance()->LogToFile("Error: unable to create graphics context.");
        return;
    }

    _viewer->getCamera()->setGraphicsContext(gc.get());
	_viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));
    _viewer->getCamera()->setViewport(0,0,_resX,_resY);


	//// create the root node used for camera transformation
	_rootNode = new osg::PositionAttitudeTransform();
	_rootNode->setScale(osg::Vec3d(1, 0.5, 1));
	_rootNode->setAttitude(osg::Quat(osg::DegreesToRadians(-45.0), osg::Vec3(0,1,0)));

	_sceneRootNode = new osg::Group();
	_rootNode->addChild(_sceneRootNode);
	_viewer->setSceneData(_rootNode);

	

    // create a tracball manipulator to move the camera around in response to keyboard/mouse events
	//_camManip = new osgGA::TrackballManipulator();
	//_camManip->setHomePosition(osg::Vec3d(100, 25, 100), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));
	//_viewer->setCameraManipulator(_camManip);

	_viewer->setCameraManipulator(NULL);


	// add the stats handler
	_viewer->addEventHandler( new osgViewer::StatsHandler() );
	_viewer->addEventHandler( new UserInputsHandler() );

	 // create the windows
    _viewer->realize();


	// put everything in the right place
	ResetScreen();
	LogHandler::getInstance()->LogToFile("Initializing of graphics window done.");
}


/***********************************************************
change screen resolution
***********************************************************/
void OsgHandler::Resize(int resX, int resY)
{
	if(resX != _resX || resY != _resY)
	{
		_resX = resX;
		_resY = resY;

		ResetScreen();
	}
}


/***********************************************************
toggle fullscreen or windowed mode
***********************************************************/
void OsgHandler::ToggleFullScreen(bool Fullscreen)
{
	if(_isFullscreen != Fullscreen)
	{
		_isFullscreen = Fullscreen;
		ResetScreen();
	}
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
			_viewportX = screenWidth;
			_viewportY = screenHeight;
			
		}
		else
		{
			window->setWindowDecoration(true);
			window->setWindowRectangle((screenWidth - _resX) / 2, (screenHeight - _resY) / 2, _resX, _resY);
			_viewportX = _resX;
			_viewportY = _resY;
		}

		_viewer->getCamera()->setViewport(0,0,_viewportX,_viewportY);
    }

	ResetCameraProjectiomMatrix();
}



/***********************************************************
set or reset camera projection matrix
***********************************************************/
void OsgHandler::ResetCameraProjectiomMatrix()
{
	if(!_viewer)
		return;

	if(_isPerspective)
	{
		_viewer->getCamera()->setProjectionMatrixAsPerspective(_fov,_viewportX/(double)_viewportY, 0.01,2000);
	}
	else
	{
		_viewer->getCamera()->setProjectionMatrixAsOrtho(-0.12*_distance, 0.12*_distance,
															-0.12*_distance, 0.12*_distance,
															-2000, 2000);
	}

	ResetCameraTransform();
}



/***********************************************************
set or reset camera transform
***********************************************************/
void OsgHandler::ResetCameraTransform()
{
	if(_viewer)
	{
		osg::Matrixd viewMatrix;
		if(_isPerspective)
		{
			osg::Matrixd cameraRotation1;
			osg::Matrixd cameraTrans;

			cameraRotation1.makeRotate(osg::DegreesToRadians(_zenit), osg::Vec3(1,0,0));
			cameraTrans.makeTranslate( -_targetx,-_targety/2.0,-_targetz-_distance );
			viewMatrix = cameraRotation1* cameraTrans;
		}
		else
		{
			osg::Matrixd cameraRotation1;
			osg::Matrixd cameraTrans;

			cameraRotation1.makeRotate(osg::DegreesToRadians(30.0), osg::Vec3(1,0,0));
			cameraTrans.makeTranslate( -_targetx,-_targety/2.0,-_targetz-1000 );
			viewMatrix = cameraRotation1 * cameraTrans;
		}


		_viewer->getCamera()->setViewMatrix(viewMatrix);
	}
}



/***********************************************************
set if the view is perspective or ortho
***********************************************************/
void OsgHandler::TogglePerspectiveView(bool Perspective)
{
	if(_isPerspective != Perspective)
	{
		_isPerspective = Perspective;
		ResetCameraProjectiomMatrix();
	}
}

/***********************************************************
delta update camera distance
***********************************************************/
void OsgHandler::DeltaUpdateCameraDistance(double delta)
{
	SetCameraDistance(_distance+delta);
}


/***********************************************************
set camera distance
***********************************************************/
void OsgHandler::SetCameraDistance(double distance)
{
	if(_distance != distance)
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

		ResetCameraProjectiomMatrix();
	}
}


/***********************************************************
delta update camera zenit
***********************************************************/
void OsgHandler::DeltaUpdateCameraZenit(double delta)
{
	SetCameraZenit(_zenit+delta);
}

/***********************************************************
set camera zenit
***********************************************************/
void OsgHandler::SetCameraZenit(double zenit)
{
	if(_zenit != zenit)
	{
		_zenit = zenit;

		if(_zenit < 10)
			_zenit = 10;
		if(_zenit > 70)
			_zenit = 70;

		ResetCameraTransform();
	}
}


/***********************************************************
set camera target
***********************************************************/
void OsgHandler::SetCameraTarget(double TargetX, double TargetY, double TargetZ)
{
	_targetx = TargetX;
	_targety = TargetY;
	_targetz = TargetZ;

	ResetCameraTransform();
}


/***********************************************************
clear all nodes of the display tree
typically called when changing map
***********************************************************/
void OsgHandler::EmptyDisplayTree()
{
	if(!_sceneRootNode)
		return;

	std::vector<osg::ref_ptr<osg::Node> >::iterator itn = _addedNodes.begin();
	std::vector<osg::ref_ptr<osg::Node> >::iterator endn = _addedNodes.end();
	for(; itn != endn; ++itn)
		_sceneRootNode->removeChild(*itn);

	_addedNodes.clear();

	if(_mapNode)
	{
		_sceneRootNode->removeChild(_mapNode);
		_mapNode = NULL;
	}
}


/***********************************************************
set the current map to display
***********************************************************/
void OsgHandler::SetMap(osg::ref_ptr<osg::Node> mapnode)
{
	if(!_sceneRootNode)
		return;

	if(_mapNode)
		_sceneRootNode->removeChild(_mapNode);

	_mapNode = mapnode;
	_sceneRootNode->addChild(_mapNode);

	//_camManip->setNode(_mapNode);
}



/***********************************************************
update display - returns true if need to terminate
***********************************************************/
bool OsgHandler::Update()
{
	if(!_viewer->done())
	{
		_viewer->frame();
		return false;
	}

	return true;
}


/***********************************************************
load osg files into a osg node
***********************************************************/
osg::ref_ptr<osg::Node> OsgHandler::LoadOSGFile(const std::string & filename)
{
	return osgDB::readNodeFile(filename);
}