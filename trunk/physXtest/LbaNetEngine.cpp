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

#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

#define NOMINMAX
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <IL/ilut.h>

#include "SynchronizedTimeHandler.h"
#include "LbaNetEngine.h"
#include "PhysXEngine.h"
#include "PlayerHandler.h"
#include "NxVec3.h"

#include <iostream>

#define TICK_INTERVAL    30 //16



/***********************************************************
	Constructor
***********************************************************/
LbaNetEngine::LbaNetEngine()
: m_screen(NULL), m_screen_size_X(800), m_screen_size_Y(600), m_fullscreen(false),
	m_eventHandler(this)
{
	Initialize();

	unsigned int controllerid = PhysXEngine::getInstance()->CreateCharacter(NxVec3(20, 10, 20), 1, 3);
	m_player = new PlayerHandler(10.0f, controllerid);
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetEngine::~LbaNetEngine()
{
	PhysXEngine::getInstance()->Quit();

	// free screen
	SDL_FreeSurface(m_screen);

    // Shutdown all subsystems
    SDL_Quit();
}



/***********************************************************
initialize the class
***********************************************************/
void LbaNetEngine::Initialize(void)
{
    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		//LogHandler::getInstance()->LogToFile(std::string("Couldn't initialize SDL video: ") + SDL_GetError());
		return;
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );



	ilInit();

	// Initialize ILU
	iluInit();

	// Initialize ILUT with OpenGL support.
	ilutRenderer(ILUT_OPENGL);


   /*
     * Initialize the display in 8-bit palettized mode,
     * requesting a software surface
     */
    ResetScreen();
    if ( m_screen == NULL )
	{
		std::string err("Couldn't set the video mode: ");
		std::cout<<err<<SDL_GetError()<<std::endl;
		return;
    }

	// set window caption
	SDL_WM_SetCaption("LBANet",NULL);



	//SDL_ShowCursor (SDL_DISABLE);
	SDL_EnableUNICODE (1);
	SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);


	m_lbaNetModel.Initialize();


	PhysXEngine::getInstance()->Init();
}


/***********************************************************
entry point of the engine
***********************************************************/
void LbaNetEngine::run(void)
{
	bool quit = false;
	SDL_Event even;
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();

	try
	{
		// Loop until an SDL_QUIT event is found
		while( !quit )
		{
			double currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
			double timediff = ( currtime - m_lasttime );
			m_currframetime.Update( timediff ) ;
			m_lasttime = currtime;


			// get physics results
			PhysXEngine::getInstance()->GetPhysicsResults();


			while( SDL_PollEvent( &even ) )
				quit = m_eventHandler.Handle(even);

			m_player->Process(currtime, (float)timediff);



			// start physic calculation
			PhysXEngine::getInstance()->StartPhysics();

			// redraw the scene
			Redraw();

			// wait function used to keep framerate constant
			SDL_Delay(TimeLeft());
		}
	}
	catch(std::exception & ex)
	{
		//LogHandler::getInstance()->LogToFile(std::string("Unhandled exception catched:") + ex.what());
	}
	catch(...)
	{
		//LogHandler::getInstance()->LogToFile(std::string("Unknown exception catched"));
	}

	//LogHandler::getInstance()->LogToFile("Quitting the game.");
}





/***********************************************************
redraw the scene on screen
***********************************************************/
void LbaNetEngine::Redraw()
{
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	m_lbaNetModel.Draw();


	SDL_GL_SwapBuffers();
}


/***********************************************************
calculate time left between frames
***********************************************************/
long LbaNetEngine::TimeLeft(void)
{
    static long next_time = 0;
    long now;

    now = SDL_GetTicks();
    if ( next_time <= now )
	{
        next_time = now+TICK_INTERVAL;
        return(0);
    }
    return(next_time-now);
}



/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::ResetScreen()
{
	if(m_screen)
	{
		SDL_FreeSurface(m_screen);
		m_screen = NULL;
	}

	if (!m_fullscreen)
	{
		m_screen = SDL_SetVideoMode(m_screen_size_X, m_screen_size_Y, 0, SDL_OPENGL);
		const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	}
	else
	{
		m_screen = SDL_SetVideoMode(m_screen_size_X, m_screen_size_Y, 0, SDL_OPENGL|SDL_FULLSCREEN);
		const SDL_VideoInfo *vidI = SDL_GetVideoInfo();
	}

	m_lbaNetModel.SetScreenSize(m_screen_size_X, m_screen_size_Y);
}



/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::DeltaUpdateZoom(bool ZoomIn)
{
	m_lbaNetModel.DeltaUpdateZoom(ZoomIn);
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::DeltaUpdateZenit(double zenit)
{
	m_lbaNetModel.DeltaUpdateZenit(zenit);
}


		
/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::PlayerStartMove(int moveDirection)
{
	m_player->PlayerStartMove(moveDirection);
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::PlayerStopMove(int moveDirection)
{
	m_player->PlayerStopMove(moveDirection);
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::PlayerDoAction(bool ForcedNormalAction)
{

}
