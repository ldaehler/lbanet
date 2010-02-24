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

#include "ObjMapRenderer.h"



#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library

#include <IL/ilut.h>

/***********************************************************
	Constructor
***********************************************************/
ObjMapRenderer::ObjMapRenderer(const std::string &objfilename, const std::string &texturefilename)
: _objfilename(objfilename), _texturefilename(texturefilename), _objmodel(NULL), _textureLoaded(false)
{
	_objmodel = ObjFileLoader::LoadObjFile(objfilename);
	ReloadTexture();
}

/***********************************************************
	destructor
***********************************************************/
ObjMapRenderer::~ObjMapRenderer()
{
	if(_objmodel)
		delete _objmodel;

	FlushTexture();
}


/***********************************************************
	render
***********************************************************/
void ObjMapRenderer::Render()
{
	if(_objmodel)
	{
		glColor4d(1,1,1,1);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		if(_textureLoaded)
		{
			glBindTexture(GL_TEXTURE_2D, _textureid);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_CLAMP);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_CLAMP);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	GL_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_CLAMP );
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_CLAMP );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	GL_LINEAR);
		}

		_objmodel->Render();
	}
}


/***********************************************************
cut the room at a certain Y to display only bottom
***********************************************************/
void ObjMapRenderer::SetMapYLimit(int YLimit)
{
	_YLimit = YLimit;
	_objmodel->Compile(YLimit);
}



/***********************************************************
flush the current map texture
***********************************************************/
void ObjMapRenderer::FlushTexture()
{
	if(_textureLoaded)
	{
		glDeleteTextures(1, &_textureid);
		_textureLoaded = false;
	}
}

/***********************************************************
reload the current map texture
***********************************************************/
void ObjMapRenderer::ReloadTexture()
{
	FlushTexture();

	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	ilLoadImage(const_cast<char *>(_texturefilename.c_str()));
	_textureid = ilutGLBindTexImage();
	ilDeleteImages(1, &ImgId);

	_textureLoaded = true;
}
