#include "globals.h"
#include <map>
#include <set>

class lbrick
{
public:
	int idx;
	int shape;
	int material;
	int material2;

	lbrick()
		: idx(0), shape(1), material(0), material2(0)
	{
	
	}
};


int main(int argc, char** argv)
{
	std::vector<lbrick > bricks(26618);

	{
		for(int i=0; i<134; ++i)
		{
			LBA_MAP mp(i, false);
			//LBA_INFO_BRICK *** info_brick = mp.GetGrid()->GetBricks();

			//for(int i=0;i<64;i++)
			//for(int j=0;j<64;j++)
			//for(int k=0;k<25;k++)
			//{
			//	int idx = info_brick[i][j][k].index_brick;
			//	int mat = (info_brick[i][j][k].material >> 4);
			//	int mat2 = info_brick[i][j][k].material & unsigned char(15);
			//	int sh = info_brick[i][j][k].shape;


			//	if(idx >13 && bricks[idx-14].shape == 1)
			//	{
			//		lbrick l;
			//		l.idx = idx-14;
			//		l.shape = sh;
			//		l.material = mat;
			//		l.material2 = mat2;

			//		bricks[l.idx] = l;
			//	}
			//}
		}
	}

	{
		for(int i=1; i<149; ++i)
		{
			LBA_MAP mp(i, true);
			//LBA_INFO_BRICK *** info_brick = mp.GetGrid()->GetBricks();

			//for(int i=0;i<64;i++)
			//for(int j=0;j<64;j++)
			//for(int k=0;k<25;k++)
			//{
			//	int idx = info_brick[i][j][k].index_brick;
			//	int mat = (info_brick[i][j][k].material >> 4);
			//	int mat2 = info_brick[i][j][k].material & unsigned char(15);
			//	int sh = info_brick[i][j][k].shape;


			//	if(idx >13)
			//	{
			//		lbrick l;
			//		l.idx = idx-14;
			//		l.shape = sh;
			//		l.material = mat;
			//		l.material2 = mat2;



			//		bricks[l.idx] = l;
			//	}
			//}
		}
	}


	//std::ofstream file("lba_brick_info.txt");
	//std::vector<lbrick >::iterator it = bricks.begin();
	//std::vector<lbrick >::iterator end = bricks.end();

	//for(int cc=0; it != end; ++it, ++cc)
	//{
	//	int nmb = cc;
	//	if(nmb > 8714)
	//		nmb -= 8715;

	//	char buf[255];
	//	itoa(nmb, buf, 10);
	//	std::string path;
	//	if(cc <= 8714)
	//	{
	//		path = std::string("Bricks/Lba1/") + std::string(buf) + std::string(".png");
	//	}
	//	else
	//	{
	//		path = std::string("Bricks/Lba2/") + std::string(buf) + std::string(".png");
	//	}

	//	file<<cc<<" "<<path<<" "<<it->shape<<std::endl;
	//}




	return 0;
}














