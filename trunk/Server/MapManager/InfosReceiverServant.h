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


#ifndef __INFO_RECEIVER_SERVANT_H_
#define __INFO_RECEIVER_SERVANT_H_

#include <ActorInfo.h>
#include <string>

#include "SharedData.h"

/***********************************************************
  Wrapper class containing the callback function used when a
  message is received in IceStorm
***********************************************************/
class InfosReceiverServant : public LbaNet::ActorsObserver
{
public:

	explicit InfosReceiverServant(SharedData * SD)
		: _SD(SD)
	{}

	// callback function called when a message is received from IceStorm
	virtual void UpdatedInfo(const LbaNet::ActorInfo& asi, const Ice::Current&);

	// callback function called when a message is received from IceStorm
	virtual void Quitted(const std::string& ActorName, const Ice::Current&){}

	// callback function called when a message is received from IceStorm
	virtual void ActivatedActor(const LbaNet::ActorActivationInfo &ai, const Ice::Current&){}

	// callback function called when a message is received from IceStorm
	virtual void SignaledActor(const LbaNet::ActorSignalInfo &ai, const Ice::Current&){}

	// update actor life
	virtual void UpdatedLife(const LbaNet::ActorLifeInfo&, const Ice::Current&){}

private:
	SharedData * _SD;
};

#endif
