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
#include "MapHandler.h"
/*
************************************************************************************************************************
*                                                  class ServerSignaler
************************************************************************************************************************
*/
class ServerSignaler : public SignalerBase
{

public:
	//! constructor
	ServerSignaler(MapHandler * MH)
		: _MH(MH)
	{}

	//! destructor
	virtual ~ServerSignaler(){}

	//! send signal
	virtual void SendSignal(long signal, const std::vector<long> &targets)
	{
		_MH->SendSignal(signal, targets);
	}

private:
	MapHandler * _MH;
};



#endif