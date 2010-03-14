#include "lba_map_gl.h"
#include "PhysicHandler.h"
#include <sstream>

int main(int argc, char** argv)
{
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
		phH.SavePlanes(file2.str());
	}

	return 0;
}
