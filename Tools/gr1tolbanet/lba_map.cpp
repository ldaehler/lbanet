#include "lba_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "DataFileHandler.h"
#include <sstream>
#include <vector>
#include <algorithm>

/***********************************************************
constructor
***********************************************************/
LBA_PACK::LBA_PACK(const std::string & file)
{
	datalenght=0;
	FILE *f=fopen(file.c_str(),"rb");
	fseek(f, 0, SEEK_END);
	datalenght = ftell(f);
	data=new unsigned char[datalenght];
	rewind(f);
	fread (data, 1, datalenght, f);
	fclose (f);
}



/***********************************************************
destructor
***********************************************************/
LBA_PACK::~LBA_PACK()
{
    delete data;
}





/***********************************************************
constructor
***********************************************************/
LBA_ENTRY::LBA_ENTRY(LBA_PACK *pack,int entry)
{
    int i=0;

    long compressed_size=0;
    int compression_type=0;
    long pack_offset=0;

	//std::cout<<entry<<std::endl;
    pack_offset=entry*4;
    pack_offset=(long)(pack->data[pack_offset]+pack->data[pack_offset+1]*256+pack->data[pack_offset+2]*65536+pack->data[pack_offset+3]*16777216);
    datalenght=(long)(pack->data[pack_offset]+pack->data[pack_offset+1]*256+pack->data[pack_offset+2]*65536+pack->data[pack_offset+3]*16777216);
    pack_offset+=4;
    compressed_size=(long)(pack->data[pack_offset]+pack->data[pack_offset+1]*256+pack->data[pack_offset+2]*65536+pack->data[pack_offset+3]*16777216);
    pack_offset+=4;
    compression_type=(long)(pack->data[pack_offset]+pack->data[pack_offset+1]*256);
    pack_offset+=2;
    data=new unsigned char[datalenght];


    if(compression_type>0)
    {
        int e=0;
        int e2=0;
        unsigned char b=0,d=0,f=0,g=0;
        unsigned char c1=0,c2=0;
        int data_offset=0;
            do
            {
                b=pack->data[pack_offset++];
                for(d=0;d<8;d++)
                {

                        if((b & (1<<d))!=0)
                        {
                            data[data_offset]=pack->data[pack_offset];
                            pack_offset++;
                            data_offset++;
                        }
                        else
                        {
                            c1=pack->data[pack_offset++];
                            c2=pack->data[pack_offset++];
                            e=(c1*256+c2)%0x10000;
                            f=(((e>>8)&0x000f)+compression_type+1)%0x10000;
                            e2=(((e<<4)&0x0ff0)+((e>>12)&0x000f))%0x10000;
                            for(g=0;g<f;g++)
                            {
                                data[data_offset]=data[data_offset+1-e2-2];
                                data_offset++;
                            }

                        }

                        if(data_offset>=datalenght)break;
                }
            }
            while(data_offset<datalenght);
    }
    else
    {
        for(i=0;i<datalenght;i++)
        data[i]=pack->data[pack_offset++];
    }
}



/***********************************************************
destructor
***********************************************************/
LBA_ENTRY::~LBA_ENTRY()
{
  delete data;
}

LBA_ENTRY::LBA_ENTRY(unsigned char *dt, int lenght)
{
    data=new unsigned char[lenght];
	datalenght = lenght;
	memcpy(data, dt, lenght);

}


/***********************************************************
constructor
***********************************************************/
LBA_PALET::LBA_PALET(LBA_PACK *pack)
{
    int i=0;
    int offset_data=0;

	//std::cout<<"LBA_PALET"<<std::endl;
    LBA_ENTRY *entry=new LBA_ENTRY(pack,0);

    for(i=0;i<256;i++)
    {
        couleur[i][0]=entry->data[offset_data++]; //rouge
        couleur[i][1]=entry->data[offset_data++]; //vert
        couleur[i][2]=entry->data[offset_data++]; //bleu
    }
    delete entry;
}





