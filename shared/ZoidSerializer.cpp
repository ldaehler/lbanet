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

#include "ZoidSerializer.h"
#include <zoidcom.h>

/***********************************************************
data will be stored as a bool on 1bit
***********************************************************/
void ZoidSerializer::addBool(bool data, const std::string &id)
{
	_stream->addBool(data);
}

bool ZoidSerializer::getBool(const std::string &id)
{
	return _stream->getBool();
}


/***********************************************************
data will be stored as a unsigned short using 8bits
***********************************************************/
void ZoidSerializer::addUShort(unsigned short data, const std::string &id)
{
	_stream->addInt(data, 8);
}

unsigned short ZoidSerializer::getUShort(const std::string &id)
{
	return _stream->getInt(8);
}


/***********************************************************
data will be stored as a unsigned int using 16bits
***********************************************************/
void ZoidSerializer::addUInt(unsigned int data, const std::string &id)
{
	_stream->addInt(data, 16);
}

unsigned int ZoidSerializer::getUInt(const std::string &id)
{
	return _stream->getInt(16);
}


/***********************************************************
data will be stored as a unsigned long using 32bits
***********************************************************/
void ZoidSerializer::addULong(unsigned long data, const std::string &id)
{
	_stream->addInt(data, 32);
}

unsigned long ZoidSerializer::getULong(const std::string &id)
{
	return _stream->getInt(32);
}



/***********************************************************
data will be stored as a signed short using 8bits
***********************************************************/
void ZoidSerializer::addShort(short data, const std::string &id)
{
	_stream->addSignedInt(data, 8);
}

short ZoidSerializer::getShort(const std::string &id)
{
	return _stream->getSignedInt(8);
}


/***********************************************************
data will be stored as a signed int using 16bits
***********************************************************/
void ZoidSerializer::addInt(int data, const std::string &id)
{
	_stream->addSignedInt(data, 16);
}

int ZoidSerializer::getInt(const std::string &id)
{
	return _stream->getSignedInt(16);
}


/***********************************************************
data will be stored as a signed long using 32bits
***********************************************************/
void ZoidSerializer::addLong(long data, const std::string &id)
{
	_stream->addSignedInt(data, 32);
}

long ZoidSerializer::getLong(const std::string &id)
{
	return _stream->getSignedInt(32);
}


/***********************************************************
data will be stored as a float using 32bits
***********************************************************/
void ZoidSerializer::addFloat(float data, const std::string &id)
{
	_stream->addFloat(data, 32);
}

float ZoidSerializer::getFloat(const std::string &id)
{
	return _stream->getFloat(32);
}


/***********************************************************
data will be stored as a string of dynamic dimension
the size of the string should not be larger than 250 characters
***********************************************************/
void ZoidSerializer::addString(const std::string &data, const std::string &id)
{
	_stream->addString(data.c_str());
}

std::string ZoidSerializer::getString(const std::string &id)
{
	char buff[250];
	_stream->getString(buff, 250);
	return buff;
}
