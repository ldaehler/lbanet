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

#ifndef _LBANET_USERALLOCATOR_H
#define _LBANET_USERALLOCATOR_H

#include <boost/thread/mutex.hpp>
#include "NxUserAllocator.h"

/***********************************************************************
 * Module:  UserAllocator.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Memory allocator for physix engine
 ***********************************************************************/
class UserAllocator : public NxUserAllocator
{
public:
	//! constructor
	UserAllocator();

	//! destructor
	virtual	~UserAllocator();

	void		reset();

	void*		malloc(size_t size);
	void*		malloc(size_t size, NxMemoryType type);
	void*		mallocDEBUG(size_t size, const char* file, int line);
	void*		mallocDEBUG(size_t size, const char* file, int line, const char* className, NxMemoryType type);
	void*		realloc(void* memory, size_t size);
	void		free(void* memory);

	size_t*		mMemBlockList;
	NxU32		mMemBlockListSize;
	NxU32		mMemBlockFirstFree;
	NxU32		mMemBlockUsed;

	NxI32		mNbAllocatedBytes;
	NxI32		mHighWaterMark;
	NxI32		mTotalNbAllocs;
	NxI32		mNbAllocs;
	NxI32		mNbReallocs;

	boost::mutex mAllocatorLock;

};

#endif