/***********************************************************
constructor
***********************************************************/
LBA_BRICK::LBA_BRICK(LBA_PACK *pack,int n)
{
    int data_offset=0;
    int i=0,j=0,k=0;
    unsigned char c=0;
    int offset=0;

	//std::cout<<"LBA_BRICK"<<std::endl;
    LBA_ENTRY *entry=new LBA_ENTRY(pack,n);
    lenght=48;
    height=38;
    unsigned char brick_width=entry->data[data_offset++];
    unsigned char brick_height=entry->data[data_offset++];
    unsigned char brick_offsetx=entry->data[data_offset++];
    unsigned char brick_offsety=entry->data[data_offset++];
    unsigned char brick_nombre_subline=0;
    unsigned char brick_type_subline=0;
    unsigned char brick_lenght_subline=0;
    for(i=0;i<38;i++)
    for(j=0;j<48;j++)
    {
        pixel[i][j]=0;
    }
    for(i=0;i<brick_height;i++)
    {
        c=entry->data[data_offset++];
        brick_nombre_subline=c;
        offset=0;

        for(j=0;j<brick_nombre_subline;j++)
        {

            c=entry->data[data_offset++];
            brick_type_subline=c/64;
            brick_lenght_subline=c*4;
            brick_lenght_subline/=4;
            brick_lenght_subline=brick_lenght_subline+1;
            if(brick_type_subline==0)
            {
                offset+=brick_lenght_subline;
            }
            if(brick_type_subline==1)
            {
                for(k=0;k<brick_lenght_subline;k++)
                {
                    c=entry->data[data_offset++];
                    pixel[i+brick_offsety][brick_offsetx+offset]=c;
                    offset++;
                }


            }
            if(brick_type_subline==3)
            {
                for(k=0;k<brick_lenght_subline;k++)
                {
                    c=entry->data[data_offset++];
                    pixel[i+brick_offsety][brick_offsetx+offset]=c;
                    offset++;
                }
            }

            if(brick_type_subline==2)
            {
                c=entry->data[data_offset++];
                for(k=0;k<brick_lenght_subline;k++)
                {
                    pixel[i+brick_offsety][brick_offsetx+offset]=c;
                    offset++;
                }
            }
        }
    }
    delete entry;
}







/***********************************************************
constructor
***********************************************************/
LBA_OBJECT::LBA_OBJECT(int X,int Y,int Z)
{
    taille_X=X;
    taille_Y=Y;
    taille_Z=Z;
    info_brick=new LBA_INFO_BRICK[X*Y*Z];
}


/***********************************************************
destructor
***********************************************************/
LBA_OBJECT::~LBA_OBJECT()
{
    delete info_brick;
}




/***********************************************************
constructor
***********************************************************/
LBA_LAYOUT::LBA_LAYOUT(LBA_PACK *pack,int n)
{

    int i=0,j=0;
    long index_data=0;

	//std::cout<<"LBA_LAYOUT"<<std::endl;
    LBA_ENTRY *entry=new LBA_ENTRY(pack,n);
    number_objects=static_cast<int>((entry->data[0]+entry->data[1]*256+entry->data[2]*65536+entry->data[3]*16777216)/4);

    object=new LBA_OBJECT*[number_objects];

	//std::ofstream file("infolayout.txt");
	//file<<"info layout: "<<n<<std::endl;

    for(i=0;i<number_objects;i++)//
    {
        index_data=static_cast<long>((entry->data[i*4]+entry->data[i*4+1]*256+entry->data[i*4+2]*65536+entry->data[i*4+3]*16777216));
		//file<<i<<" "<<index_data<<std::endl;
		
		
		object[i]=new LBA_OBJECT(entry->data[index_data],entry->data[index_data+1],entry->data[index_data+2]);
		index_data+=3;
		//file<<(short)entry->data[index_data-3]<<" "<<(short)entry->data[index_data-2]<<" "<<(short)entry->data[index_data-1]<<std::endl;
		//file<<(short)object[i]->taille_X<<" "<<(short)object[i]->taille_Y<<" "<<(short)object[i]->taille_Z<<std::endl;
        for(j=0;j<object[i]->taille_X*object[i]->taille_Y*object[i]->taille_Z;j++)
        {
            object[i]->info_brick[j].shape=entry->data[index_data++];
            object[i]->info_brick[j].material=entry->data[index_data++];
            object[i]->info_brick[j].index_brick=entry->data[index_data]+(entry->data[index_data+1])*256;
            index_data+=2;
        }
    }

	//std::ofstream file("infolayout.txt");
	//file<<"info layout: "<<std::endl;
	//for(i=0;i<number_objects;i++)
 //   {
	//	file<<object[i]->info_brick[j][k].index_brick<<" ";
	//}
	//file<<std::endl;
	//file.close();


    delete entry;
}


