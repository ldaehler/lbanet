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

#include "LbaNetEngine.h"
#include "OSGHandler.h"
#include "LogHandler.h"
#include "PhysXEngine.h"
#include "EventHandler.h"
#include "SynchronizedTimeHandler.h"
#include "chatclient.h"
#include "GameEvents.h"
#include "InternalWorkpile.h"
#include "MusicHandler.h"
#include "ConfigurationManager.h"
#include "MessageBox.h"
#include "gameclient.h"

#define	_CUR_LBANET_VERSION_ "v0.8"
#define	_CUR_LBANET_SERVER_VERSION_ "v0.8"


/***********************************************************
	Constructor
***********************************************************/
LbaNetEngine::LbaNetEngine(ChatClient* Chatcl, GameClient* Gamecl)
: m_Chatcl(Chatcl), m_Gamecl(Gamecl), 
	m_currentstate(EGaming), m_oldstate(ELogin)
{
	Initialize();
	SwitchGuiToLogin();
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetEngine::~LbaNetEngine()
{	
	LogHandler::getInstance()->LogToFile("Finalizing Game...");


	// finalize music handler
	LogHandler::getInstance()->LogToFile("Finalizing sound engine...");
	MusicHandler::getInstance()->Initialize();

	// finalize OSG
	LogHandler::getInstance()->LogToFile("Finalizing Display engine...");
	OsgHandler::getInstance()->Finalize();

	// delete physic engine
	LogHandler::getInstance()->LogToFile("Finalizing physic engine...");
}



/***********************************************************
initialize the class
***********************************************************/
void LbaNetEngine::Initialize(void)
{
	LogHandler::getInstance()->LogToFile("Initializing Game...");

	// init music handler
	LogHandler::getInstance()->LogToFile("Initializing sound engine...");
	MusicHandler::getInstance()->Initialize();

	// init event handler
	m_eventHandler = boost::shared_ptr<EventHandler>(new EventHandler(this));

	// init OSG
	LogHandler::getInstance()->LogToFile("Initializing display engine...");
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data", m_eventHandler, &m_gui_handler);


	//init physic engine
	LogHandler::getInstance()->LogToFile("Initialize physic engine...");
	m_physic_engine = boost::shared_ptr<PhysXEngine>(new PhysXEngine());


	// init GUI
	m_gui_handler.Initialize(_CUR_LBANET_VERSION_, this);


	// set engine to model
	m_lbaNetModel.SetPhysicEngine(m_physic_engine);


	LogHandler::getInstance()->LogToFile("Initializing Completed.");
}


/***********************************************************
entry point of the engine
***********************************************************/
void LbaNetEngine::run(void)
{
	try
	{
		// init time
		unsigned int lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		unsigned int waittime = lasttime;

		// Loop until a quit event is found
		while(!OsgHandler::getInstance()->Update() && !InternalWorkpile::getInstance()->GameQuitted())
		{
			//get physic results
			m_physic_engine->GetPhysicsResults();

			//process chat client part
			{
				// processes incoming packets
				// all callbacks are generated from within the processInput calls
				m_Chatcl->ZCom_processInput( eZCom_NoBlock );

				//process internal stuff
				m_Chatcl->Process();

				// outstanding data will be packed up and sent from here
				m_Chatcl->ZCom_processOutput();
			}

			//process game client part
			{
				// update replicators and tell zoidcom how much ingame time has passed since the last
				// time this was called
				unsigned int currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
				unsigned int diff = (currtime-lasttime);
				lasttime = currtime;
				m_Gamecl->ZCom_processReplicators(diff);

				// processes incoming packets
				// all callbacks are generated from within the processInput calls
				m_Gamecl->ZCom_processInput( eZCom_NoBlock );

				// process stuff between frame
				Process();

				//! apply historic modifications before simulating current time
				m_physic_engine->ApplyHistoricModifications();

				//process internal stuff
				m_Gamecl->Process();

				// outstanding data will be packed up and sent from here
				m_Gamecl->ZCom_processOutput();
			}

			//update the elements to draw on screen
			m_lbaNetModel.UpdateDrawing();

			// pause the program for a few milliseconds
			unsigned int currwtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
			unsigned int wdiff = (currwtime-waittime);
			if(wdiff < SIMULATION_TIME_PER_UPDATE)
				ZoidCom::Sleep(SIMULATION_TIME_PER_UPDATE-wdiff);

			//start physic calculation
			m_physic_engine->StartPhysics();

			// mesure the time used to do one cycle
			waittime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
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
void LbaNetEngine::Process(void)
{
	//handle game events
	HandleGameEvents();

	// process model (update display stuff)
	m_lbaNetModel.Process();

	// process GUI
	m_gui_handler.Process();

	//process sound
	MusicHandler::getInstance()->Update();
}



/***********************************************************
start a move from keyboard input
***********************************************************/
void LbaNetEngine::StartMove(int MoveType)
{
	m_lbaNetModel.StartMove(MoveType);
}


/***********************************************************
stop a move from keyboard input
***********************************************************/
void LbaNetEngine::StopMove(int MoveType)
{
	m_lbaNetModel.StopMove(MoveType);
}

/***********************************************************
do action from keyboard input
***********************************************************/
void LbaNetEngine::DoAction()
{
	m_lbaNetModel.DoAction();
}



/***********************************************************
called to check for game events and handle them
***********************************************************/
void LbaNetEngine::HandleGameEvents()
{
	std::vector<GameEvent *> events;
	InternalWorkpile::getInstance()->GetPendingEvents(events);
	std::vector<GameEvent *>::iterator it = events.begin();
	std::vector<GameEvent *>::iterator end = events.end();
	for(;it != end; ++it)
	{
		switch((*it)->GetType())
		{
			case 1: // login event
				if(m_currentstate == ELogin)
				{
					//SaveCharToFile();
					LoginEvent * ev = static_cast<LoginEvent *> (*it);
					TryLogin(ev->_Name, ev->_Password);
					m_gui_handler.SetPlayerName(ev->_Name);
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

			case 5: // GameServerAddressEvent
				{
					GameServerAddressEvent * evdg = static_cast<GameServerAddressEvent *> (*it);
					ConnectToGameServer(evdg->_ServerName, evdg->_ServerAddress);
				}
			break;

			case 6: // GameServerUnreachableEvent
				{
					GameErrorMessageEvent * evdg = static_cast<GameErrorMessageEvent *> (*it);
					GameErrorMessage(evdg->_Message);
				}
			break;

			case 7: // NewMapEvent
				{
					NewMapEvent * evdg = static_cast<NewMapEvent *> (*it);
					m_lbaNetModel.NewMapEvent(evdg->_MapName, evdg->_MapType);
					m_gui_handler.SetCurrentMap(evdg->_MapName);
				}
			break;



			case 8: // new font size event
				{
					m_gui_handler.ReloadFontSize();
				}
			break;

			//case 9: // teleport event
			//	{
			//		TeleportEvent * evcbe = static_cast<TeleportEvent *> (*it);
			//		m_lbaNetModel.ChangeMap(evcbe->_NewMap, evcbe->_Spawning);
			//	}
			//break;

			//case 10: // teleport event
			//	{
			//		ChangeMainBodyColorEvent * evcbe = static_cast<ChangeMainBodyColorEvent *> (*it);
			//		if(evcbe->_plus)
			//			m_lbaNetModel.IncreasePlayerBodyColor();
			//		else
			//			m_lbaNetModel.DecreasePlayerBodyColor();
			//	}
			//break;

			case 11: // display text event
				{
					if(m_currentstate == EGaming)
					{
						DisplayGameTextEvent * evcbe = static_cast<DisplayGameTextEvent *> (*it);
						m_gui_handler.DisplayGameText(evcbe->_textid);
					}
				}
			break;

			//case 12: // player scripted event
			//	{
			//		if(m_currentstate == EGaming)
			//		{
			//			MainPlayerScriptedEvent * evcbe = static_cast<MainPlayerScriptedEvent *> (*it);
			//			m_lbaNetModel.DoPlayerScriptedEvent(evcbe->_script);
			//		}
			//	}
			//break;

			//case 13: // game signal event
			//	{
			//		if(m_currentstate == EGaming)
			//		{
			//			GameSignalvent * evcbe = static_cast<GameSignalvent *> (*it);
			//			m_lbaNetModel.SignalEvent(evcbe->_signal, evcbe->_targets);
			//		}
			//	}
			//break;

			//case 14: // teleport anywere event
			//	{
			//		if(m_currentstate == EGaming)
			//		{
			//			TeleportAnyEvent * evcbe = static_cast<TeleportAnyEvent *> (*it);
			//			m_lbaNetModel.ChangeMap(evcbe->_NewMap, evcbe->_X, evcbe->_Y, evcbe->_Z, 0);
			//		}
			//	}
			//break;

			//case 15: // change perspective event
			//	{
			//		ChangePerspectiveEvent * evcs = static_cast<ChangePerspectiveEvent *> (*it);
			//		m_lbaNetModel.ChangePespective(evcs->_perspective);
			//	}
			//break;

			//case 16: // display exits event
			//	{
			//		DisplayExitsEvent * evcs = static_cast<DisplayExitsEvent *> (*it);
			//		m_lbaNetModel.DisplayExits(evcs->_display);
			//	}
			//break;

			//case 17: // playerhurt
			//	{
			//		PlayerHurtEvent * evcs = static_cast<PlayerHurtEvent *> (*it);
			//		m_lbaNetModel.PlayerHurt(evcs->_fromactorid);
			//	}
			//break;

			//case 18: // do full check
			//	{
			//		m_lbaNetModel.DoFullCheckEvent();
			//	}
			//break;

			//case 19: // player life changed
			//	{
			//		PlayerLifeChangedEvent * evcs = static_cast<PlayerLifeChangedEvent *> (*it);
			//		m_lbaNetModel.PlayerLifeChanged(evcs->_CurLife, evcs->_MaxLife, 
			//											evcs->_CurMana, evcs->_MaxMana);

			//		_MaxLife = evcs->_MaxLife;
			//		_MaxMana = evcs->_MaxMana;
			//		_CurrentLife = evcs->_CurLife;
			//		if(_CurrentLife < 0)
			//			_CurrentLife = 0;
			//		_CurrentMana = evcs->_CurMana;
			//		if(_CurrentMana < 0)
			//			_CurrentMana = 0;
			//	}
			//break;

			//case 20: // player name color changed
			//	{
			//		PlayerNameColorChangedEvent * evcs = 
			//			static_cast<PlayerNameColorChangedEvent *> (*it);
			//		m_lbaNetModel.SetPlayerNameColor(evcs->_R, evcs->_G, evcs->_B);
			//	}
			//break;

			//case 21: // change stance event
			//	{
			//		ChangeStanceEvent * evcs = 
			//			static_cast<ChangeStanceEvent *> (*it);
			//		PlayerChangeStance(evcs->_stance);
			//	}
			//break;

			//case 22: // inventory used event
			//	{
			//		InventoryObjectUsedEvent * evcs = 
			//			static_cast<InventoryObjectUsedEvent *> (*it);
			//		m_lbaNetModel.InventoryUsed(evcs->_ObjectId, _CurrentLife==_MaxLife, _CurrentMana==_MaxMana);
			//	}
			//break;

			case 23: // focus chatbox event
				FocusChatbox(true);
			break;

			//case 24: // display NPC dialog
			//	{
			//		DisplayDialogEvent * evcs = 
			//			static_cast<DisplayDialogEvent *> (*it);
			//		m_guiHandler.ShowDialog(evcs->_ActorId, evcs->_ActorName, evcs->_Dialog,
			//									evcs->_Show, evcs->_inventory);
			//	}
			//break;


			//case 25: // object update event
			//	{
			//		ObjectUpdateEvent * evcs = 
			//			static_cast<ObjectUpdateEvent *> (*it);

			//		if(!evcs->_Received)
			//		{
			//			std::stringstream strs;
			//			strs<<"You used "<<evcs->_Number<<" [colour='FFFFFFFF'][image='set:"<<ImageSetHandler::GetInstance()->GetInventoryMiniImage(evcs->_ObjectId)<<"   image:full_image']"; 
			//			ThreadSafeWorkpile::ChatTextData cdata;
			//			cdata.Channel = "All";
			//			cdata.Sender = "info";
			//			cdata.Text = strs.str();
			//			ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
			//		}
			//		else
			//		{
			//			std::stringstream strs;
			//			strs<<"You received "<<evcs->_Number<<" [colour='FFFFFFFF'][image='set:"<<ImageSetHandler::GetInstance()->GetInventoryMiniImage(evcs->_ObjectId)<<"   image:full_image']"; 
			//			ThreadSafeWorkpile::ChatTextData cdata;
			//			cdata.Channel = "All";
			//			cdata.Sender = "info";
			//			cdata.Text = strs.str();
			//			ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
			//		}
			//	}
			//break;
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

	// disconnect from servers
	m_Chatcl->CloseConnection();
	m_Gamecl->CloseConnection();

	// clean up the world on disconnect
	m_lbaNetModel.CleanupWorld();

	PlayMenuMusic();

	m_gui_handler.SwitchGUI(0);
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
	//m_lbaNetModel.ResetZoom();

	PlayMenuMusic();
	m_gui_handler.SwitchGUI(1);
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

	if(m_lastmusic != "")
		MusicHandler::getInstance()->PlayMusic(m_lastmusic, 0);

	m_lbaNetModel.Resume(false);
	m_gui_handler.SwitchGUI(2);
	m_gui_handler.RefreshOption();

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
		m_gui_handler.SwitchGUI(3);
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
	m_gui_handler.SwitchGUI(4);
	m_oldstate = m_currentstate;
	m_currentstate = EOption;
}



/***********************************************************
try to login to the server
***********************************************************/
void LbaNetEngine::TryLogin(const std::string &Name, const std::string &Password)
{
	m_userlogin = Name;
	m_userpass = Password;
	std::string ip;
	ConfigurationManager::GetInstance()->GetString("Network.IpAddress", ip);
	m_Chatcl->ConnectToServer(ip, Name, Password, _CUR_LBANET_SERVER_VERSION_, this);
}


/***********************************************************
connection callback
***********************************************************/
void LbaNetEngine::ConnectionCallback(int SuccessFlag, const std::string & reason)
{
	// if connection not successful
	if(SuccessFlag < 1)
	{
		m_gui_handler.InformNotLoggedIn(SuccessFlag, reason);
		SwitchGuiToLogin();

		return;
	}

	//else
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
	//disconnect from previous world
	m_Gamecl->CloseConnection();

	//reset friend list
	m_Chatcl->GetFriendList();

	//subscribe to world channel
	m_Chatcl->SubscribeWorld();
	m_gui_handler.RefreshChannels();

	//clean up old world
	m_lbaNetModel.CleanupWorld();

	//set chat world name
	m_gui_handler.SetCurrentWorld(NewWorld);

	//ask server to switch world
	m_Chatcl->GetGameServerAddress(NewWorld);

	//change gui to game gui
	SwitchGuiToGame();
}

/***********************************************************
display a specific gui
***********************************************************/
void LbaNetEngine::DisplayGUI(int guinumber)
{
	switch(guinumber)
	{
		case -1: // got disconnected from server
			SwitchGuiToLogin();
			m_gui_handler.InformNotLoggedIn(-2, "");
		break;
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
		case 5: // inventory
			SwitchGuiToGame();
			m_gui_handler.ShowInventory();
		break;
	}
}


/***********************************************************
called to play the assigned music when menu
***********************************************************/
void LbaNetEngine::PlayMenuMusic()
{
	std::string mmusic = "Data/Lba1/Music/LBA1-Track9.mp3";

	std::string tmp = MusicHandler::getInstance()->GetCurrentMusic();
	if(tmp != mmusic)
	{
		m_lastmusic = tmp;
		MusicHandler::getInstance()->PlayMusic(mmusic, -1);
	}
}



/***********************************************************
focus the chatbox
***********************************************************/
void LbaNetEngine::FocusChatbox(bool focus)
{
	if(m_currentstate == EGaming)
		m_gui_handler.FocusChatbox(focus);
}



/***********************************************************
called when need to connect to game server
***********************************************************/
void LbaNetEngine::ConnectToGameServer(const std::string &ServerName, const std::string &ServerAddress)
{
	if(m_Gamecl)
		m_Gamecl->ConnectToServer(ServerName, ServerAddress, m_userlogin, m_userpass, 
											_CUR_LBANET_SERVER_VERSION_, &m_lbaNetModel);
}

/***********************************************************
called when game server unreachable
***********************************************************/
void LbaNetEngine::GameErrorMessage(const std::string &Message)
{
	CGMessageBox::getInstance()->Show("Error", Message);
}