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
#include "SynchronizedTimeHandler.h"

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
							const ObjectInfo & oinfo, GameClientCallbackBase * callback, 
							PlayerCallbackBase * playerCallback, bool IsMainPlayer)
	: m_myid(myid), m_callback(callback), m_playerCallback(playerCallback)
{

	//inform callback of new actor creation
	if(m_callback)
		m_physicObj = m_callback->AddObject(m_myid, oinfo, IsMainPlayer);

	if(m_playerCallback)
		m_playerCallback->SetPhysicalCharacter(m_physicObj);


	// data replication setup
	{
		m_node->beginReplicationSetup(1);

		// create the movement replicator
		//m_moverep = new ZCom_Replicate_Movement<zFloat, 4>(32, ZCOM_REPFLAG_MOSTRECENT|ZCOM_REPFLAG_SETUPPERSISTS,
		//ZCOM_REPRULE_OWNER_2_AUTH|ZCOM_REPRULE_AUTH_2_PROXY);
		m_moverep3 = new ZCom_Replicate_Movement<zFloat, 3>(32, ZCOM_REPFLAG_MOSTRECENT|ZCOM_REPFLAG_SETUPPERSISTS,
		ZCOM_REPRULE_OWNER_2_AUTH|ZCOM_REPRULE_AUTH_2_PROXY);


		//// set a low error threshold
		//((ZCom_RSetupMovement<zS32>*)m_moverep->getSetup())->setConstantErrorThreshold(1);
		//// set interpolation time to 50 milliseconds
		//((ZCom_RSetupMovement<zS32>*)m_moverep->getSetup())->setInterpolationTime(50);
		//// apply timescale
		//m_moverep->setTimeScale((float)SIM_UPDATE_RATE/1000.0f);

		// set a low error threshold
		((ZCom_RSetupMovement<zS32>*)m_moverep3->getSetup())->setConstantErrorThreshold(1);
		// set interpolation time to 50 milliseconds
		((ZCom_RSetupMovement<zS32>*)m_moverep3->getSetup())->setInterpolationTime(50);
		// apply timescale
		m_moverep3->setTimeScale((float)SIM_UPDATE_RATE/1000.0f);


		// add the replicator
		//m_node->addReplicator(m_moverep, true);
		m_node->addReplicator(m_moverep3, true);

		// set update listener (the methods NObject::inputUpdated, NObject::inputSent, NObject::correctionReceived,
		// and NObject::updateReceived implement the update listener interface and are called by moverep)
		//m_moverep->setUpdateListener(this);
		m_moverep3->setUpdateListener(this);

		// set initial position
		if(m_physicObj)
		{
			//float tmppos[4];
			float tmppos[3];
			m_physicObj->GetPosition(tmppos[0], tmppos[1], tmppos[2]);
			//tmppos[3] = m_physicObj->GetRotationSingleAngle();
			//m_moverep->updateState(tmppos, NULL, NULL, false);
			m_moverep3->updateState(tmppos, NULL, NULL, false);
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


	// only do that on authority
	if(m_node->getRole() == eZCom_RoleAuthority)
	{
		// add announcement data
		ZCom_BitStream *adata = new ZCom_BitStream();
		ZoidSerializer zoids(adata);
		oinfo.Serialize(&zoids);
		m_node->setAnnounceData(adata);


		// change zoidlevel
		m_node->applyForZoidLevel( zoidlevel );
		//m_node->removeFromZoidLevel( 1 );
	}
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
	if(_inputchanged)
	{
		Input in;
		UnpackInputs(in, _inputstream);

		if(m_playerCallback)
		{
			m_playerCallback->inputUpdated(_estimated_time_sent, in);

			if(m_physicObj)
				m_physicObj->RevertBack(_estimated_time_sent);
		}
	}
}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::inputSent(ZCom_BitStream& _inputstream)
{
	// unpack the input again
	Input in;
	UnpackInputs(in, _inputstream);

	// apply it
	m_callback->ApplyInputs(SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync(), in);
}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::correctionReceived(zFloat *_pos, zFloat* _vel, zFloat *_acc, 
										bool _teleport, zU32 _estimated_time_sent)
{
	std::cout<<"received correction "<<_pos[0]<<" "<< _pos[1]<<" "<< _pos[2]<<std::endl;

	if(m_physicObj)
	{
		if(_teleport)
			m_physicObj->SetPosition(_estimated_time_sent, _pos[0], _pos[1], _pos[2]);
		else
			m_physicObj->MoveTo(_estimated_time_sent, _pos[0], _pos[1], _pos[2]);
		
		//m_physicObj->RotateTo(_estimated_time_sent, LbaQuaternion(_pos[3], LbaVec3(0, 1, 0)));
	}
}


/************************************************************************/
/* update listener callback                                     
/************************************************************************/
void PlayerObject::updateReceived(ZCom_BitStream& _inputstream, zFloat *_pos, zFloat* _vel, zFloat *_acc, zU32 _estimated_time_sent)
{
	//std::cout<<"update received "<<_pos[0]<<" "<< _pos[1]<<" "<< _pos[2]<<std::endl;
}

/************************************************************************/
/* do a custom process step if required                                
/************************************************************************/
void PlayerObject::CustomProcess()
{
	switch (m_node->getRole())
	{
		case eZCom_RoleAuthority:
			doAuth(); 
		break;

		case eZCom_RoleOwner:
			doOwner(); 
		break;

		case eZCom_RoleProxy:
			doProxy();
		break;
	}
}



/************************************************************************/
/* do owner process work                       
/************************************************************************/
void PlayerObject::doOwner()
{
	//float tmppos[4] = {0, 0, 0, 0};
	float tmppos[3] = {0, 0, 0};
	if(m_physicObj)
	{
		m_physicObj->GetPosition(tmppos[0], tmppos[1], tmppos[2]);
		//tmppos[3] = m_physicObj->GetRotationSingleAngle();
	}

	// get keyboard input
	Input in = m_callback->GetLastPlayerInput();

	// CHANGED
	// if input didn't change, keep old input
	if (in == m_last_input_sent)
	{
		// give empty input update to movement replicator.
		// this will tell zoidcom to stay with the last input.
		// zoidcom will optimize bandwidth doing this.
		// the current position is needed so that the server
		// can check if a correction is needed
		//m_moverep->updateInput(tmppos,  NULL);
		m_moverep3->updateInput(tmppos,  NULL);
	} 
	else
	// input changed
	{
		// pack input update
		ZCom_BitStream *bs = new ZCom_BitStream(1);
		PackInputs(in, *bs);

		// give input update to movement replicator
		// the current position is needed so that the server
		// can check if a correction is needed
		//m_moverep->updateInput(tmppos,  bs);
		m_moverep3->updateInput(tmppos,  bs);
	}

	 m_last_input_sent = in;
}


/************************************************************************/
/* do proxy process work                    
/************************************************************************/
void PlayerObject::doProxy()
{
	//float tmppos[4] = {0, 0, 0, 0};
	//m_moverep->getExtrapolatedPosition(0, tmppos);
	float tmppos[3] = {0, 0, 0};
	m_moverep3->getExtrapolatedPosition(0, tmppos);


	if(m_physicObj)
	{
		unsigned int ctime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		m_physicObj->MoveTo(ctime, tmppos[0], tmppos[1], tmppos[2]);
		//m_physicObj->RotateTo(ctime, LbaQuaternion(tmppos[3], LbaVec3(0, 1, 0)));
	}

} 


/************************************************************************/
/* do authority process work           
/************************************************************************/
void PlayerObject::doAuth()
{
	//update client view if object position has changed
	if(m_physicObj)
	{
		LbaVec3 position;
		m_physicObj->GetPosition(position.x, position.y, position.z);
		if(!equal(position, m_lastposition))
		{
			float tmppos[3] = {position.x, position.y, position.z};
			m_moverep3->updateState(tmppos, NULL, NULL, false);

			m_lastposition = position;	
		}
	}
}


/************************************************************************/
/* write input to bitstream                           
/************************************************************************/
void PlayerObject::PackInputs(const Input & input, ZCom_BitStream &_str) 
{
	_str.addBool(input.up);
	_str.addBool(input.down);
	_str.addBool(input.left);
	_str.addBool(input.right);
}

/************************************************************************/
/* read input from bitstream                       
/************************************************************************/
void PlayerObject::UnpackInputs(Input & input, ZCom_BitStream &_str) 
{
	input.up = _str.getBool();
	input.down = _str.getBool();
	input.left = _str.getBool();
	input.right = _str.getBool();
}

