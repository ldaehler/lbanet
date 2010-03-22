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
#include "SynchronizedTimeHandler.h"


/***********************************************************
	Constructor
***********************************************************/
LbaNetModel::LbaNetModel()
{
	LogHandler::getInstance()->LogToFile("Initializing model class...");

}


/***********************************************************
	Destructor
***********************************************************/
LbaNetModel::~LbaNetModel()
{
	// clear model
	ClearModel(LbaMainLightInfo());
}



/***********************************************************
set physic engine
***********************************************************/
void LbaNetModel::SetPhysicEngine(boost::shared_ptr<PhysXEngine> & pEngine)
{
	_physicEngine = pEngine;
}


/***********************************************************
do all check to be done when idle
***********************************************************/
void LbaNetModel::Process()
{
	double currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDoubleSync();
	float diff = (float)(currtime-m_lasttime);
	m_lasttime = currtime;


	// process all dynamic objects
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.begin();
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator end = _dynamicObjects.end();
	for(; it != end; ++it)
		it->second->Process();

	// process player controller
	if(m_controller)
		m_controller->Process(currtime, diff);

}





/***********************************************************
reset model with a new map
***********************************************************/
void LbaNetModel::SetMap(const ObjectInfo &mapInfo, const LbaMainLightInfo &NewLightningInfo)
{
	// clear previous map if there was one
	ClearModel(NewLightningInfo);

	_currMap = mapInfo.BuildSelf(_physicEngine);
}


/***********************************************************
clear current model before changing map
***********************************************************/
void LbaNetModel::ClearModel(const LbaMainLightInfo &NewLightningInfo)
{
	//clear dynamic object of the current scene
	_dynamicObjects.clear();

	// clear current map
	_currMap.reset();


	// clear physic engine
	if(_physicEngine)
		_physicEngine->Clear();


	//clear display engine
	OsgHandler::getInstance()->ResetDisplayTree(NewLightningInfo);
}



/***********************************************************
add object to the scene
***********************************************************/
void LbaNetModel::AddObject(long id, const ObjectInfo &desc)
{
	// remove object of same id if already there
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
		RemObject(id);

	_dynamicObjects[id] = desc.BuildSelf(_physicEngine);
}

/***********************************************************
remove object from the scene
***********************************************************/
void LbaNetModel::RemObject(long id)
{
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
	if(it != _dynamicObjects.end())
	{
		it->second->Destroy();
		_dynamicObjects.erase(it);
	}
}



/***********************************************************
remove object from the scene
***********************************************************/
boost::shared_ptr<DynamicObject> LbaNetModel::GetObject(long id)
{
	std::map<long, boost::shared_ptr<DynamicObject> >::iterator it = _dynamicObjects.find(id);
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
	LbaMainLightInfo linfo;
	linfo.UseLight = false;
	linfo.UseShadow = false;
	linfo.StartOn = true;
	ClearModel(linfo);
}


/***********************************************************
pause the game
***********************************************************/
void LbaNetModel::Pause()
{
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDoubleSync();
}

/***********************************************************
resume the game
***********************************************************/
void LbaNetModel::Resume(bool reinit)
{
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDoubleSync();
}


/***********************************************************
start a move from keyboard input
***********************************************************/
void LbaNetModel::StartMove(int MoveType)
{
	if(m_controller)
		m_controller->StartMove(MoveType);
}


/***********************************************************
stop a move from keyboard input
***********************************************************/
void LbaNetModel::StopMove(int MoveType)
{
	if(m_controller)
		m_controller->StopMove(MoveType);
}

/***********************************************************
do action from keyboard input
***********************************************************/
void LbaNetModel::DoAction()
{
	if(m_controller)
		m_controller->DoAction();
}


	//m_controller = boost::shared_ptr<CharacterController>(new CharacterController(m_physic_engine));

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
	