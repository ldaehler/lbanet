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


#include "MapManagerServant.h"


//! constructor
MapManagerServant::MapManagerServant(const Ice::CommunicatorPtr& communicator,
									 const Ice::ObjectAdapterPtr & adapter)
: _communicator(communicator), _adapter(adapter)
{

}

//! destructor
MapManagerServant::~MapManagerServant()
{
	std::map<std::string, MapHandler *>::iterator it = _running_maps.begin();
	std::map<std::string, MapHandler *>::iterator end = _running_maps.end();

	for(; it != end; ++it)
		delete it->second;
}

//! a player join a map
LbaNet::MapObserverPrx MapManagerServant::JoinMap(const std::string& mapName,
										Ice::Long PlayerId, const Ice::Current&)
{
	// clean old threads
	CleanThreads();

	MapHandler * MH = NULL;
	{
		Lock sync(*this);
		std::map<std::string, MapHandler *>::iterator it = _running_maps.find(mapName);
		if(it != _running_maps.end())
		{
			MH = it->second;
		}
		else
		{
			MH = new MapHandler(_communicator, _adapter, mapName, this);
			_running_maps[mapName] = MH;
		}
	}

	MH->Join(PlayerId);
	return MH->GetMapProxy();
}

//! a player leave a map
void MapManagerServant::LeaveMap(const std::string& mapName, Ice::Long PlayerId, const Ice::Current&)
{
	Lock sync(*this);
	std::map<std::string, MapHandler *>::iterator it = _running_maps.find(mapName);
	if(it != _running_maps.end())
		it->second->Leave(PlayerId);
}


//! stop the thread for map mapName
void MapManagerServant::StopThread(const std::string& mapName)
{
	Lock sync(*this);
	_tostop.push_back(mapName);
}


//! clean thread to be stopped
void MapManagerServant::CleanThreads()
{
	std::vector<std::string> tostoptmp;
	{
		Lock sync(*this);
		tostoptmp.swap(_tostop);
	}


	for(size_t i=0; i<tostoptmp.size(); ++i)
	{
		std::map<std::string, MapHandler *>::iterator it = _running_maps.find(tostoptmp[i]);
		if(it != _running_maps.end())
		{
			delete it->second;
			_running_maps.erase(it);
		}
	}
}