/***********************************************************
destructor
***********************************************************/
LBA_LAYOUT::~LBA_LAYOUT()
{
    int i;
    for(i=0;i<number_objects;i++)
    delete object[i];
    delete object;
}




/***********************************************************
constructor
***********************************************************/
LBA_GRID::LBA_GRID(LBA_PACK *pack_grid,LBA_PACK *pack_layout,int n,bool LBA2, bool forcelayout)
{
    int i=0,j=0,k=0,l=0;
    LBA_LAYOUT *layout;
    int index_data=0;
    unsigned char flag=0,number_sub_colomn=0;
    int height_subcolumn=0,offset_column=0;

	//std::cout<<"LBA_GRID"<<std::endl;
    LBA_ENTRY *entry=new LBA_ENTRY(pack_grid->data,pack_grid->datalenght);

    if(!forcelayout && LBA2)
		n=entry->data[0]+180-1;

    layout=new LBA_LAYOUT(pack_layout,n);


    for(i=0;i<64;i++)
    for(j=0;j<64;j++)
    {
        info_brick=new LBA_INFO_BRICK **[64];
        for(i=0;i<64;i++)
        info_brick[i]=new LBA_INFO_BRICK *[64];
        for(i=0;i<64;i++)
        for(j=0;j<64;j++)
        info_brick[i][j]=new LBA_INFO_BRICK [25];
        for(i=0;i<64;i++)
        for(j=0;j<64;j++)
        for(k=0;k<25;k++)
        {
            info_brick[i][j][k].index_brick=0;
            info_brick[i][j][k].material=0xF0;
            info_brick[i][j][k].shape=0;
        }
    }


	//std::ofstream file("infobrick2.txt");
	//file<<"info brick: "<<std::endl;

    for(j=0;j<64;j++)
    for(i=0;i<64;i++)
    {
        if(LBA2)
			index_data=entry->data[2*(j*64+i)+34]+entry->data[2*(j*64+i)+1+34]*256+34;///////////////LBA2 +34 +34 +34
        else 
			index_data=entry->data[2*(j*64+i)]+entry->data[2*(j*64+i)+1]*256;

        number_sub_colomn=entry->data[index_data++];



        offset_column=0;
        for(k=0;k<number_sub_colomn;k++)
        {
            flag=entry->data[index_data]>>6;
            height_subcolumn=entry->data[index_data]%0x20+1;
            index_data++;

            if(flag==0)
            {
                offset_column+=height_subcolumn;

            }
            if(flag==1)
            {
                for(l=0;l<height_subcolumn;l++)
                {
                    if(entry->data[index_data]==0)
                    {
                        info_brick[i][j][offset_column].index_brick=0;
                        info_brick[i][j][offset_column].material=0xF0;
                        info_brick[i][j][offset_column].shape=1;
                    }
                    else
                    {
                        info_brick[i][j][offset_column]=layout->object[entry->data[index_data]-1]->info_brick[entry->data[index_data+1]];
						//file<<"1: "<<i<<" "<<j<<" "<<offset_column<<" "<<index_data<<" "<<(int)entry->data[index_data]<<" "<<(int)entry->data[index_data+1]<<" "<<layout->object[entry->data[index_data]-1]->info_brick[entry->data[index_data+1]].index_brick<<" "<<info_brick[i][j][offset_column].index_brick<<std::endl;
						//file<<(short)layout->object[entry->data[index_data]-1]->taille_X<<" "<<(short)layout->object[entry->data[index_data]-1]->taille_Y<<" "<<(short)layout->object[entry->data[index_data]-1]->taille_Z<<std::endl;
					
					}

                    offset_column++;
                    index_data+=2;
                }
            }
            if(flag==2)
            {
                for(l=0;l<height_subcolumn;l++)
                {
                    if(entry->data[index_data]==0)
                    {
                        info_brick[i][j][offset_column].index_brick=0;
                        info_brick[i][j][offset_column].material=0xF0;
						info_brick[i][j][offset_column].shape=1;
                    }
                    else
                    {
                        info_brick[i][j][offset_column]=layout->object[entry->data[index_data]-1]->info_brick[entry->data[index_data+1]];
                  		//file<<"2: "<<index_data<<" "<<(int)entry->data[index_data]<<" "<<layout->object[entry->data[index_data]-1]->info_brick[entry->data[index_data+1]].index_brick<<" "<<info_brick[i][j][offset_column].index_brick<<std::endl;  
					}
                    offset_column++;
                }
                index_data+=2;
            }
        }
    }


    delete layout;
    delete entry;
}


