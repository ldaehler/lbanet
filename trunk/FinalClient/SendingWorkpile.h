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


#if !defined(__LbaNetModel_1_SendingWorkpile_h)
#define __LbaNetModel_1_SendingWorkpile_h

#include "CommonTypes.h"
#include <boost/thread/mutex.hpp>


/***********************************************************************
 * Module:  SendingWorkpile.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class SendingWorkpile
 ***********************************************************************/
class SendingWorkpile
{
public:


	//! destructor
	~SendingWorkpile();

	// singleton pattern
	static SendingWorkpile * getInstance();



	//! start a move from keyboard input
	void StartMove(int moveDirection);

	//! stop a move from keyboard input
	void StopMove(int moveDirection);

	//! called when action key is pressed
	void PressActionKey();

	//! called when action key is released
	void ReleaseActionKey();

	//! get what key are pressed currently
	void GetKeyPressed(KeyPressed & pressed);


protected:

	//! construtor
	SendingWorkpile(){}

	SendingWorkpile(const SendingWorkpile &);
	const SendingWorkpile & operator=(const SendingWorkpile &);

private:
	static SendingWorkpile *					_singletonInstance;

	boost::mutex			m_mutex;
	KeyPressed				m_keypressed;


};

#endif