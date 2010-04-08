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

#include <zoidcom.h>

#include "PlayerObject.h"
#include "LogHandler.h"
#include "ObjectsDescription.h"
#include "ZoidSerializer.h"
#include "GameClientCallbackBase.h"
#include "PhysicalObjectHandlerBase.h"

// declare static member
ZCom_ClassID PlayerObject::m_classid = ZCom_Invalid_ID;



/************************************************************************/
/* register this class                                        
/************************************************************************/
void PlayerObject::registerClass(ZCom_Control *_control) 
{
	m_classid = _control->ZCom_registerClass("PlayerObject", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


/************************************************************************/
/* constructor                                        
/************************************************************************/
PlayerObject::PlayerObject(ZCom_Control *_control, unsigned int zoidlevel, unsigned int myid, 
						   const ObjectInfo & oinfo, GameClientCallbackBase * callback)
	: m_myid(myid), m_callback(callback)
{

	//inform callback of new actor creation
	if(m_callback)
		m_physicObj = m_callback->AddObject(m_myid, oinfo, false);


	// data replication setup
	{
		m_node->beginReplicationSetup(1);

		// create the movement replicator
		m_moverep = new ZCom_Replicate_Movement<zFloat, 3>(32, ZCOM_REPFLAG_MOSTRECENT|ZCOM_REPFLAG_SETUPPERSISTS,
		ZCOM_REPRULE_OWNER_2_AUTH|ZCOM_REPRULE_AUTH_2_PROXY);
		// set a low error threshold
		((ZCom_RSetupMovement<zS32>*)m_moverep->getSetup())->setConstantErrorThreshold(1);
		// set interpolation time to 50 milliseconds
		((ZCom_RSetupMovement<zS32>*)m_moverep->getSetup())->setInterpolationTime(50);
		// apply timescale
		m_moverep->setTimeScale((float)SIM_UPDATE_RATE/1000.0f);

		// add the replicator
		m_node->addReplicator(m_moverep, true);

		// set update listener (the methods NObject::inputUpdated, NObject::inputSent, NObject::correctionReceived,
		// and NObject::updateReceived implement the update listener interface and are called by moverep)
		m_moverep->setUpdateListener(this);

		// set initial position
		if(m_physicObj)
		{
			float tmppos[3];
			m_physicObj->GetPosition(tmppos[0], tmppos[1], tmppos[2]);
			m_moverep->updateState(tmppos, NULL, NULL, false);
		}

		// done
		m_node->endReplicationSetup();
	}



	#ifndef _ZOID_USED_NEW_VERSION_
		m_node->registerNodeDynamic(m_classid, _control);
	#else
		_control->ZCom_registerDynamicNode( m_node, m_classid );
	#endif

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("New Node " + GetObjectName());
	#endif


	// add announcement data
	ZCom_BitStream *adata = new ZCom_BitStream();
	ZoidSerializer zoids(adata);
	oinfo.Serialize(&zoids);
	m_node->setAnnounceData(adata);


	// change zoidlevel
	m_node->removeFromZoidLevel( 1 );
	m_node->applyForZoidLevel( zoidlevel );
}



/************************************************************************/
/* destructor                                        
/************************************************************************/
PlayerObject::~PlayerObject()
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Deleting node " + GetObjectName());
	#endif

	//inform callback of actor removed
	if(m_callback)
		m_callback->RemObject(m_myid);
}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::inputUpdated(ZCom_BitStream& _inputstream, bool _inputchanged, zU32 _client_time, 
								zU32 _estimated_time_sent)
{

}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::inputSent(ZCom_BitStream& _inputstream)
{

}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::correctionReceived(zS32 *_pos, zFloat* _vel, zFloat *_acc, 
									  bool _teleport, zU32 _estimated_time_sent)
{

}

