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


#ifndef __LBANET_SIGNALER_SERVER_H__
#define __LBANET_SIGNALER_SERVER_H__

#include "SignalerBase.h"
#include "MapHandlerThread.h"
/*
************************************************************************************************************************
*                                                  class ServerSignaler
************************************************************************************************************************
*/
class ServerSignaler : public SignalerBase
{

public:
	//! constructor
	ServerSignaler(MapHandlerThread * MH)
		: _MH(MH)
	{}

	//! destructor
	virtual ~ServerSignaler(){}

	//! send signal
	virtual void SendSignal(long actorid, long signal, const std::vector<long> &targets, 
																	bool broadcast = false)
	{
		LbaNet::ActorSignalInfo aif;
		aif.ActorId = actorid;
		aif.SignalId =  signal;
		for(size_t i=0; i<targets.size(); ++i)
			aif.Targets.push_back(targets[i]);
		_MH->SignalActor(aif, broadcast);
	}

private:
	MapHandlerThread * _MH;
};



#endif