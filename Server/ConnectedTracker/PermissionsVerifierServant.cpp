#include "PermissionsVerifierServant.h"


/***********************************************************
constructor
***********************************************************/
PermissionsVerifierServant::PermissionsVerifierServant(SharedData * shd)
:_shd(shd)
{

}

/***********************************************************
check user permission
***********************************************************/
bool PermissionsVerifierServant::checkPermissions(const std::string& userId, const std::string& passwd, 
												  std::string& reason, const Ice::Current&) const
{
	if(userId == "")
	{
		reason = "Please provide username.";
		return false;
	}

	bool res = _shd->TryLogin(userId);
	if(!res)
		reason = "Username already logged in.";

	return res;
}