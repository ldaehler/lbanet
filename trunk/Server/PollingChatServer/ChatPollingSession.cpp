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


#include "ChatPollingSession.h"
#include "ChatReceiverServant.h"
#include <IceUtil/UUID.h>


/***********************************************************
constructor
***********************************************************/
ChatPollingSessionServant::ChatPollingSessionServant(const std::string username,
													const LbaNet::ChatRoomObserverPrx & chatP) 
:    _name(username),
    _timestamp(IceUtil::Time::now(IceUtil::Time::Monotonic)),
    _destroy(false), _chatP(chatP)
{
	_chatP->Message("info", "#joined " +_name);
}

/***********************************************************
destructor
***********************************************************/
ChatPollingSessionServant::~ChatPollingSessionServant()
{
}

/***********************************************************
get user online
***********************************************************/
LbaNet::UserSeq ChatPollingSessionServant::getInitialUsers(const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
}

/***********************************************************
get update since last time asked
***********************************************************/
LbaNet::ChatRoomEventSeq ChatPollingSessionServant::getUpdates(const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);

	_timestamp = IceUtil::Time::now(IceUtil::Time::Monotonic);

	LbaNet::ChatRoomEventSeq seq;
	seq.swap(_events);
	return seq;
}

/***********************************************************
send message
***********************************************************/
Ice::Long ChatPollingSessionServant::send(const std::string& message, const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);

	try
	{
		_chatP->Message(_name, message);
	}
	catch(Ice::Exception &)	{}

	return IceUtil::Time::now().toMilliSeconds();
}

/***********************************************************
destroy session
***********************************************************/
void ChatPollingSessionServant::destroy(const Ice::Current& c)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);

	_destroy = true;

    try
    {
        c.adapter->remove(c.id);
		_chatP->Message("info", "#left " +_name);
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        // No need to clean up, the server is shutting down.
    }
}

/***********************************************************
get timestamp since last action
***********************************************************/
IceUtil::Time ChatPollingSessionServant::timestamp() const
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);

	return _timestamp;
}



/***********************************************************
add new chat event
***********************************************************/
void ChatPollingSessionServant::AddEvent(const LbaNet::ChatRoomEventPtr & evt)
{
    IceUtil::Mutex::Lock sync(_mutex);
	_events.push_back(evt);
}