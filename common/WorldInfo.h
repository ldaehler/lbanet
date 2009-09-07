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


#endif
