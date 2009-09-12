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


#ifndef _LBANET_AVI_VIDEO_RENDERER_
#define _LBANET_AVI_VIDEO_RENDERER_

#include "SpriteRenderer.h"
#include <windows.h>
#include <vfw.h>

class AviVideoRenderer : public SpriteRenderer
{
public:
	//! constructor
	AviVideoRenderer();

	//! destructor
	~AviVideoRenderer();

	//! Process function
	virtual int Process(double tnow, float tdiff);

protected:
	//! load function
	virtual void Load(const std::string & filename);

	void flipIt(unsigned char* buffer, int sizeX, int sizeY);
	bool OpenAVI(const char * szFile);
	void GrabAVIFrame(int frame);
	void CloseAVI(void);
	void InitializeTexture();

private:
	bool				_video_opened;				// flag saying if the video has been opened
	long				_timeperframe;				// approximate time per frame
	long				_lastFrame;					// last displayed avi frame
	long				_endFrame;					// index of the end avi frame
	bool				_firstTime;					// flag first time we play the video
	unsigned long		_startingTime;				// starting time when the video played

	int					_video_width;				// Video Width
	int					_video_height;				// Video Height
	unsigned char*		_img_data;					// Pointer To Our Resized Image
	bool				_avi_opened;

	static long			_init_counter;


	// TODO - remove that and the windows.h include from the header - place this part on a separate class and use pointer here
	AVISTREAMINFO		_AVR_psi;								// Pointer To A Structure Containing Stream Info
	PAVISTREAM			_AVR_pavi;								// Handle To An Open Stream
	PGETFRAME			_AVR_pgf;								// Pointer To A GetFrame Object
	BITMAPINFOHEADER	_AVR_bmih;								// Header Information For DrawDibDraw Decoding
	HDRAWDIB			_AVR_hdd;								// Handle For Our Dib
	HBITMAP				_AVR_hBitmap;							// Handle To A Device Dependant Bitmap
	HDC					_AVR_hdc;								// Creates A Compatible Device Context

};

#endif