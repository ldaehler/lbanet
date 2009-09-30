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

#include "GUI.h"
#include "GuiHandler.h"
#include "LoginGUI.h"
#include "LogHandler.h"
#include "ChooseWorldGUI.h"
#include "GameGUI.h"
#include "MenuGUI.h"
#include "MenuGUI.h"
#include "OptionsGUI.h"
#include "DataLoader.h"
#include "ConfigurationManager.h"

#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

#include <CEGUI.h>
#include <RendererModules/OpenGLGUIRenderer/openglrenderer.h>
#include <CEGUIDefaultResourceProvider.h>





/***********************************************************
	Constructor
***********************************************************/
GuiHandler::GuiHandler()
: _currentGUI(-1), _gui_renderer(NULL), _game_gui(NULL), _login_gui(NULL)
{

}


/***********************************************************
	Destructor
***********************************************************/
GuiHandler::~GuiHandler()
{
	if(_gui_renderer)
		delete _gui_renderer;

	std::vector<GUI *>::iterator it = _guis.begin();
	std::vector<GUI *>::iterator end = _guis.end();
	for(; it != end; ++it)
		delete *it;
}


/***********************************************************
initialize function
***********************************************************/
void GuiHandler::Initialize(int screen_size_X, int screen_size_Y, bool ServerOn,
							const std::string &clientversion)
{
	try
	{
		_gui_renderer = new CEGUI::OpenGLRenderer (0, screen_size_X, screen_size_Y);
		new CEGUI::System (_gui_renderer);

		// initialise the required dirs for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
			(CEGUI::System::getSingleton().getResourceProvider());

		rp->setResourceGroupDirectory("schemes", "./Data/GUI/schemes/");
		rp->setResourceGroupDirectory("imagesets", "./Data/GUI/imagesets/");
		rp->setResourceGroupDirectory("fonts", "./Data/GUI/fonts/");
		rp->setResourceGroupDirectory("layouts", "./Data/GUI/layouts/");
		rp->setResourceGroupDirectory("looknfeels", "./Data/GUI/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "./Data/GUI/lua_scripts/");

		// set the default resource groups to be used
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");


		// load in the scheme file, which auto-loads the TaharezLook imageset
		CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );

		if(! CEGUI::FontManager::getSingleton().isFontPresent( "abbey_m1-9" ) )
		  CEGUI::FontManager::getSingleton().createFont( "abbey_m1-9.font" );

		ReloadFontSize();

		CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );
		CEGUI::System::getSingleton().setDefaultTooltip( "TaharezLook/Tooltip" );

		// Load the Imageset that has the pictures for our button.
		CEGUI::ImagesetManager::getSingleton().createImageset( "LogoBig.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "lbaNetB.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "chatbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "TeleportButton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "HeadInterface.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "optionsbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "soundbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "changeworldbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "quitbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "MenuChar.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "tunic.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "quest.imageset" );
		CEGUI::ImagesetManager::getSingleton().createImageset( "weapon.imageset" );
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init gui: ") + ex.getMessage().c_str());
	}

	//initialize the login gui
	_login_gui = new LoginGUI();
	_login_gui->Initialize(clientversion);
	_login_gui->SetServrOn(ServerOn);
	_guis.push_back(_login_gui);

	ChooseWorldGUI * cwG = new ChooseWorldGUI();
	cwG->Initialize();
	_guis.push_back(cwG);

	std::vector<WorldDesc> list;
	DataLoader::getInstance()->GetAvailableWorlds(list);
	cwG->SetWorldList(list);

	_game_gui = new GameGUI();
	_game_gui->Initialize();
	_guis.push_back(_game_gui);

	MenuGUI * menuG = new MenuGUI();
	menuG->Initialize();
	_guis.push_back(menuG);

	OptionsGUI * optG = new OptionsGUI();
	optG->Initialize();
	_guis.push_back(optG);


	SwitchGUI(0);
}


/***********************************************************
inject time to the GUI
***********************************************************/
void GuiHandler::inject_time_pulse()
{
	/* get current "run-time" in seconds */
	double t = 0.001*SDL_GetTicks();
	/* inject the time that passed since the last call */
	CEGUI::System::getSingleton().injectTimePulse( float(t-m_last_time_pulse) );
	/* store the new time as the last time */
	m_last_time_pulse = t;
}


