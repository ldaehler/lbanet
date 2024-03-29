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
#include "ConfigurationManager.h"
#include "EventHandler.h"


#include <osg/PositionAttitudeTransform>
#include <osg/ClipNode>


#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

#include <osgShadow/ShadowedScene>
#include <osgShadow/StandardShadowMap>
#include <osgShadow/ShadowMap>

#include <osgGA/GUIEventHandler>



#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;


OsgHandler* OsgHandler::_singletonInstance = NULL;








//*************************************************************************************************
//*                               class UserInputsHandler
//*************************************************************************************************
/**
* @brief Class taking care of user inputs (mouse and keyboard)
*
*/
// class to handle events with a pick
class UserInputsHandler : public osgGA::GUIEventHandler
{
public:
	//! constructor
    UserInputsHandler(EventHandler * evH)
		: _right_button_pressed(false), _evH(evH)
	{}

	//! destructor
    ~UserInputsHandler(){}

	//! handle inputs
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	bool	_right_button_pressed;
	int		_mouse_Y;
	EventHandler * _evH;
};







/***********************************************************
handle inputs
***********************************************************/
bool UserInputsHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if(_evH && _evH->Handle(ea, aa))
		return true;

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

			return true;
		}
		
		// toggle fullscreen on F12
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			if(ea.getKey() == osgGA::GUIEventAdapter::KEY_F12)
			{			
				OsgHandler::getInstance()->ToggleFullScreen();
				return true;
			}

			if(ea.getKey() == osgGA::GUIEventAdapter::KEY_F5)
			{			
				OsgHandler::getInstance()->ResetLight(true);
				return true;
			}

			if(ea.getKey() == osgGA::GUIEventAdapter::KEY_F6)
			{			
				OsgHandler::getInstance()->ResetLight(false);
				return true;
			}
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
	_viewer(NULL), _rootNode(NULL), _sceneRootNode(NULL), _translNode(NULL),
	_viewportX(800), _viewportY(600), _displayShadow(true), _movecamera(false),
	_current_clip_layer(-1)
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
void OsgHandler::Initialize(const std::string &WindowName, const std::string &DataPath, EventHandler * evH)
{
	osgDB::setDataFilePathList(DataPath);

	LogHandler::getInstance()->LogToFile("Initializing graphics window...");
	_viewer = new osgViewer::Viewer();


	// get data from configuration file
	{
		LogHandler::getInstance()->LogToFile("Initializing camera...");
		bool perspec;
		double camdistance, camzenit;
		ConfigurationManager::GetInstance()->GetDouble("Display.Camera.Distance", camdistance);
		ConfigurationManager::GetInstance()->GetDouble("Display.Camera.Zenit", camzenit);
		ConfigurationManager::GetInstance()->GetBool("Display.Camera.Perspective", perspec);
		SetCameraDistance(camdistance);
		SetCameraZenit(camzenit);
		TogglePerspectiveView(perspec);

		ConfigurationManager::GetInstance()->GetInt("Display.Screen.ScreenResolutionX", _resX);
		ConfigurationManager::GetInstance()->GetInt("Display.Screen.ScreenResolutionY", _resY);
		ConfigurationManager::GetInstance()->GetBool("Display.Screen.Fullscreen", _isFullscreen);

		ConfigurationManager::GetInstance()->GetBool("Display.ShadowOn", _displayShadow);
	}


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
	_translNode = new osg::PositionAttitudeTransform();
	_rootNode->addChild(_translNode);

	_viewer->setSceneData(_rootNode);
	_viewer->setCameraManipulator(NULL);



	_clipNode = new osg::ClipNode();
	_clipNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON|osg::StateAttribute::PROTECTED|osg::StateAttribute::OVERRIDE);
	_translNode->addChild(_clipNode);




	// add the stats handler
	_viewer->addEventHandler( new osgViewer::StatsHandler() );
	_viewer->addEventHandler( new UserInputsHandler(evH) );

	 // create the windows
	_viewer->setUpViewOnSingleScreen(0);

    _viewer->realize();
	_viewer->renderingTraversals();



	// put everything in the right place
	ResetScreen();
	LogHandler::getInstance()->LogToFile("Initializing of graphics window done.");
}



