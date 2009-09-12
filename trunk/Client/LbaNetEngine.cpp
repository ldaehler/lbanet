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

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <IL/ilut.h>

#include "ServerConnectionHandler.h"
#include "SynchronizedTimeHandler.h"
#include "TextWritter.h"
#include "MusicHandler.h"
#include "LogHandler.h"
#include "LbaNetEngine.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "ConfigurationManager.h"
#include "DataLoader.h"

#define TICK_INTERVAL    30 //16



/***********************************************************
	Constructor
***********************************************************/
LbaNetEngine::LbaNetEngine(ServerConnectionHandler * serverH, const std::string & clientV)
: m_serverConnectionHandler(serverH), m_screen(NULL),
	m_currframetime(unsigned long(100)), m_eventHandler(this),
	m_currentstate(EGaming), m_oldstate(ELogin), m_lbaNetModel(&m_guiHandler),
	m_clientV(clientV)
{
	//init the values from file
	ConfigurationManager::GetInstance()->GetInt("Options.Video.ScreenResolutionX", m_screen_size_X);
	ConfigurationManager::GetInstance()->GetInt("Options.Video.ScreenResolutionY", m_screen_size_Y);
	ConfigurationManager::GetInstance()->GetBool("Options.Video.Fullscreen", m_fullscreen);
	ConfigurationManager::GetInstance()->GetBool("Options.Video.DisplayFPS", m_DisplayFPS);

	std::string lang = "en";
	ConfigurationManager::GetInstance()->GetString("Options.General.Language", lang);
	DataLoader::getInstance()->SetLanguage(lang);
	Initialize();
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetEngine::~LbaNetEngine()
{
	// free screen
	SDL_FreeSurface(m_screen);

	// exit music
	MusicHandler::getInstance()->Unitialize();

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
		LogHandler::getInstance()->LogToFile(std::string("Couldn't initialize SDL video: ") + SDL_GetError());
		return;
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


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


	// initialize the music
	MusicHandler::getInstance()->Initialize();


	// Initialize IL
	ilInit();

	// Initialize ILU
	iluInit();

	// Initialize ILUT with OpenGL support.
	ilutRenderer(ILUT_OPENGL);

	SDL_ShowCursor (SDL_DISABLE);
	SDL_EnableUNICODE (1);
	SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	m_guiHandler.Initialize(m_screen_size_X, m_screen_size_Y, m_serverConnectionHandler->ServerOn(), m_clientV);
	m_lbaNetModel.Initialize();
	m_lbaNetModel.SetScreenSize(m_screen_size_X, m_screen_size_Y);

	m_guiHandler.SetActors(m_lbaNetModel.GetLActors(), m_lbaNetModel.GetEActors());

	PlayMenuMusic();
	SwitchGuiToLogin();


}


/***********************************************************
entry point of the engine
***********************************************************/
void LbaNetEngine::run(void)
{
	bool quit = false;
	SDL_Event even;
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();

    // Loop until an SDL_QUIT event is found
    while( !quit )
	{
		double currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
		m_currframetime.Update( currtime - m_lasttime ) ;
		m_lasttime = currtime;


		while( SDL_PollEvent( &even ) )
			quit = m_eventHandler.Handle(even);


		// proceed function doing everything that need to be done for each new tick
		Process();

		// redraw the scene
		Redraw();

		// wait function used to keep framerate constant
		SDL_Delay(TimeLeft());
    }
}


/***********************************************************
process function
***********************************************************/
bool LbaNetEngine::Process(void)
{
	//let the gui process
	m_guiHandler.process();

	//check for game events
	HandleGameEvents();

	// process model
	m_lbaNetModel.Process();

	return true;
}


/***********************************************************
redraw the scene on screen
***********************************************************/
void LbaNetEngine::Redraw()
{
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(m_currentstate == EGaming)
	{
		m_lbaNetModel.Draw();


		if(m_DisplayFPS)
		{
			std::stringstream strs;
			strs<<"FPS: "<<1000.0/m_currframetime.Value();
			TextWritter::getInstance()->glPrintTextOnScreen(strs.str(), 0,
										m_screen_size_X, m_screen_size_Y, 10, 10);
		}

		m_guiHandler.Redraw();
	}
	else
	{
		if(m_currentstate == ELogin)
		{
			m_guiHandler.Redraw();
			m_lbaNetModel.Draw();
		}
		else
		{
			m_lbaNetModel.Draw();
			m_guiHandler.Redraw();
		}
	}



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

 //   double currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	//double diff = (currtime - m_lasttime);
	//std::cout<<diff<<std::endl;

	//if(diff > TICK_INTERVAL)
	//	return 0;
	//else
	//	return (TICK_INTERVAL - (long)diff);
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
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetEngine::ChangeScreenAndLinkedRessources()
{
	m_guiHandler.grabTextures();
	ResetScreen();
	m_guiHandler.Resize(m_screen_size_X, m_screen_size_Y);
	m_guiHandler.restoreTextures();
	m_lbaNetModel.SetScreenSize(m_screen_size_X, m_screen_size_Y);
	TextWritter::getInstance()->ReloadTexture();
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
	if(m_currentstate != EOption)
	{
		m_fullscreen = !m_fullscreen;
		ConfigurationManager::GetInstance()->SetBool("Options.Video.Fullscreen", m_fullscreen);
		ChangeScreenAndLinkedRessources();
	}
}


/***********************************************************
called to check for game events and handle them
***********************************************************/
void LbaNetEngine::HandleGameEvents()
{
	std::vector<GameEvent *> events;
	ThreadSafeWorkpile::getInstance()->GetPendingEvents(events);
	std::vector<GameEvent *>::iterator it = events.begin();
	std::vector<GameEvent *>::iterator end = events.end();
	for(;it != end; ++it)
	{
		switch((*it)->GetType())
		{
			case 1: // login event
				if(m_currentstate == ELogin)
				{
					LoginEvent * ev = static_cast<LoginEvent *> (*it);
					TryLogin(ev->_Name, ev->_Password, ev->_Local);
				}
			break;

			case 2: // gui exit event
				ExitGui();
			break;

			case 3: // change world event
				{
					ChangeWorldEvent * evcw = static_cast<ChangeWorldEvent *> (*it);
					ChangeWorld(evcw->_NewWorld);
				}
			break;

			case 4: // display gui event
				{
					DisplayGUIEvent * evdg = static_cast<DisplayGUIEvent *> (*it);
					DisplayGUI(evdg->_GuiNumber);
				}
			break;

			case 5: // change screen res event
				{
					ScreenResEvent * evcs = static_cast<ScreenResEvent *> (*it);
					ChangeScreen(evcs->_screenX, evcs->_screenY, evcs->_fullscreen);
				}
			break;

			case 6: // display fps event
				{
					DisplayFPSEvent * evcs = static_cast<DisplayFPSEvent *> (*it);
					m_DisplayFPS = evcs->_display;
				}
			break;

			case 7: // change body event
				{
					ChangeMainBodyEvent * evcbe = static_cast<ChangeMainBodyEvent *> (*it);
					if(evcbe->_plus)
						m_lbaNetModel.IncreasePlayerBody();
					else
						m_lbaNetModel.DecreasePlayerBody();
				}
			break;

			case 8: // new font size event
				{
					m_guiHandler.ReloadFontSize();
				}
			break;

			case 9: // teleport event
				{
					TeleportEvent * evcbe = static_cast<TeleportEvent *> (*it);
					m_lbaNetModel.ChangeMap(evcbe->_NewMap, evcbe->_Spawning);
				}
			break;

			case 10: // teleport event
				{
					ChangeMainBodyColorEvent * evcbe = static_cast<ChangeMainBodyColorEvent *> (*it);
					if(evcbe->_plus)
						m_lbaNetModel.IncreasePlayerBodyColor();
					else
						m_lbaNetModel.DecreasePlayerBodyColor();
				}
			break;

			case 11: // display game event
				{
					if(m_currentstate == EGaming)
					{
						DisplayGameTextEvent * evcbe = static_cast<DisplayGameTextEvent *> (*it);
						m_guiHandler.DisplayGameText(evcbe->_textid);
					}
				}
			break;

			case 12: // player scripted event
				{
					if(m_currentstate == EGaming)
					{
						MainPlayerScriptedEvent * evcbe = static_cast<MainPlayerScriptedEvent *> (*it);
						m_lbaNetModel.DoPlayerScriptedEvent(evcbe->_script);
					}
				}
			break;

			case 13: // game signal event
				{
					if(m_currentstate == EGaming)
					{
						GameSignalvent * evcbe = static_cast<GameSignalvent *> (*it);
						m_lbaNetModel.SignalEvent(evcbe->_signal, evcbe->_targets);
					}
				}
			break;

			case 14: // teleport anywere event
				{
					if(m_currentstate == EGaming)
					{
						TeleportAnyEvent * evcbe = static_cast<TeleportAnyEvent *> (*it);
						m_lbaNetModel.ChangeMap(evcbe->_NewMap, evcbe->_X, evcbe->_Y, evcbe->_Z);
					}
				}
			break;

			case 15: // change perspective event
				{
					ChangePerspectiveEvent * evcs = static_cast<ChangePerspectiveEvent *> (*it);
					m_lbaNetModel.ChangePespective(evcs->_perspective);
				}
			break;

			case 16: // display exits event
				{
					DisplayExitsEvent * evcs = static_cast<DisplayExitsEvent *> (*it);
					m_lbaNetModel.DisplayExits(evcs->_display);
				}
			break;
		}

		delete *it;
	}
}


/***********************************************************
switch gui helpers
***********************************************************/
void LbaNetEngine::SwitchGuiToLogin()
{
	if(m_currentstate == ELogin)
		return;

	m_lbaNetModel.CleanupWorld();
	m_lbaNetModel.ZoomInPlayerForLogin();
	PlayMenuMusic();
	m_serverConnectionHandler->Disconnect();
	m_guiHandler.SwitchGUI(0);
	m_oldstate = m_currentstate;
	m_currentstate = ELogin;
}

/***********************************************************
switch gui helpers
***********************************************************/
void LbaNetEngine::SwitchGuiToChooseWorld()
{
	if(m_currentstate == EChoosingWorld)
		return;

	m_lbaNetModel.Pause();
	m_lbaNetModel.ResetZoom();
	PlayMenuMusic();
	m_guiHandler.SwitchGUI(1);
	m_oldstate = m_currentstate;
	m_currentstate = EChoosingWorld;
}

/***********************************************************
switch gui helpers
***********************************************************/
void LbaNetEngine::SwitchGuiToGame()
{
	if(m_currentstate == EGaming)
		return;

	m_lbaNetModel.Resume(false);
	m_guiHandler.SwitchGUI(2);
	m_oldstate = m_currentstate;
	m_currentstate = EGaming;
}

/***********************************************************
switch gui helpers
***********************************************************/
void LbaNetEngine::SwitchGuiToMenu()
{
	if(m_currentstate == EMenu)
	{
		SwitchGuiToGame();
		return;
	}

	if(m_currentstate == EGaming)
	{
		m_lbaNetModel.Pause();
		m_guiHandler.SwitchGUI(3);
		m_oldstate = m_currentstate;
		m_currentstate = EMenu;
	}
}

/***********************************************************
switch gui helpers
***********************************************************/
void LbaNetEngine::SwitchGuiToOption()
{
	if(m_currentstate == EOption)
		return;

	m_lbaNetModel.Pause();
	m_guiHandler.SwitchGUI(4);
	m_oldstate = m_currentstate;
	m_currentstate = EOption;
}



/***********************************************************
try to login to the server
***********************************************************/
void LbaNetEngine::TryLogin(const std::string &Name, const std::string &Password, bool Local)
{
	if(!Local)
	{
		bool ircon = false;
		std::string reason;
		int res = m_serverConnectionHandler->Connect(Name, Password, ircon, reason);
		if(res < 1)
		{
			ThreadSafeWorkpile::getInstance()->SetServeron(false);
			m_guiHandler.InformNotLoggedIn(res, reason);
			return;
		}
		ThreadSafeWorkpile::getInstance()->SetServeron(true);

		if(ircon)
			m_guiHandler.SetIrcThread(m_serverConnectionHandler->GetIrcThread());
	}

	m_lbaNetModel.SetPlayerName(Name);
	SwitchGuiToChooseWorld();
}


/***********************************************************
exit current active gui
***********************************************************/
void LbaNetEngine::ExitGui()
{
	switch(m_oldstate)
	{
		case ELogin:
			SwitchGuiToLogin();
		break;
		case EChoosingWorld:
			SwitchGuiToChooseWorld();
		break;
		case EGaming: case EMenu: case EOption:
			SwitchGuiToGame();
		break;
	}
}


/***********************************************************
change the world
***********************************************************/
void LbaNetEngine::ChangeWorld(const std::string & NewWorld)
{
	m_lbaNetModel.ChangeWorld(NewWorld);
	SwitchGuiToGame();
}


/***********************************************************
display a specific gui
***********************************************************/
void LbaNetEngine::DisplayGUI(int guinumber)
{
	switch(guinumber)
	{
		case 0:
			SwitchGuiToLogin();
		break;
		case 1:
			SwitchGuiToChooseWorld();
		break;
		case 2:
			SwitchGuiToGame();
		break;
		case 3:
			SwitchGuiToMenu();
		break;
		case 4:
			SwitchGuiToOption();
		break;
	}
}


/***********************************************************
update camera functions
***********************************************************/
void LbaNetEngine::DeltaUpdateZoom(bool ZoomIn)
{
	if(m_currentstate == EGaming)
		m_lbaNetModel.DeltaUpdateZoom(ZoomIn);
}


/***********************************************************
update camera functions
***********************************************************/
void LbaNetEngine::DeltaUpdateZenit(double zenit)
{
	if(m_currentstate == EGaming)
		m_lbaNetModel.DeltaUpdateZenit(zenit);
}

/***********************************************************
called to play the assigned music when menu
***********************************************************/
void LbaNetEngine::PlayMenuMusic()
{
	MusicHandler::getInstance()->PlayMusic("Data/Music/LBA1-Track9.mp3", -1);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerStartMove(int moveDirection)
{
	if(m_currentstate == EGaming || m_currentstate == ELogin)
		m_lbaNetModel.PlayerStartMove(moveDirection);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerStopMove(int moveDirection)
{
	m_lbaNetModel.PlayerStopMove(moveDirection);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerStartMoveUpDown(bool Up)
{
	if(m_currentstate == EGaming)
		m_lbaNetModel.PlayerStartMoveUpDown(Up);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerStopMoveUpDown(bool Up)
{
	m_lbaNetModel.PlayerStopMoveUpDown(Up);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerChangeStance(int NewStance)
{
	if(m_currentstate == EGaming || m_currentstate == ELogin)
		m_lbaNetModel.PlayerChangeStance(NewStance);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetEngine::PlayerDoAction(bool ForcedNormalAction)
{
	if(m_currentstate == EGaming)
	{
		if(!m_guiHandler.DisplayGameText(0, false))
			m_lbaNetModel.PlayerDoAction(ForcedNormalAction);
	}
}


/***********************************************************
replace player in case it is stuck
***********************************************************/
void LbaNetEngine::ReplaceMain()
{
	if(m_currentstate == EGaming)
		m_lbaNetModel.ReplaceMain();
}

/***********************************************************
focus the chatbox
***********************************************************/
void LbaNetEngine::FocusChatbox(bool focus)
{
	if(m_currentstate == EGaming)
		m_guiHandler.FocusChatbox(focus);
}


/***********************************************************
debug function
***********************************************************/
void LbaNetEngine::GoNextRoom()
{
	m_lbaNetModel.GoNextRoom();
}