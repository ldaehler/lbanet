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


#ifndef _LBANET_SERVER_LOCAL_SENDER_
#define _LBANET_SERVER_LOCAL_SENDER_

#include "CommonTypes.h"
#include "ServerSenderBase.h"

class ReceivingWorkpile;

#include <boost/shared_ptr.hpp>

/***********************************
*	Class used to locally simulate sending data through the network
*************************************/
class ServerLocalSender : public ServerSenderBase
{
public:
	//! constructor
	ServerLocalSender(boost::shared_ptr<ReceivingWorkpile> buffer);

	//! destructor
	~ServerLocalSender(){}

	//! send keys to server
	virtual void SendActorInfo(const ActorInfo & ai);


private:
	boost::shared_ptr<ReceivingWorkpile> _senderbuffer;
};

#endif
