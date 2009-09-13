#include "globals.h"
#include <map>
#include <set>



int main(int argc, char** argv)
{
	std::cout<<"Please give the path of the gr file you want to convert:"<<std::endl;
	std::string file;
	std::cin>>file;
	int layout = 0;
	bool lba2 = true;

	std::string extension = file.substr(file.size() - 3);
	if(extension == "gr1")
	{
		lba2 = false;
		std::cout<<"Please give the layout you used to create the gr1 file:"<<std::endl;
		std::cout<<"0: Prisons, factories, industrial buildings"<<std::endl;
		std::cout<<"1: Islands"<<std::endl;
		std::cout<<"5: Houses"<<std::endl;
		std::cout<<"7: Yellow brick buildings (e.g. Library)"<<std::endl;
		std::cout<<"8: Caves"<<std::endl;
		std::cout<<"9: Snow scenes"<<std::endl;
		std::cout<<"12: Military islands"<<std::endl;
		std::cout<<"17: Ruins"<<std::endl;
		std::cout<<"34: Sewers"<<std::endl;
		std::cout<<"36: Desert scenes"<<std::endl;
		std::cout<<"59: Ruined military islands (e.g. Rebellion)"<<std::endl;
		std::cout<<"93: Ruined scenes (e.g. Fortress)"<<std::endl;
		std::cout<<"106: Working areas (e.g. Polar Island)"<<std::endl;
		std::cout<<"116: Decorated scenes"<<std::endl<<std::endl;

		std::cin>>layout;

	}
	std::cout<<"Generating"<<std::endl;

	LBA_MAP mp(lba2, file, layout);

	return 0;
}














