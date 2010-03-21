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

#include "LogHandler.h"
#include <boost/date_time/posix_time/posix_time.hpp>


LogHandler* LogHandler::_singletonInstance = NULL;




/***********************************************************
singleton pattern
***********************************************************/
LogHandler * LogHandler::getInstance()
{
	if(!_singletonInstance)
		_singletonInstance = new LogHandler();

	return _singletonInstance;
}


/***********************************************************
Constructor
***********************************************************/
LogHandler::LogHandler()
: _logfile("LBAClient.log", std::ios::app)
{
	_logfile<<std::endl<<std::endl<<"**********************************"<<std::endl;
	_logfile<<boost::posix_time::second_clock::local_time()<<" - Starting..."<<std::endl;
}


/***********************************************************
Destructor
***********************************************************/
LogHandler::~LogHandler()
{

}


/***********************************************************
log a text into file
***********************************************************/
void LogHandler::LogToFile(const std::string text, int category)
{
	boost::mutex::scoped_lock lock(m_mutex);
	_logfile<<boost::posix_time::second_clock::local_time()<<","<<category<<","<<text<<std::endl;
}



/***********************************************************
inform the use of something
***********************************************************/
void LogHandler::InformUser(const std::string text)
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::cout<<text<<std::endl;
}




