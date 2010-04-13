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

#include <string>
#include <vector>
#include <map>

#include "ObjectsDescription.h"
#include "DynamicObject.h"
#include "GameClientCallbackBase.h"
#include "CommonTypes.h"

#include <boost/shared_ptr.hpp>

class PhysXEngine;
class CharacterController;
class RoomCutController;
class CameraController;


/***********************************************************************
 * Module:  LbaNetModel.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 13:54:52
 * Purpose: Declaration of the class LbaNetModel
 ***********************************************************************/
class LbaNetModel : public GameClientCallbackBase
{
public:
	//! constructor
	LbaNetModel();

	//! destructor
	~LbaNetModel();


	//! set physic engine
	void SetPhysicEngine(boost::shared_ptr<PhysXEngine> & pEngine);

	//! do all check to be done when idle
	void Process();


	//! update the element to draw on screen
	void UpdateDrawing();


	//! add object to the scene
	//! if IsMainPlayer then treat this object as the player object
	virtual boost::shared_ptr<PhysicalObjectHandlerBase> 
		AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer);

	//! remove object from the scene
	virtual void RemObject(unsigned int id);

	//! get object from the scene
	boost::shared_ptr<DynamicObject> GetObject(unsigned int id);

	//! clean up everything
	void CleanupWorld();

	//! pause the game
	void Pause();

	//! resume the game
	void Resume(bool reinit);


	//! start a move from keyboard input
	void StartMove(int MoveType);

	//! stop a move from keyboard input
	void StopMove(int MoveType);

	//! do action from keyboard input
	void DoAction();

	//! inform that we entered a new map
	void NewMapEvent(const std::string & MapName, const std::string & MapType);

	// get last player inputs
	virtual Input GetLastPlayerInput();

	//apply inputs
	virtual void ApplyInputs(const Input & in);

protected:

	//! reset player object
	void ResetPlayerObject();

private:
	//physic engine
	boost::shared_ptr<PhysXEngine>						_physicEngine;

	// list of object populating the scene
	std::map<unsigned int, boost::shared_ptr<DynamicObject> >	_dynamicObjects;

	// last cycle time
	double												m_lasttime;

	// controllers
	boost::shared_ptr<CharacterController>				m_controllerChar;
	boost::shared_ptr<RoomCutController>				m_controllerRC;
	boost::shared_ptr<CameraController>					m_controllerCam;

	// player object
	unsigned int										m_playerObjectId;
	boost::shared_ptr<DynamicObject>					m_playerObject;



	//last player input
	Input												m_player_input;
};

#endif