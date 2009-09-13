#ifndef _LBANET_LBA_MAP_H_
#define _LBANET_LBA_MAP_H_

#include <string>

class LBA_PACK
{
public :

	// constructors
	LBA_PACK(){}
	LBA_PACK(const std::string & file);

	// destructor
    ~LBA_PACK();

	// class members
    unsigned char *data;
};



class LBA_ENTRY
{
public :

    //constructor
    LBA_ENTRY(LBA_PACK *pack,int entry);

	// destructor
    ~LBA_ENTRY();

	// class members
    long datalenght;
    unsigned char *data;
};




class LBA_PALET
{
public :

	// constructor
    LBA_PALET(LBA_PACK *pack);

	// destructor
    ~LBA_PALET(){}

	// class members
    unsigned char couleur[256][3];
};



class LBA_BRICK
{
public:

	// constructor
    LBA_BRICK(LBA_PACK *pack,int n);

	// destructor
    ~LBA_BRICK(){}

	// class members
    int lenght;
    int height;
    unsigned char pixel[38][48];
};





struct LBA_INFO_BRICK
{
	unsigned int index_brick;
	unsigned char shape;
	unsigned char material;
	short sound;
};



class LBA_OBJECT
{
public:

	// constructor
    LBA_OBJECT(int X,int Y,int Z);

	// destructor
    ~LBA_OBJECT();

	// class members
    unsigned char taille_X;
	unsigned char taille_Y;
	unsigned char taille_Z;
    LBA_INFO_BRICK *info_brick;
};



class LBA_LAYOUT
{
public:

	// constructor
    LBA_LAYOUT(LBA_PACK *pack,int n);

	// destructor
	~LBA_LAYOUT();

	// class members
    int number_objects;
    LBA_OBJECT **object;

};



class LBA_GRID
{
public:
	// constructor
    LBA_GRID(LBA_PACK *pack_grid,LBA_PACK *pack_layout,int n,bool LBA2);

	// destructor
    ~LBA_GRID();

	// class members
    LBA_INFO_BRICK ***info_brick;


	LBA_INFO_BRICK *** GetBricks()
	{ return info_brick; }
};




class LBA_MAP
{
public:
	// constructor
    LBA_MAP(int n, bool LBA2);

	// destructor
    ~LBA_MAP();

	// class members
    LBA_PALET *palet;
    LBA_GRID *grid;
    int number_brick;


	LBA_GRID * GetGrid()
	{return grid;}
};




#endif
