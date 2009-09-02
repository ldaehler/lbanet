#if !defined(__Agent_shared_data_h)
#define __Agent_shared_data_h

#include <string>
#include <vector>
#include <map>

#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>
#include <Ice/Config.h>

/***********************************************************************
 * Module:  SharedData.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class SharedData
 ***********************************************************************/
class SharedData : public IceUtil::Mutex
{
public:
	//! default construtor
	SharedData();

	//! get player id
	Ice::Long GetId(const std::string & PlayerName);

	//! check if user already logged in
	//! if not log him in
	bool TryLogin(const std::string & PlayerName);

	//! get connected list
	const std::map<std::string, Ice::Long> & GetConnected();

	//! disconnect player
	bool Disconnect(Ice::Long playerid);

protected:
	SharedData(const SharedData &);
	const SharedData & operator=(const SharedData &);

private:
	Ice::Long								m_current_id;
	std::map<std::string, Ice::Long>		m_connected_users;

};

#endif