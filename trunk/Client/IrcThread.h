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

#ifndef _LBANET_IRC_THREAD_
#define _LBANET_IRC_THREAD_


#include <IceUtil/Thread.h>

typedef struct irc_session_s	irc_session_t;


/***********************************
*	Ice interface implementation
*************************************/
class IrcThread : public IceUtil::Thread
{
public:
	struct IrcCoInfo
	{
		std::string			Server;
		std::string			Nickname;
		std::string			Channel;
	};


	//! constructor
	IrcThread(const IrcCoInfo & confinfo);

	// running function of the thread
	virtual void run();

	// send message to irc
	void sendMessage(const std::string & Text);


	// used to quit irc
	void Quit();

private:
	irc_session_t *		_session;
	IrcCoInfo			_coInfo;
	long				_wait_time;
};

#endif
