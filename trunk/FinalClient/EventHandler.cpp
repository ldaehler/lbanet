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

#include "EventHandler.h"
#include "LbaNetEngine.h"
#include "LogHandler.h"

#include <osgGA/GUIEventHandler>


/***********************************************************
	Constructor
***********************************************************/
EventHandler::EventHandler(LbaNetEngine* engine)
: _lbaNetEngine(engine)
{

}


/***********************************************************
	Destructor
***********************************************************/
EventHandler::~EventHandler()
{

}



/***********************************************************
handle function
***********************************************************/
bool EventHandler::Handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//if(ThreadSafeWorkpile::getInstance()->GameQuitted())
	//{
	//	LogHandler::getInstance()->LogToFile("Game quitted.");
	//	return true;
	//}

	switch(ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			switch (ea.getKey())
			{
				case(osgGA::GUIEventAdapter::KEY_Up):
				{
					_lbaNetEngine->StartMove(1);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Down):
				{
					_lbaNetEngine->StartMove(2);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Left):
				{
					_lbaNetEngine->StartMove(3);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Right):
				{
					_lbaNetEngine->StartMove(4);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Space):
				{
					_lbaNetEngine->DoAction();
					return true;
				}
			}

		}
		break;


		case(osgGA::GUIEventAdapter::KEYUP):
		{
			switch (ea.getKey())
			{
				case(osgGA::GUIEventAdapter::KEY_Up):
				{
					_lbaNetEngine->StopMove(1);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Down):
				{
					_lbaNetEngine->StopMove(2);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Left):
				{
					_lbaNetEngine->StopMove(3);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Right):
				{
					_lbaNetEngine->StopMove(4);
					return true;
				}

				case(osgGA::GUIEventAdapter::KEY_Space):
				{
					return true;
				}
			}

		}
		break;

	}

	return false;
}