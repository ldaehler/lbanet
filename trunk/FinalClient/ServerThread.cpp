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

#include "ServerThread.h"
#include "ServerReceiveingWorkpile.h"
#include "SynchronizedTimeHandler.h"


/***********************************************************
constructor
***********************************************************/
ServerThread::ServerThread(long cycle_time, boost::shared_ptr<ServerReceivingWorkpile> receiver)
: m_stopped(false), m_cycle_time(cycle_time), m_receiver(receiver)
{

}

/***********************************************************
stop the thread
***********************************************************/
void ServerThread::Stop()
{	
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
	m_stopped = true;
	m_monitor.notifyAll();
}

/***********************************************************
running function of the thread
***********************************************************/
void ServerThread::run()
{
	m_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();

	while(true)
	{
		// check if thread needs to be stopped - else wait a few milliseconds
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
			if(m_stopped)
			{
				m_stopped = false;
				break;
			}
		}



		//do the work here

		// get key pressed since the last time
		std::vector<Sendedinfo> keypressed;
		m_receiver->GetkeyPressed(keypressed);




		// wait a scan cycle time
		unsigned long currenttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		long diff = (long)(currenttime-m_lasttime);
		m_lasttime = currenttime;
		if(diff < m_cycle_time)
		{
			IceUtil::Time t = IceUtil::Time::milliSeconds(diff-m_cycle_time);
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
			m_monitor.timedWait(t);
		}

	}
}