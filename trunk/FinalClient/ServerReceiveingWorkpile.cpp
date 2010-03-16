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

#include "ServerReceiveingWorkpile.h"
#include <boost/thread/locks.hpp>




/***********************************************************
set key pressed by a certain client
***********************************************************/
void ServerReceivingWorkpile::SetKeyPressed(const Sendedinfo & pressed)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_vecPressed.push_back(pressed);
}


/***********************************************************
set key pressed by a certain client
***********************************************************/
void ServerReceivingWorkpile::GetkeyPressed(std::vector<Sendedinfo> & Keyspressed)
{
	boost::mutex::scoped_lock lock(m_mutex);
	Keyspressed.clear();
	Keyspressed.swap(m_vecPressed);
}