/***********************************************************
finalize function
***********************************************************/
void OsgHandler::Finalize()
{
	// write data to configuration file
	{
		ConfigurationManager::GetInstance()->SetDouble("Display.Camera.Distance", _distance);
		ConfigurationManager::GetInstance()->SetDouble("Display.Camera.Zenit", _zenit);
		ConfigurationManager::GetInstance()->SetBool("Display.Camera.Perspective", _isPerspective);


		ConfigurationManager::GetInstance()->SetInt("Display.Screen.ScreenResolutionX", _resX);
		ConfigurationManager::GetInstance()->SetInt("Display.Screen.ScreenResolutionY", _resY);
		ConfigurationManager::GetInstance()->SetBool("Display.Screen.Fullscreen", _isFullscreen);

		ConfigurationManager::GetInstance()->SetBool("Display.ShadowOn", _displayShadow);
	}

	// clean up everything
	ResetScreen();
	_sceneRootNode = NULL;
	_rootNode = NULL;
	_viewer = NULL;
	_translNode =NULL;
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
void OsgHandler::ToggleFullScreen()
{
	_isFullscreen = !_isFullscreen;
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
		window->grabFocus();
		window->raiseWindow();
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
			cameraTrans.makeTranslate( 0,0,-_distance );
			viewMatrix = cameraRotation1* cameraTrans;
		}
		else
		{
			osg::Matrixd cameraRotation1;
			osg::Matrixd cameraTrans;

			cameraRotation1.makeRotate(osg::DegreesToRadians(30.0), osg::Vec3(1,0,0));
			cameraTrans.makeTranslate( 0,0,-1000 );
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
		if(!_isPerspective)
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

	//ResetCameraTransform();
	if(_translNode)
		_translNode->setPosition(osg::Vec3d( -_targetx,-_targety,-_targetz ));
}


/***********************************************************
reset light (can be used to switch light on/off
***********************************************************/
void OsgHandler::ResetLight(bool On)
{
	if(!_lightNode)
		return;


	if(On)
	{
		osg::Light* myLight1 = new osg::Light();
		myLight1->setLightNum(0);

		osg::Vec4 lightpos;
		lightpos.set(_currLightInfo.LOnPosX, _currLightInfo.LOnPosY, _currLightInfo.LOnPosZ,0.0f);
		myLight1->setPosition(lightpos);
		myLight1->setAmbient(osg::Vec4(_currLightInfo.LOnAmbientR,_currLightInfo.LOnAmbientG,_currLightInfo.LOnAmbientB, 1.0));
		myLight1->setDiffuse(osg::Vec4(_currLightInfo.LOnDiffuseR,_currLightInfo.LOnDiffuseG,_currLightInfo.LOnDiffuseB, 1.0));
		_lightNode->setLight(myLight1);
		_lightNode->setLocalStateSetModes(osg::StateAttribute::ON);
		_lightNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON);
	}
	else
	{
		osg::Light* myLight1 = new osg::Light();
		myLight1->setLightNum(0);

		osg::Vec4 lightpos;
		lightpos.set(_currLightInfo.LOffPosX, _currLightInfo.LOffPosY, _currLightInfo.LOffPosZ,0.0f);
		myLight1->setPosition(lightpos);
		myLight1->setAmbient(osg::Vec4(_currLightInfo.LOffAmbientR,_currLightInfo.LOffAmbientG,_currLightInfo.LOffAmbientB, 1.0));
		myLight1->setDiffuse(osg::Vec4(_currLightInfo.LOffDiffuseR,_currLightInfo.LOffDiffuseG,_currLightInfo.LOffDiffuseB, 1.0));
		_lightNode->setLight(myLight1);
		_lightNode->setLocalStateSetModes(osg::StateAttribute::ON);	
		_lightNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON);
	}

}


/***********************************************************
clear all nodes of the display tree
typically called when changing map
***********************************************************/
void OsgHandler::ResetDisplayTree(const LbaMainLightInfo &NewLightningInfo)
{
	//reset cmera node
	_cameraNode = NULL;


	if(!_translNode)
		return;

	if(_lightNode)
		_translNode->removeChild(_lightNode);

	_lightNode = new osg::LightSource();
	_translNode->addChild(_lightNode);



	// reset light information
	_currLightInfo = NewLightningInfo;
	if(_currLightInfo.UseLight)
	{
		ResetLight(_currLightInfo.StartOn);
	}
	else
	{
		_lightNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
	}


	// check if we use shadow or not
	if(_currLightInfo.UseLight && _currLightInfo.UseShadow && _displayShadow)
	{
		osg::ref_ptr<osgShadow::ShadowedScene> shadowscene = new osgShadow::ShadowedScene();
		shadowscene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
		shadowscene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

		osg::ref_ptr<osgShadow::StandardShadowMap> shmap = new osgShadow::StandardShadowMap();
		shadowscene->setShadowTechnique(shmap.get());
		_sceneRootNode = shadowscene;
	}
	else
	{
		_sceneRootNode = new osg::Group();	
	}

	_lightNode->addChild(_sceneRootNode);





	//osg::Light* myLight2 = new osg::Light;
	//myLight2->setLightNum(1);
	//osg::Vec4 lightpos;
	//lightpos.set(10.0f,10.0f,10.0f,1.0f);
	//osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
	//myLight2->setLightNum(1);
	//myLight2->setPosition(lightpos);
	//myLight2->setDirection(osg::Vec3(-0.5, -0.5, 0));
	//myLight2->setAmbient(osg::Vec4(0.8,0.2,0.2,1.0));
	//myLight2->setDiffuse(osg::Vec4(0.8,0.8,0.8,1.0));
	//myLight2->setSpotCutoff(20.0f);
	//myLight2->setSpotExponent(50.0f);

	//ls->setLight(myLight2);
	//ls->setLocalStateSetModes(osg::StateAttribute::ON); 
	//ls->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON);

	//_rootNode->addChild(ls);
}



