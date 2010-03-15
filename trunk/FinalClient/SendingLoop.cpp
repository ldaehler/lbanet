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

#include "SendingLoop.h"
#include "SendingWorkpile.h"

/***********************************************************
constructor
***********************************************************/
SendingLoopThread::SendingLoopThread(long cycle_time, boost::shared_ptr<SenderBase> sender)
: m_stopped(false), m_cycle_time(cycle_time), m_sender(sender)
{

}

/***********************************************************
stop the thread
***********************************************************/
void SendingLoopThread::Stop()
{	
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
	m_stopped = true;
	m_monitor.notifyAll();
}

/***********************************************************
running function of the thread
***********************************************************/
void SendingLoopThread::run()
{
	while(true)
	{
		// check if thread needs to be stopped - else wait a few milliseconds
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
			if(!m_stopped)
			{
				IceUtil::Time t = IceUtil::Time::milliSeconds(m_cycle_time);
				m_monitor.timedWait(t);
			}

			if(m_stopped)
			{
				m_stopped = false;
				break;
			}
		}



		//do the work here
		KeyPressed pressed;
		SendingWorkpile::getInstance()->GetKeyPressed(pressed);
		m_sender->SendKey(pressed);
	}
}