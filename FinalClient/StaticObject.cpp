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

#include "StaticObject.h"


/***********************************************************
constructor
***********************************************************/
StaticObject::StaticObject(boost::shared_ptr<PhysicalObjectHandlerBase> phH,
							boost::shared_ptr<DisplayObjectHandlerBase> disH)
	: DynamicObject(phH, disH)
{
	if(_phH && _disH)
		StraightSync();
}

/***********************************************************
destructor
***********************************************************/
StaticObject::~StaticObject()
{
}


/***********************************************************
directly synchronize value between physic and display
***********************************************************/
void StaticObject::StraightSync()
{
	float posX, posY, posZ;
	LbaQuaternion Q;

	// get value from physic object
	_phH->GetPosition(posX, posY, posZ);
	_phH->GetRotation(Q);

	// set it to display object
	_disH->SetPosition(posX, posY, posZ);
	_disH->SetRotation(Q);

}