/***********************************************************
destructor
***********************************************************/
LBA_GRID::~LBA_GRID()
{
        int i,j;
        for(i=0;i<64;i++)
        for(j=0;j<64;j++)
        delete info_brick[i][j];
        for(i=0;i<64;i++)
        delete info_brick[i];
        delete info_brick;

}




/***********************************************************
constructor
***********************************************************/
LBA_MAP::LBA_MAP(bool LBA2, const std::string &grfile, int layoutused, bool forcelayout)
{
	//std::cout<<"LBA_MAP "<<n<<std::endl;

    int i=0,j=0,k=0,l=0;
	std::vector<int> brick_list;
    number_brick=0;

	LBA_PACK *pack_brick;
	LBA_PACK *pack_grid;
	LBA_PACK *pack_ress;
	LBA_PACK *pack_layout;

	if(LBA2)
	{
		pack_brick=new LBA_PACK(DataFileHandler::GetPath("LBA_BKG"));
		//pack_grid=new LBA_PACK(DataFileHandler::GetPath("LBA_BKG"));
		pack_ress=new LBA_PACK(DataFileHandler::GetPath("RESS2"));
		pack_layout=new LBA_PACK(DataFileHandler::GetPath("LBA_BKG"));}
	else
	{
		pack_brick=new LBA_PACK(DataFileHandler::GetPath("LBA_BRK"));
		//pack_grid=new LBA_PACK(DataFileHandler::GetPath("LBA_GRI"));
		pack_ress=new LBA_PACK(DataFileHandler::GetPath("RESS"));
		pack_layout=new LBA_PACK(DataFileHandler::GetPath("LBA_BLL"));
	}

	pack_grid=new LBA_PACK(grfile);

    palet=new LBA_PALET(pack_ress);
    grid=new LBA_GRID(pack_grid,pack_layout,layoutused,LBA2, forcelayout);


    printf("%d ",number_brick);




	for(i=0;i<64;i++)
    for(j=0;j<64;j++)
    for(k=0;k<25;k++)
    {
		grid->info_brick[i][j][k].sound = 0;
		if(grid->info_brick[i][j][k].index_brick != 0)
		{
			if(!LBA2)
			{
				int mat = (grid->info_brick[i][j][k].material >> 4);
				int mat2 = grid->info_brick[i][j][k].material & unsigned char(15);

				if(mat == 15 && mat2 == 1)
				{
					//grid->info_brick[i][j][k].shape = 15;
					grid->info_brick[i][j][k].sound = 17;
				}

				switch(mat)
				{
					case 0:
						grid->info_brick[i][j][k].sound =1;
					break;
					case 1:
						grid->info_brick[i][j][k].sound =8;
					break;
					case 2:
						grid->info_brick[i][j][k].sound =6;
					break;
					case 3:
						grid->info_brick[i][j][k].sound =3;
					break;
					case 4:
						grid->info_brick[i][j][k].sound =2;
					break;
					case 5:
						grid->info_brick[i][j][k].sound =5;
					break;
					case 6:
						grid->info_brick[i][j][k].sound =4;
					break;
					case 7:
						grid->info_brick[i][j][k].sound =11;
					break;
					case 8:
						grid->info_brick[i][j][k].sound =12;
					break;
					case 9:
						grid->info_brick[i][j][k].sound =9;
					break;
					case 10:
						grid->info_brick[i][j][k].sound =15;
					break;
					case 11:
						grid->info_brick[i][j][k].sound =16;
					break;
					case 12:
						grid->info_brick[i][j][k].sound =14;
					break;
				}

			}
			else
			{
				int mat = (grid->info_brick[i][j][k].material >> 4);
				int mat2 = grid->info_brick[i][j][k].material & unsigned char(15);


				switch(mat2)
				{
					case 0:
						grid->info_brick[i][j][k].sound =0;
					break;
					case 1:
						grid->info_brick[i][j][k].sound =1;
					break;
					case 2:
						grid->info_brick[i][j][k].sound =1;
					break;
					case 3:
						grid->info_brick[i][j][k].sound =3;
					break;
					case 4:
						grid->info_brick[i][j][k].sound =4;
					break;
					case 5:
						grid->info_brick[i][j][k].sound =5;
					break;
					case 6:
						grid->info_brick[i][j][k].sound =6;
					break;
					case 7:
						grid->info_brick[i][j][k].sound =7;
					break;
					case 8:
						grid->info_brick[i][j][k].sound =8;
					break;
					case 9:
						grid->info_brick[i][j][k].sound =9;
					break;
					case 10:
						grid->info_brick[i][j][k].sound =10;
					break;
					case 11:
						grid->info_brick[i][j][k].sound =11;
					break;
					case 12:
						grid->info_brick[i][j][k].sound =6;
					break;
					case 13:
						grid->info_brick[i][j][k].sound =13;
					break;
					case 14:
						grid->info_brick[i][j][k].sound =1;
					break;
				}

				if(mat == 1 || mat == 15)
				{
					//grid->info_brick[i][j][k].shape = 15; // water
					grid->info_brick[i][j][k].sound = 17;
				}
				if(mat == 9 || mat == 13)
				{
					//grid->info_brick[i][j][k].shape = 16; // lava
					grid->info_brick[i][j][k].sound = 19;
				}
				if( mat == 11 || mat == 14)
				{
					//grid->info_brick[i][j][k].shape = 17; // gas
					grid->info_brick[i][j][k].sound = 18;
				}
			}
		}
	}




    for(i=0;i<64;i++)
    for(j=0;j<64;j++)
    for(k=0;k<25;k++)
    {
        if(grid->info_brick[i][j][k].index_brick!=0)
        {
			//serach if we already have this brick
            for(l=0;l<number_brick;l++)
				if(brick_list[l]==grid->info_brick[i][j][k].index_brick)
					break;

			// if not add it to the buffer
            if(l==number_brick)
            {
                brick_list.push_back(grid->info_brick[i][j][k].index_brick);
                number_brick++;
            }
        }
    }

	std::sort(brick_list.begin(), brick_list.end());

	std::string tmpfile = grfile;
	tmpfile.replace(tmpfile.size() - 3, 3, "txt");

	std::ofstream file(tmpfile.c_str());
	file<<64<<" "<<25<<" "<<64<<std::endl;
	file<<number_brick<<std::endl<<std::endl;
	for(int cc=0;cc<number_brick;++cc)
	{
		if(brick_list[cc] != 0)
		{
			if(!LBA2)
				file<<"Bricks/Lba1/"<<brick_list[cc]-1<<".png"<<std::endl;
			else
				file<<"Bricks/Lba2/"<<brick_list[cc]-1<<".png"<<std::endl;
		}
	}
	file<<std::endl;

	// replace global brick indexes by local file indexes
    for(k=0;k<25;k++)
    {
		// put graphic info
	    for(i=0;i<64;i++)
		{
			for(j=0;j<64;j++)
			{
				if(grid->info_brick[i][j][k].index_brick!=0)
				{
					l=0;
					while(grid->info_brick[i][j][k].index_brick!=brick_list[l])
						l++;

					file<<l+1<<" ";
				}
				else
					file<<"0 ";
			}
			file<<std::endl;
		}
		file<<std::endl;

		// put physic info
	    for(i=0;i<64;i++)
		{
			for(j=0;j<64;j++)
				file<<(int)grid->info_brick[i][j][k].shape<<" ";

			file<<std::endl;
		}
		file<<std::endl;

		// put material info
	    for(i=0;i<64;i++)
		{
			for(j=0;j<64;j++)
				file<<grid->info_brick[i][j][k].sound<<" ";

			file<<std::endl;
		}
		file<<std::endl;

	}

    delete pack_brick;
    delete pack_grid;
    delete pack_layout;
    delete pack_ress;
}




/***********************************************************
destructor
***********************************************************/
LBA_MAP::~LBA_MAP()
{
    delete palet;
    delete grid;
}

