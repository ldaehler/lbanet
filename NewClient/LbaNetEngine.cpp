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

//#ifdef _WIN32
//	#include "SDL.h"
//#else
//	#include <SDL/SDL.h>
//#endif
//
//#include <windows.h>    // Header File For Windows
//#include <GL/gl.h>      // Header File For The OpenGL32 Library
//#include <GL/glu.h>     // Header File For The GLu32 Library
//

#include <iostream>

#include "LogHandler.h"
#include "LbaNetEngine.h"
#include "ConfigurationManager.h"
#include "OSGHandler.h"

#include <osg/MatrixTransform>


#define TICK_INTERVAL    30 //16



/***********************************************************
	Constructor
***********************************************************/
LbaNetEngine::LbaNetEngine()
: m_screen(NULL)
{
	m_eventHandler = boost::shared_ptr<EventHandler> (new EventHandler(this));

	//init the values from file
	ConfigurationManager::GetInstance()->GetInt("Options.Video.ScreenResolutionX", m_screen_size_X);
	ConfigurationManager::GetInstance()->GetInt("Options.Video.ScreenResolutionY", m_screen_size_Y);
	ConfigurationManager::GetInstance()->GetBool("Options.Video.Fullscreen", m_fullscreen);

	Initialize();
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetEngine::~LbaNetEngine()
{

}



/***********************************************************
initialize the class
***********************************************************/
void LbaNetEngine::Initialize(void)
{
	LogHandler::getInstance()->LogToFile("Initializing SDL...");


	// init OSG
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data", m_eventHandler, &m_guiHandler);

	//init gui
	LogHandler::getInstance()->LogToFile("Initializing gui...");
	m_guiHandler.Initialize(m_screen_size_X, m_screen_size_Y, false, "v0.8", this);


	//clear display engine
	LbaMainLightInfo linfo;
	linfo.UseLight = true;
	linfo.UseShadow = true;
	linfo.StartOn = false;

	linfo.LOnPosX=50.f;
	linfo.LOnPosY=100.f;
	linfo.LOnPosZ=50.f;
	linfo.LOnAmbientR=0.6f;
	linfo.LOnAmbientG=0.6f;
	linfo.LOnAmbientB=0.6f;
	linfo.LOnDiffuseR=0.8f;
	linfo.LOnDiffuseG=0.8f;
	linfo.LOnDiffuseB=0.8f;

	linfo.LOffPosX=100.f;
	linfo.LOffPosY=50.f;
	linfo.LOffPosZ=100.f;
	linfo.LOffAmbientR=0.0f;
	linfo.LOffAmbientG=0.0f;
	linfo.LOffAmbientB=0.0f;
	linfo.LOffDiffuseR=0.5f;
	linfo.LOffDiffuseG=0.5f;
	linfo.LOffDiffuseB=0.5f;
	OsgHandler::getInstance()->ResetDisplayTree(linfo);
	OsgHandler::getInstance()->AddActorNode(OsgHandler::getInstance()->LoadOSGFile("Lba1/Maps/map0.osgb"));

	LogHandler::getInstance()->LogToFile("Displaying login window...");
}


/***********************************************************
entry point of the engine
***********************************************************/
void LbaNetEngine::run(void)
{
	bool quit = false;
	//SDL_Event even;

	try
	{
		// Loop until a quit event is found
		while(!OsgHandler::getInstance()->Update())
		{
			// process stuff between frame
			Process();

			// wait function used to keep framerate constant
			//SDL_Delay(TimeLeft());
		}

	}
	catch(std::exception & ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unhandled exception catched:") + ex.what());
	}
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception catched"));
	}

	LogHandler::getInstance()->LogToFile("Quitting the game.");
}


/***********************************************************
process function
***********************************************************/
bool LbaNetEngine::Process(void)
{
	m_guiHandler.process();


	return true;
}




/***********************************************************
calculate time left between frames
***********************************************************/
long LbaNetEngine::TimeLeft(void)
{
    static long next_time = 0;
    long now=0;

 //   now = SDL_GetTicks();
 //   if ( next_time <= now )
	//{
 //       next_time = now+TICK_INTERVAL;
 //       return(0);
 //   }
    return(next_time-now);
}



/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::ResetScreen()
{
	//if(m_screen)
	//{
	//	SDL_FreeSurface(m_screen);
	//	m_screen = NULL;
	//}


	//if (!m_fullscreen)
	//{
	//	m_screen = SDL_SetVideoMode(m_screen_size_X, m_screen_size_Y, 0, SDL_OPENGL);
	//	const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	//}
	//else
	//{
	//	m_screen = SDL_SetVideoMode(m_screen_size_X, m_screen_size_Y, 0, SDL_OPENGL|SDL_FULLSCREEN);
	//	const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	//}
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::ChangeScreenAndLinkedRessources()
{
	m_guiHandler.grabTextures();

	ResetScreen();
	m_guiHandler.restoreTextures();
	m_guiHandler.Resize(m_screen_size_X, m_screen_size_Y);
}

/***********************************************************
change the screen resolution
***********************************************************/
void LbaNetEngine::ChangeScreen(int screenX, int screenY, bool fullscreen)
{
	m_screen_size_X = screenX;
	m_screen_size_Y = screenY;
	m_fullscreen = fullscreen;
	ChangeScreenAndLinkedRessources();
}


/***********************************************************
change from windowed to fullscreen and vice versa
***********************************************************/
void LbaNetEngine::ChangeFullScreen()
{
	ChangeScreenAndLinkedRessources();

}








