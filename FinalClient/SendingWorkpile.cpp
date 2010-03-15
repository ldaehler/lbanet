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

#include "SendingWorkpile.h"
#include <boost/thread/locks.hpp>

SendingWorkpile* SendingWorkpile::_singletonInstance = NULL;


/***********************************************************
	Destructor
***********************************************************/
SendingWorkpile::~SendingWorkpile()
{

}




/***********************************************************
singleton pattern
***********************************************************/
SendingWorkpile * SendingWorkpile::getInstance()
{
    if(!_singletonInstance)
    {
        _singletonInstance = new SendingWorkpile();
		return _singletonInstance;
    }
    else
    {
		return _singletonInstance;
    }

}


/***********************************************************
start a move from keyboard input
***********************************************************/
void SendingWorkpile::StartMove(int moveDirection)
{
	boost::mutex::scoped_lock lock(m_mutex);

	switch(moveDirection)
	{
		case 1:
			m_keypressed.Up = true;
		break;
		case 2:
			m_keypressed.Down = true;
		break;
		case 3:
			m_keypressed.Left = true;
		break;
		case 4:
			m_keypressed.Right = true;
		break;
	}
}

/***********************************************************
stop a move from keyboard input
***********************************************************/
void SendingWorkpile::StopMove(int moveDirection)
{
	boost::mutex::scoped_lock lock(m_mutex);

	switch(moveDirection)
	{
		case 1:
			m_keypressed.Up = false;
		break;
		case 2:
			m_keypressed.Down = false;
		break;
		case 3:
			m_keypressed.Left = false;
		break;
		case 4:
			m_keypressed.Right = false;
		break;
	}
}


/***********************************************************
called when action key is pressed
***********************************************************/
void SendingWorkpile::PressActionKey()
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_keypressed.Action = true;
}


/***********************************************************
called when action key is released
***********************************************************/
void SendingWorkpile::ReleaseActionKey()
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_keypressed.Action = false;
}


/***********************************************************
get what key are pressed currently
***********************************************************/
void SendingWorkpile::GetKeyPressed(KeyPressed & pressed)
{
	boost::mutex::scoped_lock lock(m_mutex);

	pressed = m_keypressed;
}
