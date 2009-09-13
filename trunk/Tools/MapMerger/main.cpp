#include "MapMerger.h"
#include <iostream>

int main(int argc, char** argv)
{
	std::cout<<"Please give the path of the info file:"<<std::endl;
	std::string file;
	std::cin>>file;

	MapMerger mm;
	mm.Merge(file);

	return 0;
}














