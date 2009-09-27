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

//#include "InventoryHandler.h"
//#include "QuestHandler.h"
//#include "QuestBook.h"

#include "LbaNetModel.h"
#include "Camera.h"
#include "MapRenderer.h"
#include "DataLoader.h"
#include "PhysicHandler.h"
#include "MusicHandler.h"
#include "MainPlayerHandler.h"
#include "ConfigurationManager.h"
#include "ThreadSafeWorkpile.h"
#include "GuiHandler.h"
#include "ExternalPlayersHandler.h"
#include "LocalActorsHandler.h"
#include "SynchronizedTimeHandler.h"
#include "ExternalActorsHandler.h"
#include "LogHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library

// init color map
const short	LbaNetModel::m_body_color_map[] = {-1, 2, 19, 32, 36, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 243};


#ifndef _LBANET_SET_EDITOR_
#define _LBANET_SET_EDITOR_
#endif


/***********************************************************
	Constructor
***********************************************************/
LbaNetModel::LbaNetModel(GuiHandler*	guiH)
: _current_room_cut(-1), m_current_main_state(0), _game_paused(false),
	m_current_main_body(0), _guiH(guiH), m_current_main_body_color(0), m_debug_map(0), 
	m_room_y_cut(-1), m_need_full_check(false)
{
	LogHandler::getInstance()->LogToFile("Initializing model class...");

    _camera= new Camera();
	_mapRenderer = new MapRenderer();
	_localActorsHandler = new LocalActorsHandler();
	_externalActorsHandler = new ExternalActorsHandler();
	_physicHandler = new PhysicHandler(_localActorsHandler, _externalActorsHandler);
	_localActorsHandler->SetPhysic(_physicHandler);
	_externalActorsHandler->SetPhysic(_physicHandler);

	LogHandler::getInstance()->LogToFile("Reading configuration from file...");
	ConfigurationManager *	cm = ConfigurationManager::GetInstance();

	float NormalSpeed, SportySpeed, FightSpeed, DiscreteSpeed, HorseSpeed, DinoSpeed, JumpSpeed, JumpHeight, AnimationSpeed;
	cm->GetFloat("Speed.NormalSpeed", NormalSpeed);
	cm->GetFloat("Speed.SportySpeed", SportySpeed);
	cm->GetFloat("Speed.FightSpeed", FightSpeed);
	cm->GetFloat("Speed.DiscreteSpeed", DiscreteSpeed);
	cm->GetFloat("Speed.HorseSpeed", HorseSpeed);
	cm->GetFloat("Speed.DinoSpeed", DinoSpeed);
	cm->GetFloat("Speed.JumpSpeed", JumpSpeed);
	cm->GetFloat("Speed.JumpHeight", JumpHeight);
	cm->GetFloat("Speed.AnimationSpeed", AnimationSpeed);

	LogHandler::getInstance()->LogToFile("Creating main player character...");
	_mainPlayerHandler = new MainPlayerHandler(NormalSpeed, SportySpeed,
								FightSpeed, DiscreteSpeed, HorseSpeed, DinoSpeed, AnimationSpeed,
								JumpSpeed, JumpHeight, NormalSpeed/3, _physicHandler, _camera);

	m_main_actor_starting_X = 0;
	m_main_actor_starting_Y = 0;
	m_main_actor_starting_Z = 0;
	ReplaceMain();


	int body;
	ConfigurationManager::GetInstance()->GetInt("Player.Body", body);
	for(int i=0; i<body; ++i)
		IncreasePlayerBody();

	int bcolor;
	ConfigurationManager::GetInstance()->GetInt("Player.BodyColor", bcolor);
	for(int i=0; i<bcolor; ++i)
		IncreasePlayerBodyColor();

	LogHandler::getInstance()->LogToFile("Initializing camera...");

	bool perspec;
	double camdistance, camzenit;
	ConfigurationManager::GetInstance()->GetDouble("Camera.Distance", camdistance);
	ConfigurationManager::GetInstance()->GetDouble("Camera.Zenit", camzenit);
	ConfigurationManager::GetInstance()->GetBool("Camera.Perspective", perspec);

	_camera->SetDistance(camdistance);
	_camera->SetZenit(camzenit);
	_camera->SetPerspective(perspec);

	_externalPlayers = new ExternalPlayersHandler("", AnimationSpeed);
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetModel::~LbaNetModel()
{
	if(!m_zoomed_in)
	{
		ConfigurationManager::GetInstance()->SetDouble("Camera.Distance", _camera->GetDistance());
		ConfigurationManager::GetInstance()->SetDouble("Camera.Zenit", _camera->GetZenit());
		ConfigurationManager::GetInstance()->SetBool("Camera.Perspective", _camera->IsPerspective());
	}

	delete _camera;
	delete _mapRenderer;
	delete _physicHandler;
	delete _mainPlayerHandler;
	delete _localActorsHandler;
	delete _externalActorsHandler;
}

/***********************************************************
init function
***********************************************************/
void LbaNetModel::Initialize()
{
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0.5);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1,1,1);
    glColorMask(1,1,1,1);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
}

