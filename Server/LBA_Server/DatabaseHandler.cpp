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
#include <IceUtil/Time.h>

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


static std::string replaceall(const std::string & str, const std::string & toreplace, const std::string & with)
{
	std::string res = str;

    int len = toreplace.size(), pos;
	while((pos=res.find(toreplace)) != std::string::npos)
    {
        res=res.substr(0,pos)+with+res.substr(pos+len); 
    }

	return res;
} 


/***********************************************************
constructor
***********************************************************/
DatabaseHandler::DatabaseHandler(const std::string db, const std::string server,
									const std::string user, const std::string password)
				: _mysqlH(NULL), _db(db), _server(server), _user(user), _password(password)
{

	Connect();
}



/***********************************************************
clear db connection
***********************************************************/
void DatabaseHandler::Clear()
{
	if(_mysqlH)
	{
		if(_mysqlH->connected())
			_mysqlH->disconnect();

		delete _mysqlH;
		_mysqlH = NULL;
	}
}

/***********************************************************
connect to database
***********************************************************/
void DatabaseHandler::Connect()
{
	try
	{
		Clear();
		_mysqlH = new mysqlpp::Connection(false);

		if (!_mysqlH->connect(_db.c_str(), _server.c_str(), _user.c_str(), _password.c_str()))
		{
			std::cerr<<IceUtil::Time::now()<<": LBA_Server - DB connection failed: " << _mysqlH->error() << std::endl;
		}
	}
	catch(...){}
}


/***********************************************************
player has changed world
***********************************************************/
LbaNet::SavedWorldInfo DatabaseHandler::ChangeWorld(const std::string& NewWorldName, long PlayerId)
{
	long worldid = -1;
	LbaNet::SavedWorldInfo resP;
	resP.ppos.MapName = "";
	resP.CurrentLife = -1;
	resP.CurrentMana = -1;
	resP.MaxLife = -1;
	resP.MaxMana = -1;

	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return resP;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT uw.id, uw.lastmap, uw.lastposx, uw.lastposy, uw.lastposz, uw.lastrotation, uw.InventorySize, uw.Shortcuts, uw.LifePoint, uw.ManaPoint, uw.MaxLife, uw.MaxMana, w.id";
	query << " FROM lba_usertoworld uw, lba_worlds w";
	query << " WHERE uw.userid = '"<<PlayerId<<"'";
	query << " AND w.name = '"<<NewWorldName<<"'";
	query << " AND uw.worldid = w.id";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			//set the user as connected
			query.clear();
			query << "UPDATE lba_usertoworld SET lastvisited = UTC_TIMESTAMP() WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update usertoworldmap.lastvisited failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
			
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

			resP.CurrentLife = res[0][8];
			resP.CurrentMana = res[0][9];
			resP.MaxLife = res[0][10];
			resP.MaxMana = res[0][11];

			worldid = res[0][12];

			query.clear();
			query << "SELECT * FROM lba_inventory";
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
			query <<"SELECT id FROM lba_worlds WHERE name = '"<<NewWorldName<<"'";
			if (mysqlpp::StoreQueryResult res = query.store())
			{
				if(res.size() > 0)
				{
					worldid = res[0][0];

					query.clear();
					query << "INSERT lba_usertoworld (userid, worldid, lastvisited) VALUES('";
					query << PlayerId << "', '"<<worldid<<"', UTC_TIMESTAMP())";
					if(!query.exec())
						std::cerr<<IceUtil::Time::now()<<": LBA_Server - INSERT lba_usertoworld failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
				}
			}


		
			resP.inventory.InventorySize = 30;
			for(int i=0; i<10; ++i)
				resP.inventory.UsedShorcuts.push_back(-1);
		}

		//insert world name into user
		{
			query.clear();
			query << "UPDATE lba_users SET currentworldid = '"<<worldid<<"' WHERE id = '"<<PlayerId<<"'";
			if(!query.exec())
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - UPDATE users failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
		}
	}
	else
	{
		Clear();
	}



	return resP;
}


