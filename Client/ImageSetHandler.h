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


#ifndef _LBANET_IMAGESET_HANDLER_H_
#define _LBANET_IMAGESET_HANDLER_H_


#include <string>
#include <vector>

//*************************************************************************************************
//*                               class ImageSetHandler
//*************************************************************************************************
/**
* @brief Class handling imageset for the gui
*
*/
class ImageSetHandler
{
public:
	// destructor
	~ImageSetHandler();

	// get singleton instance
	static ImageSetHandler *	GetInstance();

	// get inventory image
	std::string GetInventoryImage(long InventoryId);

	// get image of stance
	std::string GetStanceImage(int stance);

	// set inventory files
	void SetInventoryFiles(const std::vector<std::string> & files);

protected:
	// constructor
	ImageSetHandler();

private:

	static ImageSetHandler *	_singleton_instance;
	std::vector<std::string>	_inventory_filenames;

};


#endif
