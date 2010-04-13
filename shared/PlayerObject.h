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

#ifndef _LBA_NET_PLAYER_OBJECT_H_
#define _LBA_NET_PLAYER_OBJECT_H_

#include "GameObject.h"
#include "CommonTypes.h"

#include <boost/shared_ptr.hpp>
#include <zoidcom.h>


class ObjectInfo;
class GameClientCallbackBase;
class PhysicalObjectHandlerBase;


/***********************************************************************
 * Module:  PlayerObject.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class PlayerObject
 ***********************************************************************/
class PlayerObject : public GameObject, public ZCom_MoveUpdateListener<zFloat>
{

public:
	// constructor for server
	PlayerObject(ZCom_Control *_control, unsigned int zoidlevel, unsigned int myid, const ObjectInfo & oinfo,
					GameClientCallbackBase * callback, bool IsMainPlayer);


	// destructor
	virtual ~PlayerObject();


	// class registration
	static void registerClass(ZCom_Control *_control);
	static unsigned int getClassID() { return m_classid; }


  /* update listener callbacks */
  void inputUpdated(ZCom_BitStream& _inputstream, bool _inputchanged, zU32 _client_time, zU32 _estimated_time_sent);
  void inputSent(ZCom_BitStream& _inputstream);
  void correctionReceived(zFloat *_pos, zFloat* _vel, zFloat *_acc, bool _teleport, zU32 _estimated_time_sent);
  void updateReceived(ZCom_BitStream& _inputstream, zFloat *_pos, zFloat* _vel, zFloat *_acc, zU32 _estimated_time_sent) {}


protected:
	// return the object name
	virtual std::string GetObjectName()
	{return "PlayerObject";}

	// handle init event
	virtual void HandleInitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle init event
	virtual void HandleQuitEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, unsigned int eventconnid){}

	// handle user event
	virtual void HandleUserEvent(ZCom_BitStream * data, eZCom_NodeRole remoterole, 
									unsigned int eventconnid, unsigned int _estimated_time_sent){}

	// do a custom process step if required
	virtual void CustomProcess();


	// write input to bitstream
	void PackInputs(const Input & input, ZCom_BitStream &_str);

	// read input from bitstream
	void UnpackInputs(Input & input, ZCom_BitStream &_str);


	//do owner process work
	void doOwner(); 

	//do proxy process work
	void doProxy(); 

private:
	// the class id for zoidcom
	static unsigned int  m_classid;
	
	unsigned int			m_myid; 
	GameClientCallbackBase * m_callback;
	boost::shared_ptr<PhysicalObjectHandlerBase> m_physicObj;

	ZCom_Replicate_Movement<zFloat, 4> *	m_moverep;

	//keep track of last input sent
	Input	m_last_input_sent;
};


#endif