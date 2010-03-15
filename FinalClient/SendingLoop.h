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


#ifndef _LBANET_SENDING_LOOP_
#define _LBANET_SENDING_LOOP_

#include "SenderBase.h"
#include "SendingWorkpile.h"

#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>

#include <boost/shared_ptr.hpp>

/***********************************
*	Thread used to send information to the server
*************************************/
class SendingLoopThread : public IceUtil::Thread
{
public:
	//! constructor
	SendingLoopThread(long cycle_time, boost::shared_ptr<SenderBase> sender);

	// running function of the thread
	virtual void run();

	//! stop the thread
	void Stop();

private:
	IceUtil::Monitor<IceUtil::Mutex>	m_monitor;
	bool								m_stopped;
	long								m_cycle_time;

	boost::shared_ptr<SenderBase>		m_sender;
};

#endif
