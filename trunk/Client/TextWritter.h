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


#ifndef _LBA_NET_TEXT_WRITER_H_
#define _LBA_NET_TEXT_WRITER_H_

#include <string>

/***********************************
*	class taking care of writting text on opengl screen
*   use singleton pattern as we only need one instance for the whole application
*************************************/
class TextWritter
{
public:
	// constructor
	TextWritter();

	// destructor
    ~TextWritter();

	// singleton pattern
   static TextWritter * getInstance();

	//print a text to the screen
	void glPrintText(const std::string & str, int fontset, bool setcolor=true);


	//print a text to the screen
	void glPrintTextOnScreen(const std::string & str, int fontset,
								int screen_sizeX,int screen_sizeY, int xpos, int ypos);


	// reload font texture
	void ReloadTexture();

	//set text color
	void SetTextColor(int R, int G, int B);

	//remove the font from memory
	void KillFont();

protected:

	//load a texture in memory
	bool LoadGLTextures(const std::string & textureName, unsigned int & textureHandler);

	// build the font in memory
	void BuildFont(unsigned int textureHandler);


private:
	unsigned int				_textTextureIdx;	// Storage index of the Font Texture
	unsigned int				_textDisplayList;	// Base Display List For The Font
	bool						_initialized;

	// text color
	int							m_textR;
	int							m_textG;
	int							m_textB;

	// singleton
	static TextWritter *		_singletonInstance;

};


#endif
