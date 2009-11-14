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

static void Trim(std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');

		if(pos != std::string::npos)
			str.erase(0, pos);
	}
	else
		str.clear();

}

static void Tokenize(const std::string& str,
										std::vector<std::string>& tokens,
										const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);



	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		std::string tmp = str.substr(lastPos, pos - lastPos);
		Trim(tmp);
		tokens.push_back(tmp);

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

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
LbaNet::SavedWorldInfo DatabaseHandler::ChangeWorld(const std::string& NewWorldName, long PlayerId)
{
	LbaNet::SavedWorldInfo resP;
	resP.ppos.MapName = "";

	if(!_connected)
		return resP;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "SELECT id, lastmap, lastposx, lastposy, lastposz, lastrotation, InventorySize, Shortcuts FROM usertoworldmap";
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
			
			// player pos part
			resP.ppos.MapName = res[0][1].c_str();
			resP.ppos.X = res[0][2];
			resP.ppos.Y = res[0][3];
			resP.ppos.Z = res[0][4];
			resP.ppos.Rotation = res[0][5];

			// player inventory part
			resP.inventory.InventorySize = res[0][6];

			std::vector<std::string> tokens;
			std::string shortcutstr = res[0][7].c_str();
			Tokenize(shortcutstr, tokens, "#");
			for(size_t i=0; i<tokens.size(); ++i)
				resP.inventory.UsedShorcuts.push_back(atoi(tokens[i].c_str()));

			query.clear();
			query << "SELECT * FROM userinventory";
			query << " WHERE worldid = '"<<res[0][0]<<"'";
			if (mysqlpp::StoreQueryResult res2 = query.store())
			{
				for(size_t i=0; i<res2.size(); ++i)
				{
					LbaNet::InventoryItem itm;
					itm.Number = res2[i][2];
					itm.PlaceInInventory = res2[i][3];
					resP.inventory.InventoryStructure[res2[i][1]] = itm;
				}
			}

		}
		else
		{
			query.clear();
			query << "INSERT usertoworldmap (userid, worldname, lastvisited) VALUES('";
			query << PlayerId << "', '" << NewWorldName<< "', UTC_TIMESTAMP())";
			if(!query.exec())
				std::cout<<"LBA_Server - INSERT usertoworldmap failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

		
			resP.inventory.InventorySize = 30;
			for(int i=0; i<10; ++i)
				resP.inventory.UsedShorcuts.push_back(-1);
		}
	}



	return resP;
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



/***********************************************************
update player inventory structure
***********************************************************/
void DatabaseHandler::UpdateInventory(const LbaNet::InventoryInfo &Inventory, const std::string& WorldName,
									  long PlayerId)
{
	if(!_connected || WorldName == "")
		return;

	std::stringstream shortcutstring;
	LbaNet::ShortcutSeq::const_iterator it = Inventory.UsedShorcuts.begin();
	LbaNet::ShortcutSeq::const_iterator end = Inventory.UsedShorcuts.end();
	if(it != end)
	{
		shortcutstring<<*it;
		++it;
	}
	for(;it != end; ++it)
		shortcutstring<<"#"<<*it;


	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "SELECT id FROM usertoworldmap";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldname = '"<<WorldName<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "UPDATE usertoworldmap SET InventorySize = '"<<Inventory.InventorySize<<"',";
			query << "Shortcuts = '"<<shortcutstring.str()<<"' ";
			query << " WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"LBA_Server - Update usertoworldmap_inv failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;


			query.clear();
			query << "DELETE FROM userinventory";
			query << " WHERE worldid = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"LBA_Server - Update DELETE failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

			LbaNet::InventoryMap::const_iterator iti = Inventory.InventoryStructure.begin();
			LbaNet::InventoryMap::const_iterator endi = Inventory.InventoryStructure.end();
			for(;iti != endi; ++iti)
			{
				query.clear();
				query << "INSERT INTO userinventory (worldid, objectid, number, InventoryPlace) VALUES('";
				query << res[0][0] << "', '" << iti->first << "', '" << iti->second.Number << "', '" << iti->second.PlaceInInventory << "')";
				if(!query.exec())
					std::cout<<"LBA_Server - Update INSERT usertoworldmap failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

			}
		}
	}
}

/***********************************************************
add friend function
***********************************************************/
void DatabaseHandler::AddFriend(long myId, const std::string&  name)
{
	if(!_connected)
		return;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);

	query << "SELECT id FROM users";
	query << " WHERE username = '"<<name<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "INSERT INTO friends (userid, friendid) VALUES('";
			query << myId << "', '" << res[0][0] << "')";
			if(!query.exec())
				std::cout<<"LBA_Server - Update INSERT friends failed for user id "<<myId<<" : "<<query.error()<<std::endl;
		}
	}
}

/***********************************************************
remove friend function
***********************************************************/
void DatabaseHandler::RemoveFriend(long myId, const std::string&  name)
{
	if(!_connected)
		return;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);

	query << "SELECT id FROM users";
	query << " WHERE username = '"<<name<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "DELETE FROM friends";
			query << " WHERE userid = '"<<myId<<"' AND friendid = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"LBA_Server - Update DELETE friends failed for user id "<<myId<<" : "<<query.error()<<std::endl;
		}
	}
}

/***********************************************************
get friends function
***********************************************************/
LbaNet::FriendsSeq DatabaseHandler::GetFriends(long myId)
{
	LbaNet::FriendsSeq resF;

	if(!_connected)
		return resF;

	Lock sync(*this);
	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);

	query << "SELECT users.username FROM users, friends";
	query << " WHERE users.id = friends.friendid";
	query << " AND friends.userid = '"<<myId<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			for(size_t i=0; i<res.size(); ++i)
				resF.push_back(res[i][0].c_str());
		}
	}
	else
	{
		std::cout<<query.error()<<std::endl;
	}

	return resF;
}
