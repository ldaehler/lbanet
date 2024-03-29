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
#include "LbaNetEngine.h"

#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

#include <CEGUI.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <CEGUIDefaultResourceProvider.h>





/***********************************************************
	Constructor
***********************************************************/
GuiHandler::GuiHandler()
: _currentGUI(-1), _gui_renderer(NULL), _game_gui(NULL), 
	_login_gui(NULL)
{

}


/***********************************************************
	Destructor
***********************************************************/
GuiHandler::~GuiHandler()
{
	std::vector<GUI *>::iterator it = _guis.begin();
	std::vector<GUI *>::iterator end = _guis.end();
	for(; it != end; ++it)
		delete *it;

    CEGUI::System::destroy();
    CEGUI::OpenGLRenderer::destroy(*_gui_renderer);
}


/***********************************************************
initialize function
***********************************************************/
void GuiHandler::Initialize(int screen_size_X, int screen_size_Y, bool ServerOn,
							const std::string &clientversion,
							LbaNetEngine * engine)
{
	_engine = engine;

	try
	{
		_gui_renderer =  &CEGUI::OpenGLRenderer::create();
		_gui_renderer->enableExtraStateSettings(true);

		//new CEGUI::OpenGLRenderer (0, screen_size_X, screen_size_Y);

		CEGUI::System::create( *_gui_renderer, 0, 0, 0, 0, "", LogHandler::getInstance()->GetGUIFilename().c_str() );
		//new CEGUI::System (_gui_renderer);

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
		CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );

		//! load font file
		CEGUI::FontManager::getSingleton().create( "abbey_m1-9.font" );


		ReloadFontSize();

		CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );
		CEGUI::System::getSingleton().setDefaultTooltip( "TaharezLook/Tooltip" );

		// Load the Imageset that has the pictures for our button.
		CEGUI::ImagesetManager::getSingleton().create( "LogoBig.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "LogoCenter.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "lbaNetB.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "chatbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "TeleportButton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "HeadInterface.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "optionsbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "soundbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "changeworldbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "quitbutton.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "MenuChar.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "tunic.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "quest.imageset" );
		CEGUI::ImagesetManager::getSingleton().create( "weapon.imageset" );

		CEGUI::ImagesetManager::getSingleton().create( "contourfont.imageset" );
		CEGUI::FontManager::getSingleton().create( "ContourFont.font" );

		// loading the smileys
		{
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_biggrin", "smileys/biggrin.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_bloated", "smileys/bloated.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_confused", "smileys/confused.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_cool", "smileys/cool.png");
			ims.setAutoScalingEnabled(false);
			}
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_dinofly", "smileys/dinofly.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_eek", "smileys/eek.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_embarrassment", "smileys/embarrassment.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_evil", "smileys/evil.png");
			ims.setAutoScalingEnabled(false);
			}
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_evilwink", "smileys/evilwink.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_frown", "smileys/frown.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_funfrock", "smileys/funfrock.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_hmpf", "smileys/hmpf.png");
			ims.setAutoScalingEnabled(false);
			}
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_hmz", "smileys/hmz.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_kiss", "smileys/kiss.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_lol", "smileys/lol.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_mad", "smileys/mad.png");
			ims.setAutoScalingEnabled(false);
			}
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_proud", "smileys/proud.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_rabibunny", "smileys/rabibunny.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_rolleyes", "smileys/rolleyes.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_sad", "smileys/sad.png");
			ims.setAutoScalingEnabled(false);
			}
			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_sigh", "smileys/sigh.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_smilie", "smileys/smilie.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_stupid", "smileys/stupid.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_tdown", "smileys/tdown.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_tup", "smileys/tup.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_wink", "smileys/wink.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_zoe", "smileys/zoe.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_zombie", "smileys/zombie.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("sm_tongue", "smileys/tongue.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("mailb_new", "new_im.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("mailb_notnew", "nonew_im.png");
			ims.setAutoScalingEnabled(false);
			}

			{
			CEGUI::Imageset &ims = CEGUI::ImagesetManager::getSingleton().createFromImageFile("mailb_trash", "trash.png");
			ims.setAutoScalingEnabled(false);
			}	
		}
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

	_option_gui = new OptionsGUI();
	_option_gui->Initialize();
	_guis.push_back(_option_gui);


	SwitchGUI(0);


    // clearing this queue actually makes sure it's created(!)
    //_gui_renderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    //_gui_renderer->getDefaultRenderingRoot().
    //    subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
    //        CEGUI::Event::Subscriber(&GuiHandler::overlayHandler, this));
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
    CEGUI::System::getSingleton().
        notifyDisplaySizeChanged(CEGUI::Size((float)screen_size_X,(float)screen_size_Y));

	_game_gui->Refresh();
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

	//if(!CEGUI::FontManager::getSingleton().isFontPresent( strs.str() ) )
	  CEGUI::FontManager::getSingleton().create( strs.str() + ".font" );

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
	if(_game_gui)
		_game_gui->SetActors(Lactors, Eactors);
}


/***********************************************************
set player name
***********************************************************/
void GuiHandler::SetPlayerName(const std::string & name)
{
	if(_game_gui)
		_game_gui->SetPlayerName(name);
}


/***********************************************************
handle overlay
***********************************************************/
bool GuiHandler::overlayHandler(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

	//_engine->DrawOverlay();

	return true;
}


/***********************************************************
display inventory
***********************************************************/
void GuiHandler::ShowInventory()
{
	if(_game_gui)
		_game_gui->ShowInventory();
}


/***********************************************************
display inventory
***********************************************************/
void GuiHandler::RefreshOption()
{
	if(_option_gui)
		_option_gui->SendNameColor();
}


/***********************************************************
show dialog with NPC
***********************************************************/
void GuiHandler::ShowDialog(long ActorId, const std::string &ActorName, DialogHandlerPtr Dialog, 
								bool Show, const std::map<long, TraderItem> &inventory)
{
	if(_game_gui)
		_game_gui->ShowDialog(ActorId, ActorName, Dialog, Show, inventory);
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void GuiHandler::ShowMails(const std::vector<LbaNet::PMInfo> &pms)
{
	if(_game_gui)
		_game_gui->ShowMails(pms);
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void GuiHandler::HideMails()
{
	if(_game_gui)
		_game_gui->HideMails();
}
