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


#ifndef _LBANET_SPRITE_RENDERER_
#define _LBANET_SPRITE_RENDERER_

#include <string>
#include <vector>
#include "3DObjectRenderer.h"
#include "WorldInfo.h"

/***********************************************************************
 * Module:  SpriteRenderer.h
 * Author:  vivien
 * Modified: mercredi 15 juillet 2009 13:34:13
 * Purpose: Declaration of the class 3DObjectRenderer
 **********************************************************************/
class SpriteRenderer : public D3ObjectRenderer
{
public:
	//! constructor
	//! colorformat: 3=RGB - 4=RGBA
	SpriteRenderer();

	//! destructor
	~SpriteRenderer();

	//! set the current sprites
	void SetSprites(std::vector<SpriteInfo *> sprites);

	//! Draw function
	void Render(float alpha);

	//! Process function
	virtual int Process(double tnow, float tdiff);

	////! get image height
	int GetImageHeight()
	{
		if(_currIndex >= 0)
			return _heights[_currIndex];
		else
			return 0;
	}

	////! get image width
	int GetImageWidth()
	{
		if(_currIndex >= 0)
			return _widths[_currIndex];
		else
			return 0;
	}

	//! called to reload element when resizing screen
	virtual void Reload();


	//! change to next preloaded image
	//! if counter is at end, the first preloaded image will be shown again
	void Next();

	//! cleanup the textures
	void CleanUp();

	//! only used form sprites
	virtual long GetSpriteId() 
	{return _sprites[0]->id;}

protected:
	//! load function
	virtual void Load(const std::string & filename);


protected:
	std::vector<SpriteInfo *>	_sprites;
	std::vector<unsigned int>	_textures;
	std::vector<int>			_widths;
	std::vector<int>			_heights;
	size_t						_currIndex;

	bool						_texture_loaded;
};

#endif

