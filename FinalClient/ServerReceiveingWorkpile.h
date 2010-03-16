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


#if !defined(__LbaNetModel_1_ServerReceivingWorkpile_h)
#define __LbaNetModel_1_ServerReceivingWorkpile_h

#include "CommonTypes.h"
#include <boost/thread/mutex.hpp>
#include <vector>

/***********************************************************************
 * Module:  ServerReceivingWorkpile.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class ServerReceivingWorkpile
 ***********************************************************************/
class ServerReceivingWorkpile
{
public:
	//! construtor
	ServerReceivingWorkpile(){}

	//! destructor
	~ServerReceivingWorkpile(){}


	//! set key pressed by a certain client
	void SetKeyPressed(const Sendedinfo & pressed);

	//! get key pressed by client since the last time
	void GetkeyPressed(std::vector<Sendedinfo> & Keyspressed);

private:
	boost::mutex				m_mutex;
	std::vector<Sendedinfo>		m_vecPressed;


};

#endif