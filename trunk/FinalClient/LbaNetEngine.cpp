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

	// init OSG
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data");


	//init physic engine
	LogHandler::getInstance()->LogToFile("Initialize physic engine...");
	m_physic_engine = boost::shared_ptr<PhysXEngine>(new PhysXEngine());

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
}
















