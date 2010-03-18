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


#ifndef _LBANET_USER_ALLOCATOR_HANDLER_H_
#define _LBANET_USER_ALLOCATOR_HANDLER_H_

class UserAllocator;

//*************************************************************************************************
//*                               class UserAllocatorHandler
//*************************************************************************************************
/**
* @brief Wrapper class around user memory allocator used by the physx engine
*
*/
class UserAllocatorHandler
{
public:

	// singleton pattern
	static UserAllocatorHandler * getInstance();

	//! destructor
	~UserAllocatorHandler(){}

	//! initialize the class
	//! Important: this is not thread safe and should be called only once on the main thread
	//! before the first call of the get allocator function
	void Initialize();


	//! get pointer to the allocator
	//! this is thread safe but only after you called init function
	UserAllocator * GetAllocator()
	{ return _allocatorPtr; }


protected:
	//! constructor
	UserAllocatorHandler()
		: _allocatorPtr(0)
	{}

private:
	// singleton
	static UserAllocatorHandler * _singletonInstance;

	UserAllocator * _allocatorPtr;
};


#endif