/***********************************************************
player update his current position in the world
***********************************************************/
void DatabaseHandler::UpdatePositionInWorld(const LbaNet::PlayerPosition& Position, 
											const std::string& WorldName, long PlayerId)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "UPDATE lba_usertoworld SET lastmap = '"<<Position.MapName<<"',";
	query << "lastposx = '"<<Position.X<<"',";
	query << "lastposy = '"<<Position.Y<<"',";
	query << "lastposz = '"<<Position.Z<<"',";
	query << "lastrotation = '"<<Position.Rotation<<"'";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update UpdatePositionInWorld failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
		Clear();
	}
	
}



/***********************************************************
quit current world
***********************************************************/
void DatabaseHandler::QuitWorld(const std::string& LastWorldName,long PlayerId,
								float currentlife, float currentmana, float maxlife, float maxmana)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	// quit previous world
	if(LastWorldName != "")
	{
		mysqlpp::Query query(_mysqlH, false);
		query << "UPDATE lba_usertoworld SET timeplayedmin = timeplayedmin + TIMESTAMPDIFF(MINUTE, lastvisited, UTC_TIMESTAMP())";
		query << ", LifePoint = '"<<currentlife<<"'";
		query << ", ManaPoint = '"<<currentmana<<"'";
		query << ", MaxLife = '"<<maxlife<<"'";
		query << ", MaxMana = '"<<maxmana<<"'";
		query << " WHERE userid = '"<<PlayerId<<"'";
		query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<LastWorldName<<"')";	
		if(!query.exec())
		{
			std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update usertoworldmap.timeplayedmin failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;		
			Clear();
		}
	}
}



/***********************************************************
update player inventory structure
***********************************************************/
void DatabaseHandler::UpdateInventory(const LbaNet::InventoryInfo &Inventory, const std::string& WorldName,
									  long PlayerId)
{
	if(WorldName == "")
		return;

	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

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


	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT id FROM lba_usertoworld";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";;
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "UPDATE lba_usertoworld SET InventorySize = '"<<Inventory.InventorySize<<"',";
			query << "Shortcuts = '"<<shortcutstring.str()<<"' ";
			query << " WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"LBA_Server - Update usertoworldmap_inv failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;


			query.clear();
			query << "DELETE FROM lba_inventory";
			query << " WHERE worldid = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update DELETE failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

			LbaNet::InventoryMap::const_iterator iti = Inventory.InventoryStructure.begin();
			LbaNet::InventoryMap::const_iterator endi = Inventory.InventoryStructure.end();
			for(;iti != endi; ++iti)
			{
				query.clear();
				query << "INSERT INTO lba_inventory (worldid, objectid, number, InventoryPlace) VALUES('";
				query << res[0][0] << "', '" << iti->first << "', '" << iti->second.Number << "', '" << iti->second.PlaceInInventory << "')";
				if(!query.exec())
					std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update INSERT usertoworldmap failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

			}
		}
	}
	else
	{
		Clear();
	}
}

/***********************************************************
add friend function
***********************************************************/
bool DatabaseHandler::AskFriend(long myId, const std::string &friendname)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return false;
		}
	}

	mysqlpp::Query query(_mysqlH, false);

	query << "SELECT id FROM jos_users";
	query << " WHERE username = '"<<friendname<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			long juid = res[0][0];

			// get my jos id
			query.clear();
			query << "SELECT ju.id FROM jos_users ju, lba_users lu";
			query << " WHERE ju.id = lu.josiid AND lu.id = '"<<myId<<"'";
			if (mysqlpp::StoreQueryResult res = query.store())
			{
				if(res.size() > 0)
				{
					myId = res[0][0];

					//check if there is no existing connexion - if it is the case then do nothing
					query.clear();
					query << "SELECT accepted FROM jos_comprofiler_members";
					query << " WHERE referenceid = '"<<myId<<"'";
					query << " AND memberid = '"<<juid<<"'";
					if (mysqlpp::StoreQueryResult res = query.store())
					{
						if(res.size() > 0)
							return false;
					}

					// else add the connexion
					query.clear();
					query << "INSERT INTO jos_comprofiler_members (referenceid, memberid, accepted, pending, membersince, reason, type) VALUES";
					query << "('" << myId << "', '" << juid << "', '1', '1', NOW(), 'Friend request from Lbanet', 'Friend')";
					query << ",('" << juid << "', '" << myId << "', '0', '0', NOW(), 'Friend request from Lbanet', 'Friend')";
					if(!query.exec())
					{
						std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update INSERT friends failed for user id "<<myId<<" : "<<query.error()<<std::endl;
						return false;
					}

					return true;
				}
			}
		}
	}
	else
	{
		Clear();
	}

	return false;
}

