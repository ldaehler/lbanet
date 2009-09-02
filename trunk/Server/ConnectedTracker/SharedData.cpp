#include "SharedData.h"


/***********************************************************
constructor
***********************************************************/
SharedData::SharedData()
: m_current_id(1)
{

}

/***********************************************************
get player id
***********************************************************/
Ice::Long SharedData::GetId(const std::string & PlayerName)
{
	Lock sync(*this);
	std::map<std::string, Ice::Long>::iterator it = m_connected_users.find(PlayerName);
	if(it != m_connected_users.end())
		return it->second;

	return -1;
}

/***********************************************************
check if user already logged in
if not log him in
***********************************************************/
bool SharedData::TryLogin(const std::string & PlayerName)
{
	Lock sync(*this);

	std::map<std::string, Ice::Long>::iterator it = m_connected_users.find(PlayerName);
	if(it != m_connected_users.end())
		return false;

	m_connected_users[PlayerName] = m_current_id;
	++m_current_id;

	std::cout<<IceUtil::Time::now().toDateTime()<<": "<<PlayerName<<" connected"<<std::endl;
	return true;
}

/***********************************************************
get connected list
***********************************************************/
const std::map<std::string, Ice::Long> & SharedData::GetConnected()
{
	Lock sync(*this);
	return m_connected_users;
}

/***********************************************************
disconnect player
***********************************************************/
bool SharedData::Disconnect(Ice::Long playerid)
{
	Lock sync(*this);

	std::map<std::string, Ice::Long>::iterator it = m_connected_users.begin();
	std::map<std::string, Ice::Long>::iterator end = m_connected_users.end();

	for(; it != end; ++it)
	{
		if(it->second == playerid)
		{
			std::cout<<IceUtil::Time::now().toDateTime()<<": "<<it->first<<" disconnected"<<std::endl;
			m_connected_users.erase(it);
			return true;
		}
	}

	return false;
}