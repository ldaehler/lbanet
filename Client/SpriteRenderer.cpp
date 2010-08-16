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

#include "SpriteRenderer.h"

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library

#include <IL/ilut.h>


/*
---------------------------------------------------------------------------------------
- constructor
---------------------------------------------------------------------------------------
*/
SpriteRenderer::SpriteRenderer()
	: _currIndex(0), _texture_loaded(false)
{
	_type = 1;
}

/*
---------------------------------------------------------------------------------------
- destructor
---------------------------------------------------------------------------------------
*/
SpriteRenderer::~SpriteRenderer()
{
	CleanUp();
}


/*
---------------------------------------------------------------------------------------
- cleanup the textures
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::CleanUp()
{
	if(!_texture_loaded)
		return;

	for(size_t i=0; i<_textures.size(); ++i)
		glDeleteTextures(1, &_textures[i]);

	_textures.clear();
	_widths.clear();
	_heights.clear();

	_currIndex = 0;
	_texture_loaded = false;
}

/*
---------------------------------------------------------------------------------------
- set the current sprites
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::SetSprites(std::vector<SpriteInfo *> sprites)
{
	CleanUp();

	_sprites = sprites;
	if(sprites.size() > 0)
	{
		for(size_t i=0; i<sprites.size(); ++i)
			Load(sprites[i]->filename);

		_currIndex = 0;
	}
	_texture_loaded = true;
}


/*
---------------------------------------------------------------------------------------
- load function
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::Load(const std::string & filename)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	ilLoadImage(const_cast<char *>(filename.c_str()));

	_widths.push_back(ilGetInteger(IL_IMAGE_WIDTH));
	_heights.push_back(ilGetInteger(IL_IMAGE_HEIGHT));
	_textures.push_back(ilutGLBindTexImage());

	ilDeleteImages(1, &ImgId);
}

/*
---------------------------------------------------------------------------------------
- Process function
---------------------------------------------------------------------------------------
*/
int SpriteRenderer::Process(double tnow, float tdiff)
{
	return 0;
}

/*
---------------------------------------------------------------------------------------
- Draw function
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::Render(float alpha)
{
	if(_sprites.size() <= 0 || !_texture_loaded)
		return;

	if(!Visible)
		return;

	//glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textures[_currIndex]);
	// the texture wraps over at the edges (repeat)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor4f(1,1,1, alpha);

	std::vector<QuadImageInfo>::const_iterator it = _sprites[_currIndex]->quadsInfo.begin();
	std::vector<QuadImageInfo>::const_iterator end = _sprites[_currIndex]->quadsInfo.end();

	glBegin(GL_QUADS);

	for(; it != end; ++it)
	{
		if(it->UseFullImage == true)
		{
			glTexCoord2f(1, 1);
			glVertex3f( it->TopRightCornerX,  it->TopRightCornerY/2.0f, it->TopRightCornerZ);
			glTexCoord2f(0, 1);
			glVertex3f(it->TopLeftCornerX,  it->TopLeftCornerY/2.0f, it->TopLeftCornerZ);
			glTexCoord2f(0, 0);
			glVertex3f(it->BottomLeftCornerX, it->BottomLeftCornerY/2.0f, it->BottomLeftCornerZ);
			glTexCoord2f(1, 0);
			glVertex3f( it->BottomRightCornerX, it->BottomRightCornerY/2.0f, it->BottomRightCornerZ);
		}
		else
		{
			float _TextureWidthInPixels = (float)_widths[_currIndex];
			float _TextureHeightInPixels = (float)_heights[_currIndex];

			glTexCoord2f(it->TopRightTextcoordX/_TextureWidthInPixels, it->TopRightTextcoordY/_TextureHeightInPixels);
			glVertex3f( it->TopRightCornerX,  it->TopRightCornerY/2.0f, it->TopRightCornerZ);
			glTexCoord2f(it->TopLeftTextcoordX/_TextureWidthInPixels, it->TopLeftTextcoordY/_TextureHeightInPixels);
			glVertex3f(it->TopLeftCornerX,  it->TopLeftCornerY/2.0f, it->TopLeftCornerZ);
			glTexCoord2f(it->BottomLeftTextcoordX/_TextureWidthInPixels, it->BottomLeftTextcoordY/_TextureHeightInPixels);
			glVertex3f(it->BottomLeftCornerX, it->BottomLeftCornerY/2.0f, it->BottomLeftCornerZ);
			glTexCoord2f(it->BottomRightTextcoordX/_TextureWidthInPixels, it->BottomRightTextcoordY/_TextureHeightInPixels);
			glVertex3f( it->BottomRightCornerX, it->BottomRightCornerY/2.0f, it->BottomRightCornerZ);
		}
	}
	glEnd();
}

/*
---------------------------------------------------------------------------------------
- called to reload element when resizing screen
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::Reload()
{
	size_t saveid = _currIndex;
	SetSprites(_sprites);
	_currIndex = saveid;
}

/*
---------------------------------------------------------------------------------------
- change to next preloaded image
- if counter is at end, the first preloaded image will be shown again
---------------------------------------------------------------------------------------
*/
void SpriteRenderer::Next()
{
	if(_currIndex < 0)
		return;

	++_currIndex;
	if(_currIndex >= _sprites.size())
		_currIndex = 0;
}