/***********************************************************
process function
***********************************************************/
void GuiHandler::process(void)
{
   inject_time_pulse();

	std::vector<GUI *>::iterator it = _guis.begin();
	std::vector<GUI *>::iterator end = _guis.end();
	for(; it != end; ++it)
		(*it)->Process();
}


/***********************************************************
switch from current gui to another one
***********************************************************/
void GuiHandler::SwitchGUI(int GuiNumber)
{
	if(GuiNumber == _currentGUI)
	   return;

	try
	{
		CEGUI::Window * root = _guis[GuiNumber]->GetRoot();
		if(root)
		{
			CEGUI::System::getSingleton().setGUISheet( root );
			_guis[GuiNumber]->Displayed();
		}
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception changing gui: ") + ex.getMessage().c_str());
	}

	_currentGUI = GuiNumber;
}


/***********************************************************
redraw the scene on screen
***********************************************************/
void GuiHandler::Redraw(void)
{
	CEGUI::System::getSingleton().renderGUI();
}



/***********************************************************
called when the windows is resized
***********************************************************/
void GuiHandler::Resize(int screen_size_X, int screen_size_Y)
{
	_gui_renderer->setDisplaySize(CEGUI::Size((float)screen_size_X, (float)screen_size_Y));
}

/***********************************************************
called when the windows is resized
***********************************************************/
void GuiHandler::grabTextures()
{
	_gui_renderer->grabTextures();
}

/***********************************************************
called when the windows is resized
***********************************************************/
void GuiHandler::restoreTextures()
{
	_gui_renderer->restoreTextures();

	CEGUI::Window * root = _guis[_currentGUI]->GetRoot();
	if(root)
	{
		root->hide();
		root->show();
	}
}


/***********************************************************
set the current map of the game
***********************************************************/
void GuiHandler::SetCurrentMap(const std::string & WorldName, const std::string & MapName)
{
	if(_game_gui)
		_game_gui->SetCurrentMap(WorldName, MapName);
}



/***********************************************************
focus the chatbox
***********************************************************/
void GuiHandler::FocusChatbox(bool focus)
{
	if(_game_gui)
		_game_gui->FocusChatbox(focus);
}


/***********************************************************
set irc thread
***********************************************************/
void GuiHandler::SetIrcThread(IrcThread * IT)
{
	if(_game_gui)
		_game_gui->SetIrcThread(IT);
}


/***********************************************************
called when font size changed
***********************************************************/
void GuiHandler::ReloadFontSize()
{
	// load in a font.  The first font loaded automatically becomes the default font.
	int fontsize;
	ConfigurationManager::GetInstance()->GetInt("Options.Gui.FontSize", fontsize);
	std::stringstream strs;
	strs<<"DejaVuSans-"<<fontsize;

	if(!CEGUI::FontManager::getSingleton().isFontPresent( strs.str() ) )
	  CEGUI::FontManager::getSingleton().createFont( strs.str() + ".font" );

	CEGUI::System::getSingleton().setDefaultFont( strs.str() );
}


/***********************************************************
set the list of teleport places
***********************************************************/
void GuiHandler::SetTeleportList(const std::map<std::string, TPInfo> &_lists)
{
	if(_game_gui)
		_game_gui->SetTeleportList(_lists);
}

/***********************************************************
display game text
***********************************************************/
bool GuiHandler::DisplayGameText(long textid, bool show)
{
	if(_game_gui)
		return _game_gui->DisplayGameText(textid, show);

	return true;
}


/***********************************************************
inform the user the login failed
***********************************************************/
void GuiHandler::InformNotLoggedIn(int problem, const std::string & reason)
{
	if(_login_gui)
		_login_gui->InformNotLoggedIn(problem, reason);
}



/***********************************************************
set actors
***********************************************************/
void GuiHandler::SetActors(std::map<long, Actor *> * Lactors, std::map<long, Actor *> * Eactors)
{
	if(_game_gui)_game_gui->SetActors(Lactors, Eactors);
}


/***********************************************************
set player name
***********************************************************/
void GuiHandler::SetPlayerName(const std::string & name)
{
	if(_game_gui)
		_game_gui->SetPlayerName(name);
}