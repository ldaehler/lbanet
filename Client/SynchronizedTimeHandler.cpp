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

#include "SynchronizedTimeHandler.h"


SynchronizedTimeHandler* SynchronizedTimeHandler::_singletonInstance = NULL;



// singleton pattern
SynchronizedTimeHandler * SynchronizedTimeHandler::getInstance()
{
    if(!_singletonInstance)
    {
        _singletonInstance = new SynchronizedTimeHandler();
		return _singletonInstance;
    }
    else
    {
		return _singletonInstance;
    }

}


//! initialize and synchronize time with server
void SynchronizedTimeHandler::Initialize(const LbaNet::ClientSessionPrx & session)
{
	std::cout<<"Synchronizing with server, please wait a moment..."<<std::endl;
	std::vector<std::pair<Ice::Long, Ice::Long> > probes;

	for(int i=0; i<20; ++i)
	{
		Ice::Long tsend = IceUtil::Time::now().toMilliSeconds();
		Ice::Long tserver = session->GetTime();
		Ice::Long trecv = IceUtil::Time::now().toMilliSeconds();
		Ice::Long latency = (trecv - tsend) / 2;
		Ice::Long toffset = trecv - (tserver + latency);
		probes.push_back(std::make_pair<Ice::Long, Ice::Long>(latency, toffset));

		Sleep(200);
	}

	std::sort(probes.begin(), probes.end());

	Ice::Long avlatency = 0;
	_offset = 0;
	for(int i=0; i<10; ++i)
	{
		_offset += probes[i].second;
		avlatency += probes[i].first;
	}
	_offset /= 10;
	avlatency /= 10;

	std::cout<<"average latency: "<<avlatency<<" - time offset: "<<_offset<<std::endl;
}

//! get synchronized current time
Ice::Long SynchronizedTimeHandler::GetCurrentTime()
{
	return (IceUtil::Time::now().toMilliSeconds() + _offset);
}


//! get synchronized current time
double SynchronizedTimeHandler::GetCurrentTimeDouble()
{
	return (IceUtil::Time::now().toMilliSecondsDouble() + _offset);
}