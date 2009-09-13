#include "MapMerger.h"
#include <fstream>
#include <map>


/***********************************************************
merge maps using information in the given file
***********************************************************/
void MapMerger::Merge(const std::string &InfoFile)
{
	std::ifstream file(InfoFile.c_str());

	file >> _sizeX;
	file >> _sizeY;
	file >> _sizeZ;

	std::string newmapname;
	file >> newmapname;

	std::vector<mapInfo> maps;
	while(!file.eof())
	{
		mapInfo mp;
		file >> mp.name;
		file >> mp.posX;
		file >> mp.posY;
		file >> mp.posZ;
		maps.push_back(mp);
	}

	_bricks = new int[_sizeX*_sizeY*_sizeZ];
	memset(_bricks, 0, _sizeX*_sizeY*_sizeZ*sizeof(int));

	_physics = new short[_sizeX*_sizeY*_sizeZ];
	memset(_physics, 0, _sizeX*_sizeY*_sizeZ*sizeof(short));

	_materials = new short[_sizeX*_sizeY*_sizeZ];
	memset(_materials, 0, _sizeX*_sizeY*_sizeZ*sizeof(short));


	for(size_t i=0; i<maps.size(); ++i)
	{
		if(maps[i].name != "")
			MergeMap(maps[i]);
	}

	WriteToFile(newmapname);
}


/***********************************************************
merge a map to the big one
***********************************************************/
void MapMerger::MergeMap(const mapInfo & mi)
{
	std::ifstream file(mi.name.c_str());
	int sizeX, sizeY, sizeZ;
	file >> sizeX;
	file >> sizeY;
	file >> sizeZ;

	int nbbricks;
	file >> nbbricks;
	std::map<int, int> mapidx;

	// calculate new index for bricks
	for(int i=0; i<nbbricks; ++i)
	{
		std::string tmpbr;
		file >> tmpbr;

		size_t j=0;
		for(; j<_bricklist.size(); ++j)
			if(_bricklist[j] == tmpbr)
				break;		

		if(j == _bricklist.size())
			_bricklist.push_back(tmpbr);

		mapidx[i] = j;
	}

	// merge map bricks
	for(int y=0;y<sizeY;y++)
	{
		// get graphic info
		for(int x=0;x<sizeX;x++)
		{
			for(int z=0;z<sizeZ;z++)
			{
				int tmp;
				file >> tmp;
				if(tmp != 0)
					GetBrick(x+mi.posX, y+mi.posY, z+mi.posZ) = mapidx[tmp-1]+1;
				//else
				//	GetBrick(x+mi.posX, y+mi.posY, z+mi.posZ) = 0;
			}
		}

		// get physic info
		for(int x=0;x<sizeX;x++)
		{
			for(int z=0;z<sizeZ;z++)
			{
				short tmp;
				file >> tmp;
				if(tmp != 0)
					GetPhysic(x+mi.posX, y+mi.posY, z+mi.posZ) = tmp;
			}
		}

		// get material info
		for(int x=0;x<sizeX;x++)
		{
			for(int z=0;z<sizeZ;z++)
			{
				short tmp;
				file >> tmp;
				if(tmp != 0)
					GetMaterial(x+mi.posX, y+mi.posY, z+mi.posZ) = tmp;
			}
		}
	}
}


/***********************************************************
return a brick fo the map
***********************************************************/
int & MapMerger::GetBrick(int X, int Y, int Z)
{
	return _bricks[Y*_sizeX*_sizeZ+X*_sizeZ+Z];
}

// return a brick fo the map
short & MapMerger::GetPhysic(int X, int Y, int Z)
{
	return _physics[Y*_sizeX*_sizeZ+X*_sizeZ+Z];
}

// return a brick fo the map
short & MapMerger::GetMaterial(int X, int Y, int Z)
{
	return _materials[Y*_sizeX*_sizeZ+X*_sizeZ+Z];
}



/***********************************************************
write result to file
***********************************************************/
void MapMerger::WriteToFile(const std::string &FileName)
{
	int * ptr = _bricks;
	short * ptrP = _physics;
	short * ptrM = _materials;

	std::ofstream file(FileName.c_str());
	file <<_sizeX << " " <<_sizeY << " " <<_sizeZ << std::endl;
	file <<_bricklist.size() << std::endl << std::endl;

	for(size_t j=0; j<_bricklist.size(); ++j)
		file <<_bricklist[j] << std::endl;

	file << std::endl<< std::endl;

	for(int y=0;y<_sizeY;y++)
	{
		// write graphic
		for(int x=0;x<_sizeX;x++)
		{
			for(int z=0;z<_sizeZ;z++)
			{
				file<<*ptr<<" ";
				++ptr;
			}
			file<< std::endl;
		}
		file<< std::endl;

		// write physic
		for(int x=0;x<_sizeX;x++)
		{
			for(int z=0;z<_sizeZ;z++)
			{
				file<<*ptrP<<" ";
				++ptrP;
			}
			file<< std::endl;
		}
		file<< std::endl;

		// write material
		for(int x=0;x<_sizeX;x++)
		{
			for(int z=0;z<_sizeZ;z++)
			{
				file<<*ptrM<<" ";
				++ptrM;
			}
			file<< std::endl;
		}
		file<< std::endl;
	}
}
