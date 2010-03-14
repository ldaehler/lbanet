/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/


#ifndef __LBA_NET_BRICKINFO_HANDLER_H__
#define __LBA_NET_BRICKINFO_HANDLER_H__

#include <string>
#include <vector>
#include <fstream>

struct BrickInfo
{
	std::string path;
	int shape;
};

//*************************************************************************************************
//*                                      class BrickInfoHandler
//*************************************************************************************************

class BrickInfoHandler
{
public:

	//! constructor
	BrickInfoHandler(const std::string &Filename)
	{
		std::ifstream file(Filename.c_str());
		while(!file.eof())
		{
			BrickInfo tmp;
			std::string tmps;
			file >> tmp.shape;
			file >> tmp.path;
			file >> tmp.shape;
			_infos.push_back(tmp);
		}
	}

	// get info of a brick
	const BrickInfo & GetInfo(int idx) const
	{return _infos[idx];}


private:
	std::vector<BrickInfo>	_infos;
};

#endif
