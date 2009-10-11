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


#include "DatabaseHandler.h"


/***********************************************************
constructor
***********************************************************/
DatabaseHandler::DatabaseHandler(const std::string db, const std::string server,
									const std::string user, const std::string password)
				: _connected(false), _mysqlH(false)
{
	if (_mysqlH.connect(db.c_str(), server.c_str(), user.c_str(), password.c_str()))
	{
		_connected = true;
	}
	else
	{
		std::cerr << "DB connection failed: " << _mysqlH.error() << std::endl;
		_connected = false;
	}
}


/***********************************************************
player has changed world
***********************************************************/
LbaNet::PlayerPosition DatabaseHandler::ChangeWorld(const std::string& NewWorldName, long PlayerId)
{
	LbaNet::PlayerPosition resPos;
	resPos.MapName = "";

	if(!_connected)
		return resPos;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "SELECT id, lastmap, lastposx, lastposy, lastposz, lastrotation FROM usertoworldmap";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldname = '"<<NewWorldName<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			//set the user as connected
			query.clear();
			query << "UPDATE usertoworldmap SET lastvisited = UTC_TIMESTAMP() WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"LBA_Server - Update usertoworldmap.lastvisited failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
			
			resPos.MapName = res[0][1];
			resPos.X = res[0][2];
			resPos.Y = res[0][3];
			resPos.Z = res[0][4];
			resPos.Rotation = res[0][5];
		}
		else
		{
			query.clear();
			query << "INSERT usertoworldmap (userid, worldname, lastvisited) VALUES('";
			query << PlayerId << "', '" << NewWorldName<< "', UTC_TIMESTAMP())";
			if(!query.exec())
				std::cout<<"LBA_Server - INSERT usertoworldmap failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

		}
	}



	return resPos;
}


/***********************************************************
player update his current position in the world
***********************************************************/
void DatabaseHandler::UpdatePositionInWorld(const LbaNet::PlayerPosition& Position, 
											const std::string& WorldName, long PlayerId)
{
	if(!_connected)
		return;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "UPDATE usertoworldmap SET lastmap = '"<<Position.MapName<<"',";
	query << "lastposx = '"<<Position.X<<"',";
	query << "lastposy = '"<<Position.Y<<"',";
	query << "lastposz = '"<<Position.Z<<"',";
	query << "lastrotation = '"<<Position.Rotation<<"'";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldname = '"<<WorldName<<"'";
	if(!query.exec())
		std::cout<<"LBA_Server - Update UpdatePositionInWorld failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

}



/***********************************************************
quit current world
***********************************************************/
void DatabaseHandler::QuitWorld(const std::string& LastWorldName,long PlayerId)
{
	if(!_connected)
		return;

	// quit previous world
	if(LastWorldName != "")
	{
		Lock sync(*this);
		mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
		query << "UPDATE usertoworldmap SET timeplayedmin = timeplayedmin + TIMESTAMPDIFF(MINUTE, lastvisited, UTC_TIMESTAMP())";
		query << " WHERE userid = '"<<PlayerId<<"'";
		query << " AND worldname = '"<<LastWorldName<<"'";		
		if(!query.exec())
			std::cout<<"LBA_Server - Update usertoworldmap.timeplayedmin failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;		
	}
}
