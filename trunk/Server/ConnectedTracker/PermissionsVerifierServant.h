#ifndef _PERMISSION_SERVANT_H
#define _PERMISSION_SERVANT_H

#include <Glacier2/PermissionsVerifier.h>
#include "SharedData.h"

class PermissionsVerifierServant : public Glacier2::PermissionsVerifier
{
public:
	//! constructor
	PermissionsVerifierServant(SharedData * shd);

	virtual bool checkPermissions(const std::string& userId, const std::string& passwd, std::string& reason, 
									const Ice::Current&) const;

private:
	SharedData * _shd;
};

#endif
