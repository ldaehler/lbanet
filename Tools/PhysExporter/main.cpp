#include "lba_map_gl.h"
#include "PhysicHandler.h"
#include <sstream>

int main(int argc, char** argv)
{
	std::ifstream maptypeslba1("lba1_maps_type.txt");
	std::vector<bool> _mapType;
	while(!maptypeslba1.eof())
	{
		std::string tmp;
		getline(maptypeslba1, tmp);
		_mapType.push_back(tmp=="interior");
	}

	PhysicHandler phH(NULL, NULL);

	for(int i=0; i<=133; ++i)
	{
		std::stringstream file1;
		file1<<"Maps/Lba1/map"<<i<<".txt";
		std::stringstream file2;
		file2<<"Maps/Lba1/map"<<i<<".phy";

		LBA_MAP_GL(file1.str(), &phH);
		phH.SearchFloors();
		phH.SearchWallX();
		phH.SearchWallZ();
		phH.SearchStairs();

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

		phH.SavePlanes(file2.str());
	}

	return 0;
}
