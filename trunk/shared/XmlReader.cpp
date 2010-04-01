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

#include "XmlReader.h"
#include "LogHandler.h"
#include "tinyxml.h"

/***********************************************************
constructor
***********************************************************/
XmlReader::XmlReader(TiXmlElement*	stream)
: _stream(stream)
{
}


/***********************************************************
function using to build tree structures
***********************************************************/
void XmlReader::startChildObjectList(const std::string &name)
{
	_stream = _stream->FirstChildElement(name);
}

/***********************************************************
function using to build tree structures
***********************************************************/
void XmlReader::nextChildObject(const std::string &name)
{
	_stream = _stream->NextSiblingElement(name);
}

/***********************************************************
function using to build tree structures
***********************************************************/
void XmlReader::finishChildObjectList(const std::string &name)
{
	_stream = _stream->Parent()->ToElement();
}


/***********************************************************
data will be stored as a bool on 1bit
***********************************************************/
void XmlReader::addBool(bool data, const std::string &id)
{
}

void XmlReader::getBool(const std::string &id, bool & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a unsigned short using 8bits
***********************************************************/
void XmlReader::addUShort(unsigned short data, const std::string &id)
{
}

void XmlReader::getUShort(const std::string &id, unsigned short & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a unsigned int using 16bits
***********************************************************/
void XmlReader::addUInt(unsigned int data, const std::string &id)
{
}

void XmlReader::getUInt(const std::string &id, unsigned int & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a unsigned long using 32bits
***********************************************************/
void XmlReader::addULong(unsigned long data, const std::string &id)
{
}

 void XmlReader::getULong(const std::string &id, unsigned long & res)
{
	_stream->QueryValueAttribute(id, &res);
}



/***********************************************************
data will be stored as a signed short using 8bits
***********************************************************/
void XmlReader::addShort(short data, const std::string &id)
{
}

void XmlReader::getShort(const std::string &id, short & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a signed int using 16bits
***********************************************************/
void XmlReader::addInt(int data, const std::string &id)
{
}

void XmlReader::getInt(const std::string &id, int & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a signed long using 32bits
***********************************************************/
void XmlReader::addLong(long data, const std::string &id)
{
}

void XmlReader::getLong(const std::string &id, long & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a float using 32bits
***********************************************************/
void XmlReader::addFloat(float data, const std::string &id)
{
}

void XmlReader::getFloat(const std::string &id, float & res)
{
	_stream->QueryValueAttribute(id, &res);
}


/***********************************************************
data will be stored as a string of dynamic dimension
the size of the string should not be larger than 250 characters
***********************************************************/
void XmlReader::addString(const std::string &data, const std::string &id)
{
}

void XmlReader::getString(const std::string &id, std::string & res)
{
	const std::string* str = _stream->Attribute(id);
	if(str)
		res = *str;
}
