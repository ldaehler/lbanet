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

#if !defined(__LbaNetModel_1_LbaNetModel_h)
#define __LbaNetModel_1_LbaNetModel_h

class InventoryHandler;
class MainPlayerHandler;
class MapRenderer;
class LocalActorsHandler;
class ExternalActorsHandler;
class PhysicHandler;
class QuestHandler;
class QuestBook;
class Camera;
class GuiHandler;
class ExternalPlayersHandler;
struct PlayerScriptPart;
class Actor;



#include <string>
#include <vector>
#include <map>

/***********************************************************************
 * Module:  LbaNetModel.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 13:54:52
 * Purpose: Declaration of the class LbaNetModel
 ***********************************************************************/
class LbaNetModel
{
public:
	//! constructor
	LbaNetModel(GuiHandler*	guiH);

	//! destructor
	~LbaNetModel();

	//! init function
	void Initialize();

	//! set screen size
	void SetScreenSize(int screenX, int screenY);

	//! draw the current state on the screen
	void Draw();

	//! draw the current state on the screen
	void DrawOnlyChar();

	//! draw the current state on the screen
	void DrawForLogin();

	//! do all check to be done when idle
	int Process();

	// change the world
	void ChangeWorld(const std::string & NewWorld);

	// change the map
	void ChangeMap(const std::string & NewMap, const std::string & Spawning,
					float Xoffset=0, float Yoffset=0, float Zoffset=0);

	// change the map
	void ChangeMap(const std::string & NewMap, float X, float Y, float Z, float R);

	// clean up all loaded world data in memory
	void CleanupWorld();

	// clean up all loaded map data in memory
	void CleanupMap();

	// update camera functions
	void DeltaUpdateZoom(bool ZoomIn);
	void DeltaUpdateZenit(double zenit);

	// set the player name
	void SetPlayerName(const std::string & name);

	// player moves
	void PlayerStartMove(int moveDirection);
	void PlayerStopMove(int moveDirection);
	void PlayerStartMoveUpDown(bool Up);
	void PlayerStopMoveUpDown(bool Up);
	void PlayerChangeStance(int NewStance);
	void PlayerDoAction(bool ForcedNormalAction);


	// pause and resume game in case we enter menu
	void Pause();
	void Resume(bool reinit);

	// zoom the player fro login screen
	void ZoomInPlayerForLogin();

	// reset zoom
	void ResetZoom();

	// change player body
	void IncreasePlayerBody();
	void DecreasePlayerBody();

	// replace main actor and camera to the starting point of the current room
	void ReplaceMain();


	// change player body color
	void IncreasePlayerBodyColor();
	void DecreasePlayerBodyColor();

	// display a scripted event using the player character
	void DoPlayerScriptedEvent(const std::vector<PlayerScriptPart> &script);

	// debug function
	void GoNextRoom();

	// called when a signal has been generated
	void SignalEvent(long signal, const std::vector<long> &targets);


	// get a pointer to the actors
	std::map<long, Actor *> * GetLActors();
	std::map<long, Actor *> * GetEActors();

	// change camera perspective
	void ChangePespective(bool perspective);

	// display map extis
	void DisplayExits(bool display);

	// player is hurt by an actor
	void PlayerHurt(long actorid);

	// ask to do a full position check
	void DoFullCheckEvent()
	{m_need_full_check = true;}


	// return true if we need full check
	bool NeedFullCheck()
	{
		if(m_need_full_check)
		{
			m_need_full_check = false;
			return true;
		}

		return false;
	}

	//player life changed
	void PlayerLifeChanged(float CurLife, float MaxLife, float CurMana, float MaxMana);

	//! clean up the textures before resizing
	void CleanupTexture();

	// set player name color
	void SetPlayerNameColor(int R, int G, int B);


	// called when and object from the inventory is used
	void InventoryUsed(long ObjectId, bool LifeFull, bool ManaFull);


private:
	MapRenderer*			_mapRenderer;
	PhysicHandler*			_physicHandler;
	MainPlayerHandler*		_mainPlayerHandler;
	Camera *				_camera;
	ExternalPlayersHandler *_externalPlayers;

	LocalActorsHandler*		_localActorsHandler;
	ExternalActorsHandler*	_externalActorsHandler;

	InventoryHandler*		_inventoryHandler;
	QuestHandler*			_questHandler;
	QuestBook*				_questBook;
	GuiHandler*				_guiH;

	std::string				_current_world;		// the current world in play
	std::string				_current_map;		// the current map in play

	int						_windowWidth;
	int						_windowHeight;
	int						_current_room_cut;
	bool					_actor_in_exterior;

	bool					_game_paused;
	double					_last_time;

	// starting coordinate of the main actor in the current room
	float					m_main_actor_starting_X;
	float					m_main_actor_starting_Y;
	float					m_main_actor_starting_Z;
	float					m_main_actor_starting_Rotation;
	int						m_current_main_state;
	int						m_current_main_body;
	int						m_current_main_body_color;

	bool					m_zoomed_in;
	double					m_remember_zoom;
	double					m_remember_zenit;
	bool					m_remember_perspective;

	static const short		m_body_color_map[];

	int						m_room_y_cut;

	int						m_debug_map;

	bool					m_need_full_check;

	float					m_AnimationSpeed;
};

#endif