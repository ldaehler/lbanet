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

#include "TextWritter.h"
#include "ConfigurationManager.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <IL/ilut.h>


TextWritter* TextWritter::_singletonInstance = NULL;



/***********************************************************
singleton pattern
***********************************************************/
TextWritter * TextWritter::getInstance()
{
    if(!_singletonInstance)
        _singletonInstance = new TextWritter();

	return _singletonInstance;
}


/***********************************************************
Constructor
***********************************************************/
TextWritter::TextWritter()
: _initialized(false)
{
	ReloadTexture();
}


/***********************************************************
reload font texture
***********************************************************/
void TextWritter::ReloadTexture()
{
	KillFont();
	LoadGLTextures("Data/Font.png", _textTextureIdx);
	BuildFont(_textTextureIdx);
	_initialized = true;

	ConfigurationManager::GetInstance()->GetInt("Options.General.TextR", m_textR);
	ConfigurationManager::GetInstance()->GetInt("Options.General.TextG", m_textG);
	ConfigurationManager::GetInstance()->GetInt("Options.General.TextB", m_textB);
}


/***********************************************************
Destructor
***********************************************************/
TextWritter::~TextWritter()
{
	KillFont();
}


/***********************************************************
set text color
***********************************************************/
void TextWritter::SetTextColor(int R, int G, int B)
{
	m_textR = R;
	m_textG = G;
	m_textB = B;
}

/***********************************************************
load a texture in memory
***********************************************************/
bool TextWritter::LoadGLTextures(const std::string & textureName, unsigned int & textureHandler)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	ilLoadImage(const_cast<char *>(textureName.c_str()));
	textureHandler = ilutGLBindTexImage();
	ilDeleteImages(1, &ImgId);

	return true;
}


/***********************************************************
build the font from a texture
***********************************************************/
void TextWritter::BuildFont(unsigned int textureHandler)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	_textDisplayList=glGenLists(256);					// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, textureHandler);		// Select Our Font Texture
	for (int loop=0; loop<256; loop++)					// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(_textDisplayList+loop,GL_COMPILE);		// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(8,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}

/***********************************************************
remove the font from memory
***********************************************************/
void TextWritter::KillFont()
{
	if(_initialized)
	{
		glDeleteLists(_textDisplayList,256);
		glDeleteTextures(1, &_textTextureIdx);
	}
	_initialized = false;
}


/***********************************************************
print a text to the screen
***********************************************************/
void TextWritter::glPrintText(const std::string & str, int fontset, bool setcolor)
{
	if(fontset>1)
		fontset=1;

    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
	if(setcolor)
		glColor4ub((unsigned char)m_textR, (unsigned char)m_textG, (unsigned char)m_textB, (unsigned char)255);
	glBindTexture(GL_TEXTURE_2D, _textTextureIdx);		// Select Our Font Texture
	glListBase(_textDisplayList/*-32+(128*fontset)*/);		// Choose The Font Set (0 or 1)
	glCallLists(str.length(),GL_UNSIGNED_BYTE,str.c_str());// Write The Text To The Screen

}



/***********************************************************
print a text to the screen
***********************************************************/
void TextWritter::glPrintTextOnScreen(const std::string & str, int fontset,
										int screen_sizeX,int screen_sizeY, int xpos, int ypos)
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,screen_sizeX,0,screen_sizeY,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslated(xpos,ypos,0);
	glScalef(1.5f, 1.5f , 1.5f);
	glPrintText(str, fontset);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}




