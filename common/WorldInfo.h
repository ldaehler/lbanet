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


#ifndef __LBA_NET_MAP_WORLD_INFO_H__
#define __LBA_NET_MAP_WORLD_INFO_H__

#include <string>
#include <map>
#include <vector>

// contain information about a light element
struct LighInfo
{
	// light name
	std::string		Name;

	// light type: e.g spot/main
	std::string		Type;

	// light position
	int				PosX;
	int				PosY;
	int				PosZ;

	// light direction
	int				DirX;
	int				DirY;
	int				DirZ;
};


// contain information about a spawning point
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


// contain information about a exit area
struct ExitInfo
{
	// name
	std::string		Name;

	// top left corner
	float			TopRightX;
	float			TopRightY;
	float			TopRightZ;

	// bottom right corner
	float			BottomLeftX;
	float			BottomLeftY;
	float			BottomLeftZ;

	// arrival point of the exit
	std::string		NewMap;
	std::string		Spawning;
};


// contain information about a teleport place
struct TPInfo
{
	// name
	std::string		Name;

	// arrival point
	std::string		NewMap;
	std::string		Spawning;
};

// contains information about an LBA map
struct MapInfo
{
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

	// lights
	std::map<std::string, LighInfo>	Lights;


	// spawning points
	std::map<std::string, SpawningInfo>	Spawnings;

	// exit points
	std::map<std::string, ExitInfo>	Exits;

};

// contains information about an LBA world
struct WorldInfo
{
	// world name
	std::string		Name;

	// map description
	std::string		Description;

	// map used at arrival in the world
	std::string		FirstMap;

	// spawn area used at arrival in the world
	std::string		FirstSpawning;

	// description of the maps constituing the world
	std::map<std::string, MapInfo>	Maps;

	// teleport places
	std::map<std::string, TPInfo>	Teleports;

	// files to be loaded
	std::map<std::string, std::string>	Files;

};



// quad info
struct QuadImageInfo
{
	// top left corner
	float BottomLeftCornerX;
	float BottomLeftCornerY;
	float BottomLeftCornerZ;

	float BottomRightCornerX;
	float BottomRightCornerY;
	float BottomRightCornerZ;

	// bottom right corner
	float TopRightCornerX;
	float TopRightCornerY;
	float TopRightCornerZ;

	float TopLeftCornerX;
	float TopLeftCornerY;
	float TopLeftCornerZ;

	// flag saying if we use full image
	bool UseFullImage;

	// top left texture coordinate
	int TopLeftTextcoordX;
	int TopLeftTextcoordY;

	// top right texture coordinate
	int TopRightTextcoordX;
	int TopRightTextcoordY;

	// bottom left texture coordinate
	int BottomLeftTextcoordX;
	int BottomLeftTextcoordY;

	// bottom right texture coordinate
	int BottomRightTextcoordX;
	int BottomRightTextcoordY;
};

//sprite info
struct SpriteInfo
{
	long id;
	std::string filename;
	std::vector<QuadImageInfo> quadsInfo;
};


//model info
struct ModelInfo
{
	long id;
	std::string filename;
	float ScaleX;
	float ScaleY;
	float ScaleZ;
};


struct ItemInfo
{
	long id;
	std::string filename;
	int type;
	int valueA;
	int Max;
	std::string Description;
	int Effect;
	bool Ephemere;
};


struct ItemGroupElement
{
	long id;
	int number;
	float probability;
};

struct ItemGroup
{
	std::vector<ItemGroupElement> groupelements;
	double	lastSpawningTime;
	double	RespawningTime;
	long	currpicked;
};


#endif