/***********************************************************
update display - returns true if need to terminate
***********************************************************/
bool OsgHandler::Update()
{
	UpdateCameraPos();

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



/***********************************************************
add a actor to the display list - return handler to actor position
***********************************************************/
osg::ref_ptr<osg::MatrixTransform> OsgHandler::AddActorNode(osg::ref_ptr<osg::Node> node)
{
	node->setNodeMask(ReceivesShadowTraversalMask | CastsShadowTraversalMask);
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
	transform->addChild(node);
	_sceneRootNode->addChild(transform);




	return transform;
}


/***********************************************************
remove actor from the graph
***********************************************************/
void OsgHandler::RemoveActorNode(osg::ref_ptr<osg::Node> node)
{
	_sceneRootNode->removeChild(node);
}



/***********************************************************
set the node the camera needs to follow
***********************************************************/
void OsgHandler::SetCameraFollowingNode(osg::ref_ptr<osg::MatrixTransform> node)
{
	_cameraNode = node;
}



/***********************************************************
update camera position
***********************************************************/
void OsgHandler::UpdateCameraPos()
{
	if(!_cameraNode)
		return;

	osg::Vec3d apos = _cameraNode->getMatrix().getTrans();
	double actX = apos.x();
	double actY = apos.y();
	double actZ = apos.z();

	// start to move camera only when actor moves a certain distance
	if(abs(actX - _targetx) > 3 || abs(actY - _targety) > 3 || abs(actZ - _targetz) > 3)
	{
		_movecamera = true;
	}

	if(abs(actX - _targetx) > 5 || abs(actY - _targety) > 5 || abs(actZ - _targetz) > 5)
	{
		SetCameraTarget(actX, actY, actZ);
		_movecamera = false;
		return;
	}


	if(_movecamera)
	{
		double speedX = (actX - _lastactX);
		double speedY = (actY - _lastactY);
		double speedZ = (actZ - _lastactZ);
		SetCameraTarget(_targetx+speedX, _targety+speedY, _targetz+speedZ);

		double deltaX = (actX - _targetx);
		double deltaY = (actY - _targety);
		double deltaZ = (actZ - _targetz);

		bool changed = false;
		if(abs(deltaX) > 0.1)
		{
			changed = true;
			_targetx+=deltaX/100;
		}

		if(abs(deltaY) > 0.1)
		{
			changed = true;
			_targety+=deltaY/100;
		}
		if(abs(deltaZ) > 0.1)
		{
			changed = true;
			_targetz+=deltaZ/100;
		}

		if(changed)
			SetCameraTarget(_targetx, _targety, _targetz);


		if(actX == _lastactX && actY == _lastactY && actZ == _lastactZ)
		{
			_movecamera = false;		
		}
	}

	_lastactX = actX;
	_lastactY = actY;
	_lastactZ = actZ;
}



/***********************************************************
set clip plane cut layer
***********************************************************/
void OsgHandler::SetClipPlane(float layer)
{
	if(_current_clip_layer == layer)
		return;

	_current_clip_layer = layer;

	_clipNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::OFF);
	_clipNode->removeClipPlane((unsigned int)0);


	if(layer > 0)
	{
		osg::ref_ptr<osg::ClipPlane> clipplane = new osg::ClipPlane();
		clipplane->setClipPlane(0, -1, 0, layer+0.01);
		_clipNode->addClipPlane(clipplane);
		_clipNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON);
	}
}


/***********************************************************
set screen attributes
***********************************************************/
void OsgHandler::SetScreenAttributes(int resX, int resY, bool fullscreen)
{
	_isFullscreen = fullscreen;
	_resX = resX;
	_resY = resY;
	ResetScreen();
}



/***********************************************************
set screen attributes
***********************************************************/
void OsgHandler::GetScreenAttributes(int &resX, int &resY, bool &fullscreen)
{
	fullscreen = _isFullscreen;
	resX = _resX;
	resY = _resY;
}
