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


#ifndef __LBA_NET_ZOID_SERIALIZER_H__
#define __LBA_NET_ZOID_SERIALIZER_H__

#include "SerializerBase.h"

class ZCom_BitStream;

/***********************************************************************
Base serialization of object beofre sending it over the network using zoidcom
 ***********************************************************************/
class ZoidSerializer : public SerializerBase
{
public:
	//! constructor
	ZoidSerializer(ZCom_BitStream * stream)
		: _stream(stream)
	{}

	//! destructor
	~ZoidSerializer(){}


	// function using to build tree structures
	virtual void startChildObjectList(const std::string &name){}
	virtual void nextChildObject(const std::string &name){}
	virtual void finishChildObjectList(const std::string &name){}


	// data will be stored as a bool on 1bit
	virtual void addBool(bool data, const std::string &id);
	virtual bool getBool(const std::string &id);

	// data will be stored as a unsigned short using 8bits
	virtual void addUShort(unsigned short data, const std::string &id);
	virtual unsigned short getUShort(const std::string &id);

	// data will be stored as a unsigned int using 16bits
	virtual void addUInt(unsigned int data, const std::string &id);
	virtual unsigned int getUInt(const std::string &id);

	// data will be stored as a unsigned long using 32bits
	virtual void addULong(unsigned long data, const std::string &id);
	virtual unsigned long getULong(const std::string &id);


	// data will be stored as a signed short using 8bits
	virtual void addShort(short data, const std::string &id);
	virtual short getShort(const std::string &id);

	// data will be stored as a signed int using 16bits
	virtual void addInt(int data, const std::string &id);
	virtual int getInt(const std::string &id);

	// data will be stored as a signed long using 32bits
	virtual void addLong(long data, const std::string &id);
	virtual long getLong(const std::string &id);

	// data will be stored as a float using 32bits
	virtual void addFloat(float data, const std::string &id);
	virtual float getFloat(const std::string &id);

	// data will be stored as a string of dynamic dimension
	// however the size of the string should not be larger than 250 characters
	virtual void addString(const std::string &data, const std::string &id);
	virtual std::string getString(const std::string &id);


private: 
	ZCom_BitStream * _stream;
};


#endif
