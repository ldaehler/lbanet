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

#include "LbaNetModel.h"
#include "LogHandler.h"
#include "PhysXEngine.h"
#include "OSGHandler.h"
#include "CharacterController.h"
#include "RoomCutController.h"
#include "CameraController.h"
#include "SynchronizedTimeHandler.h"
#include "StaticObject.h"

/***********************************************************
	Constructor
***********************************************************/
LbaNetModel::LbaNetModel()
: m_playerObjectId(0)
{
	LogHandler::getInstance()->LogToFile("Initializing model class...");
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync()*0.001;
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetModel::~LbaNetModel()
{
}



/***********************************************************
set physic engine
***********************************************************/
void LbaNetModel::SetPhysicEngine(boost::shared_ptr<PhysXEngine> & pEngine)
{
	_physicEngine = pEngine;

	//initialize controllers
	m_controllerChar = boost::shared_ptr<CharacterController>(new CharacterController(_physicEngine));
	m_controllerRC = boost::shared_ptr<RoomCutController>(new RoomCutController(_physicEngine));
	m_controllerCam = boost::shared_ptr<CameraController>(new CameraController());
}


/***********************************************************
do all check to be done when idle
***********************************************************/
void LbaNetModel::Process()
{
	unsigned int ctime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
	double currtime = ctime*0.001;
	float diff = (float)(currtime-m_lasttime);
	m_lasttime = currtime;


	// process all dynamic objects
	std::map<unsigned int, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.begin();
	std::map<unsigned int, boost::shared_ptr<DynamicObject> >::iterator end = _dynamicObjects.end();
	for(; it != end; ++it)
		it->second->Process();

	//process player object
	if(m_playerObject)
		m_playerObject->Process();

	// process controllers
	if(m_controllerChar)
		m_controllerChar->Process(ctime, diff);
	if(m_controllerRC)
		m_controllerRC->Process();
	if(m_controllerCam)
		m_controllerCam->Process();
}




/***********************************************************
add object to the scene
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> 
	LbaNetModel::AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer)
{
	//special treatment if main object
	if(IsMainPlayer)
	{
		m_playerObjectId = id;
		m_playerObject = desc.BuildSelf(_physicEngine, id, OsgHandler::getInstance());

		if(m_controllerChar)
			m_controllerChar->SetCharacter(m_playerObject);
		if(m_controllerRC)
			m_controllerRC->SetCharacter(m_playerObject);
		if(m_controllerCam)
			m_controllerCam->SetCharacter(m_playerObject);

		return m_playerObject->GetPhysicalObject();
	}
	else
	{
		boost::shared_ptr<DynamicObject> tmpobj = desc.BuildSelf(_physicEngine, id, OsgHandler::getInstance());
		_dynamicObjects[id] = tmpobj;

		return tmpobj->GetPhysicalObject();
	}
}


/***********************************************************
remove object from the scene
***********************************************************/
void LbaNetModel::RemObject(unsigned int id)
{
	//special treatment if main object
	if(id == m_playerObjectId)
	{
		ResetPlayerObject();
	}
	else
	{
		std::map<unsigned int, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
		if(it != _dynamicObjects.end())
			_dynamicObjects.erase(it);
	}
}


/***********************************************************
remove object from the scene
***********************************************************/
boost::shared_ptr<DynamicObject> LbaNetModel::GetObject(unsigned int id)
{
	std::map<unsigned int, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
	{
		return it->second;
	}

	return boost::shared_ptr<DynamicObject>();
}



/***********************************************************
clean up everything
***********************************************************/
void LbaNetModel::CleanupWorld()
{
	//clear dynamic object of the current scene
	_dynamicObjects.clear();

	//clean up player
	ResetPlayerObject();


	//TODO - only for testing
	//boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgSimpleObjectDescription("Lba1/Maps/map0.osgb"));
	//// maps are always 1Y too much up, remove that
	//boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
	//Tr->translationY = -1;
	//boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));
	//boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionTriangleMesh(0, 0, 0, "Lba1/Maps/map0.phy"));
	//ObjectInfo mapinfo(DInfo, Pyd, true);
	//AddObject(1, mapinfo, false);


	// clear physic engine
	//if(_physicEngine)
	//	_physicEngine->Clear();


	//clear display engine
	//OsgHandler::getInstance()->ResetDisplayTree();
}


/***********************************************************
pause the game
***********************************************************/
void LbaNetModel::Pause()
{
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync()*0.001;
}

/***********************************************************
resume the game
***********************************************************/
void LbaNetModel::Resume(bool reinit)
{
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync()*0.001;
}


/***********************************************************
start a move from keyboard input
***********************************************************/
void LbaNetModel::StartMove(int MoveType)
{
	if(MoveType == 1 || MoveType == 2)
		if(m_player_input.up || m_player_input.down)
			return;

	if(MoveType == 3 && m_player_input.left)
		return;

	if(MoveType == 4 && m_player_input.right)
		return;

	switch(MoveType)
	{
		case 1:
			m_player_input.up = true;
		break;
		case 2:
			m_player_input.down = true;
		break;
		case 3:
			m_player_input.left = true;
			m_player_input.right = false;
		break;
		case 4:
			m_player_input.left = false;
			 m_player_input.right = true;
		break;
	}
}


/***********************************************************
stop a move from keyboard input
***********************************************************/
void LbaNetModel::StopMove(int MoveType)
{
	switch(MoveType)
	{
		case 1:
			m_player_input.up = false;
		break;
		case 2:
			m_player_input.down = false;
		break;
		case 3:
			m_player_input.left = false;
		break;
		case 4:
			 m_player_input.right = false;
		break;
	}
}

/***********************************************************
do action from keyboard input
***********************************************************/
void LbaNetModel::DoAction()
{
	//if(m_controllerChar)
	//	m_controllerChar->DoAction();
}



/***********************************************************
reset player object
***********************************************************/
void LbaNetModel::ResetPlayerObject()
{
	m_playerObjectId = 0;

	boost::shared_ptr<PhysicalObjectHandlerBase> physo(new SimplePhysicalObjectHandler(0, 0, 0, LbaQuaternion()));
	m_playerObject = boost::shared_ptr<DynamicObject>(new StaticObject(physo, boost::shared_ptr<DisplayObjectHandlerBase>(), m_playerObjectId));

	if(m_controllerChar)
		m_controllerChar->SetCharacter(m_playerObject, true);
	if(m_controllerRC)
		m_controllerRC->SetCharacter(m_playerObject, true);
	if(m_controllerCam)
		m_controllerCam->SetCharacter(m_playerObject, true);
}




/***********************************************************
get last player inputs
***********************************************************/
Input LbaNetModel::GetLastPlayerInput()
{
	return m_player_input;
}

/***********************************************************
apply inputs
***********************************************************/
void LbaNetModel::ApplyInputs(const Input & in)
{
	if(m_controllerChar)
		m_controllerChar->ApplyInputs(in);
}


/***********************************************************
inform that we entered a new map
***********************************************************/
void LbaNetModel::NewMapEvent(const std::string & MapName, const std::string & MapType)
{
}



	//

	//{
	//	boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgSimpleObjectDescription("Lba1/Maps/map0.osgb"));
	//	// maps are always 1Y too much up, remove that
	//	boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
	//	Tr->translationY = -1;
	//	boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));

	//	boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionTriangleMesh(0, 0, 0, "Data/Lba1/Maps/map0.phy"));

	//	ObjectInfo mapinfo(DInfo, Pyd, true);

	//	LbaMainLightInfo linfo;
	//	linfo.UseLight = true;
	//	linfo.UseShadow = true;
	//	linfo.StartOn = false;

	//	linfo.LOnPosX=50.f;
	//	linfo.LOnPosY=100.f;
	//	linfo.LOnPosZ=50.f;
	//	linfo.LOnAmbientR=0.6f;
	//	linfo.LOnAmbientG=0.6f;
	//	linfo.LOnAmbientB=0.6f;
	//	linfo.LOnDiffuseR=0.8f;
	//	linfo.LOnDiffuseG=0.8f;
	//	linfo.LOnDiffuseB=0.8f;

	//	linfo.LOffPosX=100.f;
	//	linfo.LOffPosY=50.f;
	//	linfo.LOffPosZ=100.f;
	//	linfo.LOffAmbientR=0.0f;
	//	linfo.LOffAmbientG=0.0f;
	//	linfo.LOffAmbientB=0.0f;
	//	linfo.LOffDiffuseR=0.5f;
	//	linfo.LOffDiffuseG=0.5f;
	//	linfo.LOffDiffuseB=0.5f;

	//	m_lbaNetModel.SetMap(mapinfo, linfo);
	//}

	//{
	//	boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgOrientedCapsuleDescription(4, 0.5, 0.5, 0, 0, 1));
	//	boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
	//	Tr->rotation = LbaQuaternion(90, LbaVec3(1, 0, 0));

	//	boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));

	//	boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionCapsule(20, 30, 40, 4, 1, LbaQuaternion(), 0.5, 4));

	//	ObjectInfo objinfo(DInfo, Pyd, false, true);
	//	m_lbaNetModel.AddObject(1, objinfo);

	//	m_controller->SetCharacter(m_lbaNetModel.GetObject(1));

	//	m_lbaNetModel.GetObject(1)->GetDisplayObject()->SetCameraFollow();
	//}
	