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

/***********************************************************
	Constructor
***********************************************************/
LbaNetEngine::LbaNetEngine()
{
	Initialize();
}


/***********************************************************
	Destructor
***********************************************************/
LbaNetEngine::~LbaNetEngine()
{	
	LogHandler::getInstance()->LogToFile("Finalizing Game...");

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

	// init event handler
	m_eventHandler = boost::shared_ptr<EventHandler>(new EventHandler(this));

	// init OSG
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data", m_eventHandler.get());


	//init physic engine
	LogHandler::getInstance()->LogToFile("Initialize physic engine...");
	m_physic_engine = boost::shared_ptr<PhysXEngine>(new PhysXEngine());

	m_physic_engine2 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine3 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine4 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine5 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine6 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine7 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine8 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine9 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine10 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine11 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine12 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine13 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine14 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine15 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine16 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine17 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine18 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine19 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());
	m_physic_engine20 = boost::shared_ptr<PhysXEngine>(new PhysXEngine());


	m_controller = boost::shared_ptr<CharacterController>(new CharacterController(m_physic_engine));

	// set engine to model
	m_lbaNetModel.SetPhysicEngine(m_physic_engine);

	LogHandler::getInstance()->LogToFile("Initializing Completed.");

	{
		boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgSimpleObjectDescription("Lba1/Maps/map0.osgb"));
		// maps are always 1Y too much up, remove that
		boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
		Tr->translationY = -1;
		boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));

		boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionTriangleMesh(0, 0, 0, "Data/Lba1/Maps/map0.phy"));

		ObjectInfo mapinfo(DInfo, Pyd, true);

		LbaMainLightInfo linfo;
		linfo.UseLight = true;
		linfo.UseShadow = true;
		linfo.StartOn = false;

		linfo.LOnPosX=50.f;
		linfo.LOnPosY=100.f;
		linfo.LOnPosZ=50.f;
		linfo.LOnAmbientR=0.6f;
		linfo.LOnAmbientG=0.6f;
		linfo.LOnAmbientB=0.6f;
		linfo.LOnDiffuseR=0.8f;
		linfo.LOnDiffuseG=0.8f;
		linfo.LOnDiffuseB=0.8f;

		linfo.LOffPosX=100.f;
		linfo.LOffPosY=50.f;
		linfo.LOffPosZ=100.f;
		linfo.LOffAmbientR=0.0f;
		linfo.LOffAmbientG=0.0f;
		linfo.LOffAmbientB=0.0f;
		linfo.LOffDiffuseR=0.5f;
		linfo.LOffDiffuseG=0.5f;
		linfo.LOffDiffuseB=0.5f;

		m_lbaNetModel.SetMap(mapinfo, linfo);
	}

	{
		boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgOrientedCapsuleDescription(4, 0.5, 0.5, 0, 0, 1));
		boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
		Tr->rotation = LbaQuaternion(90, LbaVec3(1, 0, 0));

		boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));

		boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionCapsule(20, 30, 40, 4, 1, LbaQuaternion(), 0.5, 4));

		ObjectInfo objinfo(DInfo, Pyd, false, true);
		m_lbaNetModel.AddObject(1, objinfo);

		m_controller->SetCharacter(m_lbaNetModel.GetObject(1));

		m_lbaNetModel.GetObject(1)->GetDisplayObject()->SetCameraFollow();
	}
}


/***********************************************************
entry point of the engine
***********************************************************/
void LbaNetEngine::run(void)
{
	// init time variable
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDoubleSync();

	try
	{
		// Loop until a quit event is found
		while(!OsgHandler::getInstance()->Update())
		{
			//get physic results
			m_physic_engine->GetPhysicsResults();


			// process stuff between frame
			Process();

			//start physic calculation
			m_physic_engine->StartPhysics();
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
	// process model (update display stuff)
	m_lbaNetModel.Process();

	double currtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDoubleSync();
	float diff = (float)(currtime-m_lasttime);
	m_lasttime = currtime;
	m_controller->Process(currtime, diff);

}



/***********************************************************
start a move from keyboard input
***********************************************************/
void LbaNetEngine::StartMove(int MoveType)
{
	m_controller->StartMove(MoveType);
}


/***********************************************************
stop a move from keyboard input
***********************************************************/
void LbaNetEngine::StopMove(int MoveType)
{
	m_controller->StopMove(MoveType);
}

/***********************************************************
do action from keyboard input
***********************************************************/
void LbaNetEngine::DoAction()
{
	m_controller->DoAction();
}














