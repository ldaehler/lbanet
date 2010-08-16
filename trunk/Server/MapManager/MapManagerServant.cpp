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
#include "MapInfoXmlReader.h"

//! constructor
MapManagerServant::MapManagerServant(const Ice::CommunicatorPtr& communicator,
									 const Ice::ObjectAdapterPtr & adapter)
: _communicator(communicator), _adapter(adapter)
{
	std::vector<std::string> worlds;
	worlds.push_back("GiantCitadelWorld");
	worlds.push_back("GiantPrincipalWorld");
	worlds.push_back("Lba1Expanded");
	worlds.push_back("Lba1OriginalWorld");

	for(size_t i=0; i<worlds.size(); ++i)
	{


		WorldInfo wi;
		MapInfoXmlReader::LoadWorld("Data/" + worlds[i] + ".xml", wi);
		std::map<std::string, MapInfo>::iterator itmap = wi.Maps.begin();
		for(;itmap != wi.Maps.end(); ++itmap)
		{
			std::string worldname = wi.Name;
			std::string mapS = itmap->first;
			std::string localfile = itmap->second.Files["LocalActors"];
			std::string externalfile = itmap->second.Files["ExternalActors"];

			std::map<long, Actor *>	tmpactors;

			std::map<long, SpriteInfo> tmp;
			std::map<long, ModelInfo> tmp2;
			MapInfoXmlReader::LoadActors("Data/" + localfile, tmp, tmp, tmp2, tmpactors, NULL, 2.2f, NULL, NULL, NULL);

			std::map<long, Actor *> map;
			MapInfoXmlReader::LoadActors("Data/" + externalfile, tmp, tmp, tmp2, map, NULL, 2.2f, NULL, NULL, NULL);
			std::map<long, Actor *>::iterator it = map.begin();
			std::map<long, Actor *>::iterator end = map.end();
			for(;it != end; ++it)
				tmpactors[it->first] = it->second;

			_actors_maps[worldname + "_" + mapS + "_Actors"] = tmpactors;
		}
	}
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
											Ice::Long PlayerId, const ActorLifeInfo &ali, 
											const LbaNet::ClientSessionPrx & callback,
											const Ice::Current&)
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
			MH = new MapHandler(_communicator, _adapter, mapName, this, _actors_maps[mapName]);
			_running_maps[mapName] = MH;
		}
	}

	MH->Join(PlayerId, ali, callback);
	return MH->GetMapProxy();
}

//! a player leave a map
ActorLifeInfo MapManagerServant::LeaveMap(const std::string& mapName, Ice::Long PlayerId, const Ice::Current&)
{
	Lock sync(*this);
	std::map<std::string, MapHandler *>::iterator it = _running_maps.find(mapName);
	if(it != _running_maps.end())
		return it->second->Leave(PlayerId);
	else
	{
		return ActorLifeInfo();
	}
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

