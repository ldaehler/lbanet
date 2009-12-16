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

#include "AviVideoRenderer.h"

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
											// Header File For Video For Windows




long AviVideoRenderer::_init_counter = 0;


/*
---------------------------------------------------------------------------------------
- Flips The Red And Blue Bytes
---------------------------------------------------------------------------------------
*/
void AviVideoRenderer::flipIt(unsigned char* buffer, int sizeX, int sizeY)
{
	unsigned char* b = buffer;
	size_t sizebuffer = sizeX * sizeY;
	unsigned char tmpv;

	for(size_t i=0; i<sizebuffer; ++i)
	{
		tmpv = *b;
		*b = *(b+2);
		b += 2;
		*b = tmpv;
		++b;
	}
}


/*
---------------------------------------------------------------------------------------
- Opens An AVI File (szFile)
---------------------------------------------------------------------------------------
*/
bool AviVideoRenderer::OpenAVI(const char * szFile)
{
	// Opens The AVI Stream
	if (AVIStreamOpenFromFileA(&_AVR_pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
		return false;

	AVIStreamInfo(_AVR_pavi, &_AVR_psi, sizeof(_AVR_psi));						// Reads Information About The Stream Into psi
	_video_width=_AVR_psi.rcFrame.right-_AVR_psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	_video_height=_AVR_psi.rcFrame.bottom-_AVR_psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top

	_endFrame=AVIStreamLength(_AVR_pavi);							// The Last Frame Of The Stream

	_timeperframe=AVIStreamSampleToTime(_AVR_pavi,_endFrame)/_endFrame;		// Calculate Rough Milliseconds Per Frame


	int sizet = 1024;
	if((_video_width < 128) && (_video_height < 128))
		sizet = 128;
	else if((_video_width < 256) && (_video_height < 256))
		sizet = 256;
	else if((_video_width < 512) && (_video_height < 512))
		sizet = 512;

	_widths.push_back(sizet);
	_heights.push_back(sizet);

	// initialize the host texture
	InitializeTexture();


	_AVR_bmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	_AVR_bmih.biPlanes = 1;											// Bitplanes
	_AVR_bmih.biBitCount = 24;										// Bits Format We Want (24 Bit, 3 Bytes)
	_AVR_bmih.biWidth = _widths[0];									// Width We Want
	_AVR_bmih.biHeight = _heights[0];								// Height We Want
	_AVR_bmih.biCompression = BI_RGB;								// Requested Mode = RGB

	_AVR_hBitmap = CreateDIBSection (_AVR_hdc, (BITMAPINFO*)(&_AVR_bmih), DIB_RGB_COLORS, (void**)(&_img_data), NULL, NULL);
	SelectObject (_AVR_hdc, _AVR_hBitmap);							// Select hBitmap Into Our Device Context (hdc)

	_AVR_pgf=AVIStreamGetFrameOpen(_AVR_pavi, NULL);				// Create The PGETFRAME	Using Our Request Mode


	_avi_opened = true;

	return(_AVR_pgf!=NULL);
}


/*
---------------------------------------------------------------------------------------
- Grabs A Frame From The Stream
---------------------------------------------------------------------------------------
*/
void AviVideoRenderer::GrabAVIFrame(int frame)
{
	LPBITMAPINFOHEADER lpbi;									// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(_AVR_pgf, frame);	// Grab Data From The AVI Stream
	char* _tex_pdata=(char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);	// Pointer To Data Returned By AVIStreamGetFrame

	// Convert Data To Requested Bitmap Format
	DrawDibDraw (_AVR_hdd, _AVR_hdc, 0, 0,  _heights[0], _widths[0], lpbi, _tex_pdata, 0, 0, _video_width, _video_height, 0);

	flipIt(_img_data, _heights[0], _widths[0]);	// Swap The Red And Blue Bytes (GL Compatability)

	// Update The Texture
	glBindTexture(GL_TEXTURE_2D, _textures[0]);
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0,  _heights[0], _widths[0], GL_RGB, GL_UNSIGNED_BYTE, _img_data);


}



/*
---------------------------------------------------------------------------------------
- Properly Closes The Avi File
---------------------------------------------------------------------------------------
*/
void AviVideoRenderer::CloseAVI(void)
{
	if(_avi_opened)
	{
		DeleteObject(_AVR_hBitmap);							// Delete The Device Dependant Bitmap Object
		DrawDibClose(_AVR_hdd);								// Closes The DrawDib Device Context
		AVIStreamGetFrameClose(_AVR_pgf);					// Deallocates The GetFrame Resources
		AVIStreamRelease(_AVR_pavi);						// Release The Stream
	}

	_avi_opened = false;
}




/*
---------------------------------------------------------------------------------------
- constructor
---------------------------------------------------------------------------------------
*/
AviVideoRenderer::AviVideoRenderer()
: _avi_opened(false)
{
	_AVR_hdc = CreateCompatibleDC(0);

	if(_init_counter == 0)
		AVIFileInit();		// Opens The AVIFile Library

	++_init_counter;

	_type = 2;
}

/*
---------------------------------------------------------------------------------------
- InitializeTexture
---------------------------------------------------------------------------------------
*/
void AviVideoRenderer::InitializeTexture()
{
	unsigned int _texture_index;

	glEnable(GL_TEXTURE_2D);										// Enable Texture Mapping
	glGenTextures( 1, &_texture_index );
	glBindTexture(GL_TEXTURE_2D, _texture_index);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter


	char *buffer = new char[_heights[0]*_widths[0]*3];
	memset(buffer, 0, _heights[0]*_widths[0]*3);

	// Create The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  _widths[0],  _heights[0], 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	_textures.push_back(_texture_index);
}


/*
---------------------------------------------------------------------------------------
- destructor
---------------------------------------------------------------------------------------
*/
AviVideoRenderer::~AviVideoRenderer()
{
	CloseAVI();
	--_init_counter;

	if(_init_counter == 0)
		AVIFileExit();		// Release The File
}

/*
---------------------------------------------------------------------------------------
- load function
---------------------------------------------------------------------------------------
*/
void AviVideoRenderer::Load(const std::string & filename)
{
	CloseAVI();

	_AVR_hdd = DrawDibOpen();


	// Open The AVI File
	_video_opened = OpenAVI(filename.c_str());

	_lastFrame = -1;
	_firstTime = true;
}

/*
---------------------------------------------------------------------------------------
- Process function
---------------------------------------------------------------------------------------
*/
int AviVideoRenderer::Process(double tnow, float tdiff)
{
	if(!_video_opened)
		return 0;

	if(_firstTime)
	{
		_firstTime = false;
		_startingTime = (unsigned long)tnow;
	}

	unsigned long currtime = (unsigned long)tnow;

	long frame=(currtime-_startingTime)/_timeperframe;		// Calculate The Current Frame

	if (frame>=_endFrame)									// Are We At Or Past The Last Frame?
	{
		frame=0;											// Reset The Frame Back To Zero (Start Of Video)
		_firstTime=true;									// Reset The Animation Timer (next)
	}

	if(_lastFrame != frame)
	{
		GrabAVIFrame(frame);
		_lastFrame = frame;
	}

	return 0;
}