/***********************************************************
accept friend invitation
***********************************************************/
bool DatabaseHandler::AcceptFriend(long myId, long friendid, std::string &friendname)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return false;
		}
	}

	mysqlpp::Query query(_mysqlH, false);

	query << "SELECT ju.id, ju.username FROM jos_users ju, lba_users lu";
	query << " WHERE ju.id = lu.josiid AND lu.id = '"<<friendid<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			long juid = res[0][0];
			friendname = res[0][1];

			// get my jos id
			query.clear();
			query << "SELECT ju.id FROM jos_users ju, lba_users lu";
			query << " WHERE ju.id = lu.josiid AND lu.id = '"<<myId<<"'";
			if (mysqlpp::StoreQueryResult res = query.store())
			{
				if(res.size() > 0)
				{
					myId = res[0][0];

					// check if we are in pending state
					query.clear();
					query << "SELECT accepted, pending FROM jos_comprofiler_members";
					query << " WHERE (referenceid = '"<<myId<<"' AND memberid = '"<<juid<<"')";
					if (res = query.store())
					{
						if(res.size() > 0)
						{
							int acpt= res[0][0];
							int pend= res[0][1];
							if(acpt == 0 && pend == 0)
							{
								query.clear();
								query << "UPDATE jos_comprofiler_members SET accepted = '1', pending = '0' ";
								query << " WHERE (referenceid = '"<<myId<<"' AND memberid = '"<<juid<<"')";
								query << " OR (referenceid = '"<<juid<<"' AND memberid = '"<<myId<<"')";
								if(!query.exec())
									std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update UPDATE friends failed for user id "<<myId<<" : "<<query.error()<<std::endl;
							
							
								return true;
							}
						}
					}
				}	
			}
		}
	}
	else
	{
		Clear();
	}

	return false;
}

/***********************************************************
remove friend function
***********************************************************/
void DatabaseHandler::RemoveFriend(long myId, long friendid)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);

	query << "SELECT ju.id FROM jos_users ju, lba_users lu";
	query << " WHERE ju.id = lu.josiid AND lu.id = '"<<friendid<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			long juid = res[0][0];

			// get my jos id
			query.clear();
			query << "SELECT ju.id FROM jos_users ju, lba_users lu";
			query << " WHERE ju.id = lu.josiid AND lu.id = '"<<myId<<"'";
			if (mysqlpp::StoreQueryResult res = query.store())
			{
				if(res.size() > 0)
				{
					myId = res[0][0];

					query.clear();
					query << "DELETE FROM jos_comprofiler_members";
					query << " WHERE (referenceid = '"<<myId<<"' AND memberid = '"<<juid<<"')";
					query << " OR (referenceid = '"<<juid<<"' AND memberid = '"<<myId<<"')";
					if(!query.exec())
						std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update DELETE friends failed for user id "<<myId<<" : "<<query.error()<<std::endl;
				}	
			}
		}
	}
	else
	{
		Clear();
	}
}

