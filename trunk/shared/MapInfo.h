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


#ifndef __LBA_NET_MAP_INFO_H__
#define __LBA_NET_MAP_INFO_H__

#include <map>
#include <string>
#include "ObjectsDescription.h"


/***********************************************************************
contain world description
 ***********************************************************************/
struct WorldDesc
{
	std::string WorldName;
	std::string Description;
	std::string FileName;
};


/***********************************************************************
contain information about a teleport place
 ***********************************************************************/
struct TPInfo
{
	// name
	std::string		Name;

	// arrival point
	std::string		NewMap;
	std::string		Spawning;
};


/***********************************************************************
contain information about a spawning point
 ***********************************************************************/
struct SpawningInfo
{
	// name
	std::string		Name;

	// position
	float			PosX;
	float			PosY;
	float			PosZ;

	// rotation at arrival in degree
	int				Rotation;
};

/***********************************************************************
contains information about maps
 ***********************************************************************/
class MapInfo
{
public:

	// map name
	std::string		Name;

	// map type: e.g interior/exterior
	std::string		Type;

	// map description
	std::string		Description;

	// path to the music file to be played
	std::string		Music;

	// number of time the music should be played
	int				MusicLoop;

	// files to be loaded
	std::map<std::string, std::string>	Files;

	// spawning points
	std::map<std::string, SpawningInfo>	Spawnings;

	// actors
	std::map<long, ObjectInfo>	Actors;
};



/***********************************************************************
contains information about where the new player should start in the world
 ***********************************************************************/
class WorldStartingInfo
{
public:
	// map used at arrival in the world
	std::string		FirstMap;

	// spawn area used at arrival in the world
	std::string		FirstSpawning;
};


/***********************************************************************
contains information about an LBA world
 ***********************************************************************/
class WorldInfo
{
public:

	// world name
	std::string		Name;

	//world starting info
	WorldStartingInfo StartInfo;

	// map description
	std::string		Description;

	// description of the maps constituing the world
	std::map<std::string, MapInfo>	Maps;

	// teleport places
	std::map<std::string, TPInfo> Teleports;

	// files to be loaded
	std::map<std::string, std::string>	Files;
};


#endif