/***********************************************************
set screen size
***********************************************************/
void LbaNetModel::SetScreenSize(int screenX, int screenY)
{
	_windowWidth = screenX;
	_windowHeight = screenY;

	// reload the room
	if(_current_map != "")
	{
		Pause();
		MapInfo * MI = DataLoader::getInstance()->GetMapInfo();
		std::string mapN = "Data/" + MI->Files["Maps"];
		_mapRenderer->LoadMap(mapN, _physicHandler, MI);
		_localActorsHandler->Reload();
		_externalActorsHandler->Reload();
		Resume(false);
	}
}


/***********************************************************
draw the current state on the screen
***********************************************************/
void LbaNetModel::Draw()
{
	// set opengl default view
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.09f);
    glDisable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	if(_camera->IsPerspective())
	{
		gluPerspective(_camera->GetFOV(),_windowWidth/(double)_windowHeight,0.01,2000);
		glTranslated(0,0,-_camera->GetDistance());
		glRotated(_camera->GetZenit(),1,0,0);
	}
	else
	{
		glOrtho(-0.12*_camera->GetDistance(), 0.12*_camera->GetDistance(), -0.12*_camera->GetDistance(), 0.12*_camera->GetDistance(), -2000, 2000);
		glTranslated(0,0,-1000);
		glRotated(30,1,0,0);
	}


    glRotated(-_camera->GetAzimut()-45,0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslated(-_camera->GetTargetX(),-_camera->GetTargetY()/2.0,-_camera->GetTargetZ());


	// draw the map
	_mapRenderer->Render();

	// render local actors
	_localActorsHandler->Render(m_room_y_cut);
	_externalActorsHandler->Render(m_room_y_cut);


	// draw other players
	_externalPlayers->draw(m_room_y_cut);

	// draw the main player
	_mainPlayerHandler->Render();



#ifdef _LBANET_SET_EDITOR_
	// render editor stuff
	_mapRenderer->RenderEditor();
	_localActorsHandler->RenderEditor();
	_externalActorsHandler->RenderEditor();
#endif

	// reset opengl
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/***********************************************************
change the world
***********************************************************/
void LbaNetModel::ChangeWorld(const std::string & NewWorld)
{
	if(NewWorld != _current_world)
	{
		CleanupWorld();

		if(DataLoader::getInstance()->LoadWorld("./Data/" + NewWorld))
		{
			_current_world = NewWorld;
			std::string spawning;
			std::string mapname =  DataLoader::getInstance()->GetFirstMapName(spawning);

			//set the teleport list
			std::list<std::string> _tplists;
			_guiH->SetTeleportList(DataLoader::getInstance()->GetWorldInfo().Teleports);


			ChangeMap(mapname, spawning);
		}


	}
}

/***********************************************************
change the map
***********************************************************/
void LbaNetModel::ChangeMap(const std::string & NewMap, const std::string & Spawning,
							float Xoffset, float Yoffset, float Zoffset)
{
	if(DataLoader::getInstance()->LoadMap(NewMap))
	{
		MapInfo * MI = DataLoader::getInstance()->GetMapInfo();
		ChangeMap(NewMap,	MI->Spawnings[Spawning].PosX+Xoffset,
							MI->Spawnings[Spawning].PosY+Yoffset,
							MI->Spawnings[Spawning].PosZ+Zoffset);

		_mainPlayerHandler->SetRotation(_mainPlayerHandler->GetRotation() + MI->Spawnings[Spawning].Rotation);
	}
}


/***********************************************************
change the map
***********************************************************/
void LbaNetModel::ChangeMap(const std::string & NewMap, float X, float Y, float Z)
{
	if(NewMap != _current_map)
	{
		CleanupMap();
		ThreadSafeWorkpile::getInstance()->ChangeMap(_current_world.substr(0, _current_world.find(".xml")), NewMap);
		Pause();
		if(DataLoader::getInstance()->LoadMap(NewMap))
		{
			// load the map
			MapInfo * MI = DataLoader::getInstance()->GetMapInfo();
			std::string mapN = "Data/" + MI->Files["Maps"];
			_mapRenderer->LoadMap(mapN, _physicHandler, MI);

			m_room_y_cut = -1;

			MusicHandler::getInstance()->PlayMusic("Data/" + MI->Music, MI->MusicLoop);
			_current_map = NewMap;
			_actor_in_exterior = (MI->Type == "exterior");


			_externalPlayers->ResetActors(_current_map);

			// load local actors
			std::map<long, Actor *> vec;
			DataLoader::getInstance()->GetLocalMapActors(vec);
			_localActorsHandler->SetActors(vec);

			std::map<long, Actor *> vec2;
			DataLoader::getInstance()->GetExternalMapActors(vec2);
			_externalActorsHandler->SetActors(vec2);

			_guiH->SetCurrentMap(_current_world.substr(0, _current_world.find(".xml")), _current_map);

			Resume(true);
		}
	}

	m_main_actor_starting_X = X;
	m_main_actor_starting_Y = Y;
	m_main_actor_starting_Z = Z;
	_mainPlayerHandler->SetAttached(false);
	ReplaceMain();
	
}


/***********************************************************
clean up all loaded world data in memory
***********************************************************/
void LbaNetModel::CleanupWorld()
{
	CleanupMap();
	_current_world = "";
}

/***********************************************************
clean up all loaded map data in memory
***********************************************************/
void LbaNetModel::CleanupMap()
{
	_current_map="";
	_mapRenderer->CleanUp();
	_physicHandler->ClearMemory();
	_externalPlayers->ResetActors(_current_map);
	_localActorsHandler->Cleanup();
	_externalActorsHandler->Cleanup();
}


/***********************************************************
update camera functions
***********************************************************/
void LbaNetModel::DeltaUpdateZoom(bool ZoomIn)
{
	if(ZoomIn)
		_camera->DeltaUpdateDistance(5);
	else
		_camera->DeltaUpdateDistance(-5);
}


/***********************************************************
update camera functions
***********************************************************/
void LbaNetModel::DeltaUpdateZenit(double zenit)
{
		_camera->DeltaUpdateZenit(zenit);
}



/***********************************************************
do all check to be done when idle
***********************************************************/
int LbaNetModel::Process()
{
	if(_game_paused)
		return 1;

	std::vector<ActorStateInfo> newstate;
	if(ThreadSafeWorkpile::getInstance()->ActorStatesUpdated(newstate))
	{
		_localActorsHandler->UpdateActorStates(newstate);
		_externalActorsHandler->UpdateActorStates(newstate);;
	}

	if(_mainPlayerHandler->IsMoving())
	{
		// check if we need to cut the map roof
		if(!_actor_in_exterior)
		{
			m_room_y_cut = _mainPlayerHandler->IsUnderRoof();
			_mapRenderer->CutRoom(m_room_y_cut);
		}
	}


	if(_mainPlayerHandler->IsMoving() || _mainPlayerHandler->IsScriptedEvent() || 
		_mainPlayerHandler->IsAttached() || _mainPlayerHandler->NeedCheck() || NeedFullCheck())
	{
		std::string newRoom = "";
		std::string newSpawning = "";
		float Xoffset, Yoffset, Zoffset;
		if(_mapRenderer->CheckIfExitRoom(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(), _mainPlayerHandler->GetPosZ(),
											newRoom, newSpawning, Xoffset, Yoffset, Zoffset))
		{
			ChangeMap(newRoom, newSpawning, Xoffset, Yoffset, Zoffset);

			// check in new map if we need to cut the map roof
			if(!_actor_in_exterior)
			{
				m_room_y_cut = _mainPlayerHandler->IsUnderRoof();
				_mapRenderer->CutRoom(m_room_y_cut);
			}
		}
		//else
		{
			if(!_localActorsHandler->ActivateZone(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
											_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation(), _mainPlayerHandler))

			_externalActorsHandler->ActivateZone(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
											_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation(), _mainPlayerHandler);


			if(_mainPlayerHandler->IsAttached())
			{
				if(_localActorsHandler->CheckDettach((Actor *)_mainPlayerHandler->GetPlayer())
					|| _externalActorsHandler->CheckDettach((Actor *)_mainPlayerHandler->GetPlayer())
					)
				{
					_mainPlayerHandler->SetAttached(false);
					_mainPlayerHandler->CheckY();
				}
			}
			else
			{
				if(_localActorsHandler->CheckAttach((Actor *)_mainPlayerHandler->GetPlayer())
					|| _externalActorsHandler->CheckAttach((Actor *)_mainPlayerHandler->GetPlayer())
					)
					_mainPlayerHandler->SetAttached(true);
			}

		}
	}


	double tnow = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	float tdiff = (float)(tnow - _last_time);
	_last_time = tnow;
	if(tdiff <= 0)
		return 1;

	// process main player
	switch(_mainPlayerHandler->Process(tnow, tdiff))
	{
		case 0:	// the actor is moving
		break;

		case 1:	// the actor is in water - should drawn
			if(m_current_main_state == 0)
			{
				m_current_main_state = 1;
				_mainPlayerHandler->Startdrowning();
			}
		break;

		case 2:	// the actor should die
			if(m_current_main_state == 0)
			{
				m_current_main_state = 2;
				_mainPlayerHandler->Startdying();
			}
		break;

		case 4:	// the actor is being hurt
			if(m_current_main_state == 0)
			{
				m_current_main_state = 3;
			}
		break;


		case 3:	// if actor animation is terminated
			if(m_current_main_state == 1) // if actor is currently drawning - terminate
			{
				m_current_main_state = 0;
				_mainPlayerHandler->Stopstate();
				ReplaceMain();
			}

			if(m_current_main_state == 2)	// if actor is currently dying - terminate
			{
				m_current_main_state = 0;
				_mainPlayerHandler->Stopstate();
				ReplaceMain();
			}

			if(m_current_main_state == 3)	// if actor is currently being hurt - terminate
			{
				m_current_main_state = 0;
				_mainPlayerHandler->Stopstate();
			}
		break;
	}

	_externalPlayers->Process(tnow, tdiff);
	_localActorsHandler->Process(tnow, tdiff);
	_externalActorsHandler->Process(tnow, tdiff);

	return 1;
}




/***********************************************************
set the player name
***********************************************************/
void LbaNetModel::SetPlayerName(const std::string & name)
{
	_mainPlayerHandler->SetName(name);
	_externalPlayers->SetMainName(name);
}



/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerStartMove(int moveDirection)
{
	_mainPlayerHandler->PlayerStartMove(moveDirection);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerStopMove(int moveDirection)
{
	_mainPlayerHandler->PlayerStopMove(moveDirection);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerStartMoveUpDown(bool Up)
{
	_mainPlayerHandler->PlayerStartMoveUpDown(Up);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerStopMoveUpDown(bool Up)
{
	_mainPlayerHandler->PlayerStopMoveUpDown(Up);
}


/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerChangeStance(int NewStance)
{
	if(_actor_in_exterior)
		_mainPlayerHandler->PlayerChangeStance(NewStance);
	else
	{
		if(NewStance != 5 && NewStance != 6)
			_mainPlayerHandler->PlayerChangeStance(NewStance);
	}
}


/***********************************************************
player moves
***********************************************************/
void LbaNetModel::PlayerDoAction(bool ForcedNormalAction)
{
	if(_mainPlayerHandler->ActivationMode(ForcedNormalAction))
	{
		if(!_localActorsHandler->Activate(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
										_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation()))

		_externalActorsHandler->Activate(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
										_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation());
	}
	else
		_mainPlayerHandler->DoAction();
}


/***********************************************************
replace main actor and camera to the starting point of the current room
***********************************************************/
void LbaNetModel::ReplaceMain()
{
	_mainPlayerHandler->Stopstate();

	_mainPlayerHandler->SetPosition(	m_main_actor_starting_X,
										m_main_actor_starting_Y,
										m_main_actor_starting_Z);

	_camera->SetTarget(	m_main_actor_starting_X,
						m_main_actor_starting_Y,
						m_main_actor_starting_Z);

	_mainPlayerHandler->UpdateFloorY();
	_mainPlayerHandler->CheckY();

	if(!_localActorsHandler->ActivateZone(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
									_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation(), _mainPlayerHandler))

	_externalActorsHandler->ActivateZone(_mainPlayerHandler->GetPosX(), _mainPlayerHandler->GetPosY(),
									_mainPlayerHandler->GetPosZ(), _mainPlayerHandler->GetRotation(), _mainPlayerHandler);
}


/***********************************************************
pause and resume game in case we enter menu
***********************************************************/
void LbaNetModel::Pause()
{
	if(_game_paused)
		return;

	_mainPlayerHandler->Pause();
	_game_paused = true;
}

/***********************************************************
pause and resume game in case we enter menu
***********************************************************/
void LbaNetModel::Resume(bool reinit)
{
	if(!_game_paused)
		return;

	_last_time = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	_mainPlayerHandler->Resume(!_actor_in_exterior, reinit);
	_game_paused = false;
}


/***********************************************************
zoom the player fro login screen
***********************************************************/
void LbaNetModel::ZoomInPlayerForLogin()
{
	_actor_in_exterior = false;
	m_main_actor_starting_X = 0;
	m_main_actor_starting_Y = 0;
	m_main_actor_starting_Z = 0;
	ReplaceMain();
	_mainPlayerHandler->SetRotation(0);
	SetPlayerName("");
	PlayerChangeStance(1);
	Resume(true);
	m_zoomed_in = true;
	m_remember_zoom = _camera->GetDistance();
	m_remember_zenit = _camera->GetZenit();
	m_remember_perspective = _camera->IsPerspective();
	_camera->SetDistance(15);
	_camera->SetZenit(30);
	_camera->SetPerspective(true);
}

/***********************************************************
reset zoom
***********************************************************/
void LbaNetModel::ResetZoom()
{
	if(!m_zoomed_in)
		return;

	m_zoomed_in = false;
	_camera->SetDistance(m_remember_zoom);
	_camera->SetZenit(m_remember_zenit);
	_camera->SetPerspective(m_remember_perspective);

	PlayerChangeStance(1);
	_mainPlayerHandler->SetRotation(0);
}


/***********************************************************
change player body
***********************************************************/
void LbaNetModel::IncreasePlayerBody()
{
	if(m_current_main_body > 1)
		return;

	++m_current_main_body;
	_mainPlayerHandler->SetBody(m_current_main_body);
	ConfigurationManager::GetInstance()->SetInt("Player.Body", m_current_main_body);
}

/***********************************************************
change player body
***********************************************************/
void LbaNetModel::DecreasePlayerBody()
{
	if(m_current_main_body < 1)
		return;

	--m_current_main_body;
	_mainPlayerHandler->SetBody(m_current_main_body);
	ConfigurationManager::GetInstance()->SetInt("Player.Body", m_current_main_body);
}


/***********************************************************
change player body color
***********************************************************/
void LbaNetModel::IncreasePlayerBodyColor()
{
	if(m_current_main_body_color > 17)
		return;

	++m_current_main_body_color;
	_mainPlayerHandler->SetBodyColor(m_body_color_map[m_current_main_body_color]);
	ConfigurationManager::GetInstance()->SetInt("Player.BodyColor", m_current_main_body_color);
}

/***********************************************************
change player body color
***********************************************************/
void LbaNetModel::DecreasePlayerBodyColor()
{
	if(m_current_main_body_color < 1)
		return;


	--m_current_main_body_color;
	_mainPlayerHandler->SetBodyColor(m_body_color_map[m_current_main_body_color]);
	ConfigurationManager::GetInstance()->SetInt("Player.BodyColor", m_current_main_body_color);
}



/***********************************************************
display a scripted event using the player character
***********************************************************/
void LbaNetModel::DoPlayerScriptedEvent(const std::vector<PlayerScriptPart> &script)
{
	_mainPlayerHandler->DoPlayerScriptedEvent(script);
}


/***********************************************************
Debug function
***********************************************************/
void LbaNetModel::GoNextRoom()
{
	//m_debug_map++;
	//WorldInfo & wi = DataLoader::getInstance()->GetWorldInfo();

	//std::map<std::string, MapInfo>::iterator it = wi.Maps.begin();
	//for(int cc=0; cc<m_debug_map && it != wi.Maps.end(); ++it, ++cc);

	//if(it->second.Spawnings.size() > 0)
	//	ChangeMap(it->first, it->second.Spawnings.begin()->first);
}


/***********************************************************
called when a signal has been generated
***********************************************************/
void LbaNetModel::SignalEvent(long signal, const std::vector<long> &targets)
{
	_localActorsHandler->SignalEvent(signal, targets);
	_externalActorsHandler->SignalEvent(signal, targets);
}


/***********************************************************
change camera perspective
***********************************************************/
void LbaNetModel::ChangePespective(bool perspective)
{
	_camera->SetPerspective(perspective);
}



/***********************************************************
display map exits
***********************************************************/
void LbaNetModel::DisplayExits(bool display)
{
	_mapRenderer->DisplayExits(display);
}




/***********************************************************
player is hurt by an actor
***********************************************************/
void LbaNetModel::PlayerHurt(long actorid)
{
	_mainPlayerHandler->PlayerHurt(actorid);
}



/***********************************************************
player life changed
***********************************************************/
void LbaNetModel::PlayerLifeChanged(float CurLife, float MaxLife, float CurMana, float MaxMana)
{
	if(_mainPlayerHandler->PlayerLifeChanged(CurLife, MaxLife, CurMana, MaxMana))
		m_current_main_state = 2;
}


// get a pointer to the actors
std::map<long, Actor *> * LbaNetModel::GetLActors(){return _localActorsHandler->GetActors();}
std::map<long, Actor *> * LbaNetModel::GetEActors(){return _externalActorsHandler->GetActors();}