/***********************************************************
get friends function
***********************************************************/
LbaNet::FriendsSeq DatabaseHandler::GetFriends(long myId)
{
	LbaNet::FriendsSeq resF;


	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return resF;
		}
	}

	mysqlpp::Query query(_mysqlH, false);

	query << "SELECT cm.memberid, cm.pending, cm.accepted";
	query << " FROM jos_comprofiler_members cm, jos_users ju, lba_users lu";
	query << " WHERE ju.id = cm.referenceid";
	query << " AND ju.id = lu.josiid";
	query << " AND lu.id = '"<<myId<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			for(size_t i=0; i<res.size(); ++i)
			{
				long juid = res[i][0];
				int pending=res[i][1];
				int accepted=res[i][2];
				query.clear();
				query << "SELECT ju.username, lu.id";
				query << " FROM jos_users ju, lba_users lu";
				query << " WHERE ju.id = '"<<juid<<"'";
				query << " AND ju.id = lu.josiid";

				if (mysqlpp::StoreQueryResult res2 = query.store())
				{
					if(res2.size() > 0)
					{			
						LbaNet::FriendInfo ftmp;
						ftmp.Id = res2[0][1];
						ftmp.Name = res2[0][0];
						ftmp.Pending = (pending == 1);
						ftmp.ToAccept = (accepted == 0);
						resF.push_back(ftmp);
					}
				}
			}
				
		}
	}
	else
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - GetFriends failed: "<<query.error()<<std::endl;
		Clear();
	}

	return resF;
}



/***********************************************************
store letter to the server and return the letter id
***********************************************************/
long DatabaseHandler::AddLetter(long myId, const std::string& title, const std::string& message)
{
	long resF = -1;

	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return resF;
		}
	}

	std::string replaced = replaceall(message, "'", "#quote#");

	mysqlpp::Query query(_mysqlH, false);
	query << "INSERT INTO lba_letters (userid, creationdate, title, message) VALUES('";
	query << myId << "', UTC_TIMESTAMP(), '"<< title <<"', '" << replaced << "')";
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - Update INSERT letters failed for user id "<<myId<<" : "<<query.error()<<std::endl;
		Clear();
	}
	else
	{
		resF = (long) query.insert_id();
	}

	return resF;
}


/***********************************************************
return letter info
***********************************************************/
LbaNet::LetterInfo DatabaseHandler::GetLetterInfo(Ice::Long LetterId)
{
	LbaNet::LetterInfo resF;
	resF.Id = -1;


	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return resF;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT ju.username, l.creationdate, l.title, l.message";
	query << " FROM lba_users u, lba_letters l, jos_users ju";
	query << " WHERE ju.id = u.josiid";
	query << " AND u.id = l.userid";
	query << " AND l.id = '"<<LetterId<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			resF.Id = LetterId;
			resF.Writter = res[0][0].c_str();
			res[0][1].to_string(resF.Date);
			resF.Title= res[0][2].c_str();
			resF.Message= res[0][3].c_str();
			resF.Message = replaceall(resF.Message, "#quote#", "'");
		}
	}
	else
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - GetLetterInfo failed: "<<query.error()<<std::endl;
		Clear();
	}

	return resF;
}



/***********************************************************
get quest information
***********************************************************/
void DatabaseHandler::GetQuestInfo(const std::string& WorldName, long PlayerId, 
					std::vector<long> &questStarted, std::vector<long> &questFinished)
{
	if(WorldName == "")
		return;

	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT id FROM lba_usertoworld";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "SELECT questid, status FROM lba_quests";
			query << " WHERE worldid = '"<<res[0][0]<<"'";
			if (mysqlpp::StoreQueryResult res2 = query.store())
			{
				for(size_t i=0; i<res2.size(); ++i)
				{
					int status = res2[i][1];
					if(status == 0)
						questStarted.push_back(res2[i][0]);

					if(status == 1)
						questFinished.push_back(res2[i][0]);
				}
			}
			else
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - Quest SELECT failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
		}
	}
	else
	{
		Clear();
	}
}


