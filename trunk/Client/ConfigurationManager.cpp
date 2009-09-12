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

#include "ConfigurationManager.h"
#include <IceUtil/StaticMutex.h>
#include <IceUtil/IceUtil.h>
#include "LogHandler.h"
#include <libconfig.h++>

ConfigurationManager* ConfigurationManager::_singleton_instance = 0;
static IceUtil::StaticMutex myStaticMutex = ICE_STATIC_MUTEX_INITIALIZER;


/***********************************************************
constructor
***********************************************************/
ConfigurationManager::ConfigurationManager()
: _opened(false)
{
	_configH = new libconfig::Config();
	try
	{
		_configH->readFile("Data/Preferences.cfg");
		_opened = true;
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile("Can not open file Preferences.cfg");
	}
	catch(libconfig::ParseException & ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not correctly parse Preferences.cfg: ") + ex.what());
	}
}


/***********************************************************
destructor
***********************************************************/
ConfigurationManager::~ConfigurationManager()
{
	delete _configH;
}


/***********************************************************
get singleton instance
***********************************************************/
ConfigurationManager *	ConfigurationManager::GetInstance()
{
	if(!_singleton_instance)
		_singleton_instance = new ConfigurationManager();

	return _singleton_instance;
}




/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetBool(const std::string & path, bool &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);

}

/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetInt(const std::string & path, int &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);
}

/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetLong(const std::string & path, long &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);
}

/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetFloat(const std::string & path, float &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);
}

/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetDouble(const std::string & path, double &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);
}

/***********************************************************
get functions
***********************************************************/
bool ConfigurationManager::GetString(const std::string & path, std::string &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	return _configH->lookupValue(path, res);
}


/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetBool(const std::string & path, const bool &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}


	return ret;
}

/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetInt(const std::string & path, const int &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}

	return ret;
}

/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetLong(const std::string & path, const long &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}

	return ret;
}

/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetFloat(const std::string & path, const float &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}

	return ret;
}

/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetDouble(const std::string & path, const double &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}

	return ret;
}

/***********************************************************
set functions
***********************************************************/
bool ConfigurationManager::SetString(const std::string & path, const std::string &res)
{
	IceUtil::StaticMutex::Lock lock(myStaticMutex);
	if(!_opened)
		return false;

	bool ret = false;

	try
	{
		_configH->lookup(path) = res;
		_configH->writeFile("Data/Preferences.cfg");
		ret = true;
	}
	catch(libconfig::SettingNotFoundException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not find preferences setting: ") + path);
	}
	catch(libconfig::SettingTypeException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not set the preferences setting: ") + path);
	}
	catch(libconfig::FileIOException)
	{
		LogHandler::getInstance()->LogToFile(std::string("Can not write the preferences into file"));
	}

	return ret;
}

