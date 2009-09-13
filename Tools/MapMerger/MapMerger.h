#ifndef _LBANET_LBA_MAP_MERGER_H_
#define _LBANET_LBA_MAP_MERGER_H_

#include <string>
#include <vector>

struct mapInfo
{
	std::string name;
	int posX;
	int posY;
	int posZ;
};

class MapMerger
{
public:
	// constructor
	MapMerger(){}

	// destructor
	~MapMerger(){}

	// merge maps using information in the given file
	void Merge(const std::string &InfoFile);

protected:
	// merge a map to the big one
	void MergeMap(const mapInfo & mi);

	// return a brick fo the map
	int & GetBrick(int X, int Y, int Z);

	// return a brick fo the map
	short & GetPhysic(int X, int Y, int Z);

	// return a brick fo the map
	short & GetMaterial(int X, int Y, int Z);

	// write result to file
	void WriteToFile(const std::string &FileName);

private:
	int * _bricks;
	short * _physics;
	short * _materials;

	int _sizeX;
	int _sizeY;
	int _sizeZ;
	std::vector<std::string>	_bricklist;
};




#endif