/***********************************************************
set quest information
***********************************************************/
void DatabaseHandler::SetQuestInfo(const std::string& WorldName, long PlayerId, 
					const std::vector<long> &questStarted, const std::vector<long> &questFinished)
{
	if(WorldName == "")
		return;

	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT id FROM lba_usertoworld";
	query << " WHERE userid = '"<<PlayerId<<"'";
	query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "DELETE FROM lba_quests";
			query << " WHERE worldid = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - Quest DELETE failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;

			std::vector<long>::const_iterator iti = questStarted.begin();
			std::vector<long>::const_iterator endi = questStarted.end();
			for(;iti != endi; ++iti)
			{
				query.clear();
				query << "INSERT INTO lba_quests (worldid, questid, status) VALUES('";
				query << res[0][0] << "', '" << *iti << "', '" << 0  << "')";
				if(!query.exec())
					std::cerr<<IceUtil::Time::now()<<": LBA_Server - Quest INSERT failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
			}

			iti = questFinished.begin();
			endi = questFinished.end();
			for(;iti != endi; ++iti)
			{
				query.clear();
				query << "INSERT INTO lba_quests (worldid, questid, status) VALUES('";
				query << res[0][0] << "', '" << *iti << "', '" << 1  << "')";
				if(!query.exec())
					std::cerr<<IceUtil::Time::now()<<": LBA_Server - Quest INSERT failed for user id "<<PlayerId<<" : "<<query.error()<<std::endl;
			}
		}
	}
	else
	{
		Clear();
	}
}


/***********************************************************
record player kill
***********************************************************/
void DatabaseHandler::RecordKill(const std::string& WorldName, long KilledId, int Reason, long KillerId)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	if(WorldName != "")
	{
		mysqlpp::Query query(_mysqlH, false);
		query << "UPDATE lba_usertoworld ";
		switch(Reason)
		{
			case 2:
				query << "SET FallDeath = FallDeath + 1";
			break;
			case 3:
				query << "SET MonsterDeath = MonsterDeath + 1";
			break;
			case 4:
				query << "SET PvpDeath = PvpDeath + 1";
			break;
			case 6:
				query << "SET DrowningDeath = DrowningDeath + 1";
			break;
			default:
				query << "SET OtherDeath = OtherDeath + 1";
			break;
		}
		query << " WHERE userid = '"<<KilledId<<"'";
		query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";	
		if(!query.exec())
		{
			std::cerr<<IceUtil::Time::now()<<": LBA_Server - RecordKill failed for user id "<<KilledId<<" : "<<query.error()<<std::endl;		
			Clear();
		}
	}

	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	if(WorldName != "")
	{
		if(Reason == 4)
		{
			mysqlpp::Query query(_mysqlH, false);
			query << "UPDATE lba_usertoworld ";
			query << "SET PvpKill = PvpKill + 1";
			query << " WHERE userid = '"<<KillerId<<"'";
			query << " AND worldid = (SELECT id FROM lba_worlds WHERE name = '"<<WorldName<<"')";		
			if(!query.exec())
			{
				std::cerr<<IceUtil::Time::now()<<": LBA_Server - RecordKiller failed for user id "<<KillerId<<" : "<<query.error()<<std::endl;		
				Clear();
			}
		}
	}
}

 
/***********************************************************
send a pm to someone
***********************************************************/   
void DatabaseHandler::SendPM(const LbaNet::PMInfo &pm)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "INSERT INTO jos_uddeim (replyid, fromid, toid, message, datum) VALUES(";
	query << "'"<<pm.ReplyId<<"'"; // __________
	query << ",(SELECT id FROM jos_users WHERE username = '"<<pm.FromName<<"')"; 
	query << ",(SELECT id FROM jos_users WHERE username = '"<<pm.ToName<<"')";
	query << ",'"<<pm.Text<<"'";
	query << ", (int)time()"; //UNIX_TIMESTAMP()
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - SendPM failed for user id "<<pm.FromName<<" : "<<query.error()<<std::endl;		
		Clear();
	}
}
 
/***********************************************************
delete a pm
***********************************************************/   
void DatabaseHandler::DeletePM(Ice::Long pmid)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "UPDATE jos_uddeim SET totrash='1', totrashdate=((int)time()) WHERE id = '"<<pmid<<"'"; 
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": LBA_Server - DeletePM failed for id "<<pmid<<" : "<<query.error()<<std::endl;		
		Clear();
	}

}
 
/***********************************************************
get all pm in your mailbox
***********************************************************/   
LbaNet::PMsSeq DatabaseHandler::GetInboxPM()
{
	LbaNet::PMsSeq res;

	return res;
}