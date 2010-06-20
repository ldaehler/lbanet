#include "lba_map_gl.h"
#include "PhysicHandler.h"
#include <sstream>
#include <iostream>


int main(int argc, char** argv)
{
	PhysicHandler phH(NULL, NULL);



	std::vector<std::string> extramaps;
	extramaps.push_back("Maps/Citadel.txt");
	extramaps.push_back("Maps/cliff.txt");
	extramaps.push_back("Maps/lbae_map0.txt");
	extramaps.push_back("Maps/lbae_map1.txt");
	extramaps.push_back("Maps/lbae_map2.txt");
	extramaps.push_back("Maps/lbae_map3.txt");
	extramaps.push_back("Maps/lbae_map4.txt");
	extramaps.push_back("Maps/old_burg_house.txt");
	extramaps.push_back("Maps/Principal.txt");
	extramaps.push_back("Maps/seemap.txt");
	extramaps.push_back("Maps/Tippet_island_water.txt");
	extramaps.push_back("Maps/Tippett_island_village.txt");

	std::vector<bool> extramapstypes;
	extramapstypes.push_back(false);
	extramapstypes.push_back(false);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);
	extramapstypes.push_back(false);
	extramapstypes.push_back(false);
	extramapstypes.push_back(true);
	extramapstypes.push_back(true);

	for(size_t i=0; i<extramaps.size(); ++i)
	{
		std::string physmap = extramaps[i];
		physmap.replace(physmap.find(".txt"), 4, ".phy");

		LBA_MAP_GL mapgl(extramaps[i], &phH);
		phH.SearchFloors();
		phH.SearchWallX();
		phH.SearchWallZ();
		phH.SearchStairs();
		phH.MakeSurroundingPlanes();

		if(extramapstypes[i])
			phH.SearchRoof();

		phH.SavePlanes(physmap);
	}





	std::ifstream maptypeslba1("lba1_maps_type.txt");
	std::vector<bool> _mapType;
	while(!maptypeslba1.eof())
	{
		std::string tmp;
		getline(maptypeslba1, tmp);
		_mapType.push_back(tmp=="interior");
	}



	for(int i=0; i<=133; ++i)
	{
		std::stringstream file1;
		file1<<"Maps/Lba1/map"<<i<<".txt";
		std::stringstream file2;
		file2<<"Maps/Lba1/map"<<i<<".phy";

		LBA_MAP_GL mapgl(file1.str(), &phH);
		phH.SearchFloors();
		phH.SearchWallX();
		phH.SearchWallZ();
		phH.SearchStairs();
		phH.MakeSurroundingPlanes();

		if(_mapType[i])
			phH.SearchRoof();

		phH.SavePlanes(file2.str());
	}

	for(int i=1; i<=148; ++i)
	{
		std::stringstream file1;
		file1<<"Maps/Lba2/map"<<i<<".txt";
		std::stringstream file2;
		file2<<"Maps/Lba2/map"<<i<<".phy";

		LBA_MAP_GL(file1.str(), &phH);
		phH.SearchFloors();
		phH.SearchWallX();
		phH.SearchWallZ();
		phH.SearchStairs();
		phH.SearchRoof();
		phH.MakeSurroundingPlanes();

		phH.SavePlanes(file2.str());
	}

	return 0;
}
