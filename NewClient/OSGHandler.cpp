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
#include "GuiHandler.h"

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


#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif


#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;


OsgHandler* OsgHandler::_singletonInstance = NULL;




//*************************************************************************************************
//*                               convert SDL event to OSG event
//*************************************************************************************************/
bool convertEvent(SDL_Event& event, osgGA::EventQueue& eventQueue)
{
    switch (event.type) {

        case SDL_MOUSEMOTION:
            eventQueue.mouseMotion(event.motion.x, event.motion.y);
            return true;

        case SDL_MOUSEBUTTONDOWN:
            eventQueue.mouseButtonPress(event.button.x, event.button.y, event.button.button);
            return true;

        case SDL_MOUSEBUTTONUP:
            eventQueue.mouseButtonRelease(event.button.x, event.button.y, event.button.button);
            return true;

        case SDL_KEYUP:
            eventQueue.keyRelease( (osgGA::GUIEventAdapter::KeySymbol) event.key.keysym.unicode);
            return true;

        case SDL_KEYDOWN:
            eventQueue.keyPress( (osgGA::GUIEventAdapter::KeySymbol) event.key.keysym.unicode);
            return true;

        case SDL_VIDEORESIZE:
            eventQueue.windowResize(0, 0, event.resize.w, event.resize.h );
            return true;

        default:
            break;
    }
    return false;
}




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
    UserInputsHandler()
		: _right_button_pressed(false)
	{}

	//! destructor
    ~UserInputsHandler(){}

	//! handle inputs
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	bool	_right_button_pressed;
	int		_mouse_Y;
};







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

			return true;
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
	_viewportX(800), _viewportY(600), _displayShadow(true), 
	_current_clip_layer(-1), m_screen(NULL), _gw(NULL)
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
void OsgHandler::Initialize(const std::string &WindowName, const std::string &DataPath, 
								boost::shared_ptr<EventHandler> evH, GuiHandler * GuiH)
{
	//set event handler
	_evH = evH;

	//set GUI
	_GuiH = GuiH;

	osgDB::setDataFilePathList(DataPath);


	LogHandler::getInstance()->LogToFile("Initializing graphics window...");



    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		LogHandler::getInstance()->LogToFile(std::string("Couldn't initialize SDL video: ") + SDL_GetError());
		return;
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );



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


	// set SDL screen
	LogHandler::getInstance()->LogToFile("Initializing screen display...");
    ResetScreen();
    if ( m_screen == NULL )
	{
		std::string err("Couldn't set the video mode: ");
		std::cout<<err<<SDL_GetError()<<std::endl;
		return;
    }


	// set window caption
	SDL_WM_SetCaption("LBANet",NULL);
	SDL_ShowCursor (SDL_DISABLE);
	SDL_EnableUNICODE (1);
	SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    
	// create the window to draw to.
	_viewer = new osgViewer::Viewer();
   _gw = _viewer->setUpViewerAsEmbeddedInWindow(0, 0, _resX, _resY);




 //   osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
 //   traits->x = 10;
 //   traits->y = 10;
 //   traits->width = _resX;
 //   traits->height = _resY;
 //   traits->windowDecoration = true;
 //   traits->doubleBuffer = true;
 //   traits->sharedContext = 0;
	//traits->windowName = WindowName;
 //   osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 //   osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
 //   if (!gw)
 //   {
	//	LogHandler::getInstance()->LogToFile("Error: unable to create graphics context.");
 //       return;
 //   }

    //_viewer->getCamera()->setGraphicsContext(gc.get());
    //_viewer->getCamera()->setViewport(0,0,_resX,_resY);

	_viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));



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
	_viewer->addEventHandler( new UserInputsHandler() );

	 // create the windows
	//_viewer->setUpViewOnSingleScreen(0);

    _viewer->realize();
	//_viewer->renderingTraversals();

	// reset camera
	ResetCameraProjectiomMatrix();

	//set default root node
	ResetDisplayTree();

	// put everything in the right place
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


	// free screen
	SDL_FreeSurface(m_screen);


    // Shutdown all subsystems
    SDL_Quit();
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
	//if(!_viewer)
	//	return;

	LogHandler::getInstance()->LogToFile("Resetting screen resolution...");

	//if(m_screen)
	//{
	//	SDL_FreeSurface(m_screen);
	//	m_screen = NULL;
	//}






	if (!_isFullscreen)
	{
		m_screen = SDL_SetVideoMode(_resX, _resY, 0, SDL_OPENGL);
		//const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	}
	else
	{
		m_screen = SDL_SetVideoMode(_resX, _resY, 0, SDL_OPENGL|SDL_FULLSCREEN);
		//const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	}

	if(_gw)
		_gw->resized(0, 0, _resX, _resY);


	if(_viewer)
	{
		_viewer->setSceneData(NULL);
		_viewer->releaseGLObjects();
		_viewer->setSceneData(_rootNode);
	}

	_viewportX = _resX;
	_viewportY = _resY;



	// iterate over all windows
	//osgViewer::Viewer::Windows windows;
 //   _viewer->getWindows(windows);
 //   for(osgViewer::Viewer::Windows::iterator itr = windows.begin();  itr != windows.end(); ++itr)
 //   {
	//	osgViewer::GraphicsWindow *window = *itr;

	//	osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();
	//	if (wsi == NULL) 
	//	{
	//		LogHandler::getInstance()->LogToFile("Error, no WindowSystemInterface available, cannot toggle window resolution change.");
	//		return;
	//	}

	//	unsigned int    screenWidth;
	//	unsigned int    screenHeight;
	//	wsi->getScreenResolution(*(window->getTraits()), screenWidth, screenHeight);

	//	if(_isFullscreen)
	//	{
	//		window->setWindowDecoration(false);
	//		window->setWindowRectangle(0, 0, screenWidth, screenHeight);
	//		_viewportX = screenWidth;
	//		_viewportY = screenHeight;
	//		
	//	}
	//	else
	//	{
	//		window->setWindowDecoration(true);
	//		window->setWindowRectangle((screenWidth - _resX) / 2, (screenHeight - _resY) / 2, _resX, _resY);
	//		_viewportX = _resX;
	//		_viewportY = _resY;
	//	}

	//	_viewer->getCamera()->setViewport(0,0,_viewportX,_viewportY);
	//	window->grabFocus();
	//	window->raiseWindow();
 //   }

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
set camera target
***********************************************************/
void OsgHandler::GetCameraTarget(double &TargetX, double &TargetY, double &TargetZ)
{
	TargetX = _targetx;
	TargetY = _targety;
	TargetZ = _targetz;

}

/***********************************************************
set light
***********************************************************/
void OsgHandler::SetLight(const LbaMainLightInfo &LightInfo)
{
	if(!_lightNode)
		return;

	if(LightInfo.UseLight)
	{
		osg::Light* myLight1 = new osg::Light();
		myLight1->setLightNum(0);

		osg::Vec4 lightpos;
		lightpos.set(LightInfo.LOnPosX, LightInfo.LOnPosY, LightInfo.LOnPosZ,0.0f);
		myLight1->setPosition(lightpos);
		myLight1->setAmbient(osg::Vec4(LightInfo.LOnAmbientR,LightInfo.LOnAmbientG,LightInfo.LOnAmbientB, 1.0));
		myLight1->setDiffuse(osg::Vec4(LightInfo.LOnDiffuseR,LightInfo.LOnDiffuseG,LightInfo.LOnDiffuseB, 1.0));
		_lightNode->setLight(myLight1);
		_lightNode->setLocalStateSetModes(osg::StateAttribute::ON);
		_lightNode->setStateSetModes(*_translNode->getOrCreateStateSet(),osg::StateAttribute::ON);
	}
	else
	{
		_lightNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
	}

}


/***********************************************************
clear all nodes of the display tree
typically called when changing map
***********************************************************/
void OsgHandler::ResetDisplayTree()
{
	if(!_translNode)
		return;

	if(_lightNode)
		_translNode->removeChild(_lightNode);

	_lightNode = new osg::LightSource();
	_translNode->addChild(_lightNode);


	// check if we use shadow or not
	if(_displayShadow)
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


	//set default light
	LbaMainLightInfo Linf;
	Linf.UseLight = false;
	SetLight(Linf);
}



/***********************************************************
update display - returns true if need to terminate
***********************************************************/
bool OsgHandler::Update()
{
    SDL_Event evt;
    while ( SDL_PollEvent(&evt) )
    {
		if(evt.type == SDL_QUIT)
			return true;

        // pass the SDL event into the viewers event queue
		if(!_evH->Handle(evt))
			convertEvent(evt, *(_gw->getEventQueue()));

    }


    // draw the new frame
    _viewer->frame();


	//draw the GUI
	_GuiH->Redraw();


    // Swap Buffers
    SDL_GL_SwapBuffers();

	return false;

   


	//if(!_viewer->done())
	//{
	//	_viewer->frame();
	//	return false;
	//}

	//return true;
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
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added Node to display engine");
	#endif

	node->setNodeMask(ReceivesShadowTraversalMask | CastsShadowTraversalMask);
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
	transform->addChild(node);

	if(_sceneRootNode)
		_sceneRootNode->addChild(transform);

	return transform;
}


/***********************************************************
remove actor from the graph
***********************************************************/
void OsgHandler::RemoveActorNode(osg::ref_ptr<osg::Node> node)
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Removed Node from display engine");
	#endif

	if(_sceneRootNode)
		_sceneRootNode->removeChild(node);
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
