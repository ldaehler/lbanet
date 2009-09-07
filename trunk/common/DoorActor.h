#if !defined(__LbaNetModel_1_DoorActor_h)
#define __LbaNetModel_1_DoorActor_h

#include "ZoneActor.h"

/***********************************************************************
 * Module:  DoorActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class DoorActor : public ZoneActor
{
public:
	//! constructor
	DoorActor(float zoneSizeX, float zoneSizeY, float zoneSizeZ, bool Locked, long KeyId,
				bool Hide, float OpenTransX, float OpenTransY, float OpenTransZ,
				float OpenTransSpeedX, float OpenTransSpeedY, float OpenTransSpeedZ, bool destroykey);

	//! destructor
	virtual ~DoorActor();

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);

	// used to get the zone center - depend of the actor type
	virtual float GetZoneCenterX(){return _ClosedX;}
	virtual float GetZoneCenterY(){return _ClosedY;}
	virtual float GetZoneCenterZ(){return _ClosedZ;}

	//! called on signal
	virtual bool OnSignal(long SignalNumber);

	// set actor position in the scene
	virtual void SetPosition(float  posX, float  posY, float  posZ);

	//! accessors
	bool GetLocked()
	{return _locked;}
	long GetKeyId()
	{return _KeyId;}
	bool GetDesKey()
	{return _destroykey;}
	bool GetHide()
	{return _Hide;}

	void SetLocked(bool v)
	{_locked = v;}
	void SetKeyId(long v)
	{_KeyId = v;}
	void SetDesKey(bool v)
	{_destroykey = v;}
	void SetHide(bool v)
	{_Hide = v;}

	float GetOpenTransX()
	{return _OpenTransX;}
	float GetOpenTransY()
	{return _OpenTransY;}
	float GetOpenTransZ()
	{return _OpenTransZ;}
	float GetOpenTransSpeedX()
	{return _OpenTransSpeedX;}
	float GetOpenTransSpeedY()
	{return _OpenTransSpeedY;}
	float GetOpenTransSpeedZ()
	{return _OpenTransSpeedZ;}

	void SetOpenTransX(float V)
	{_OpenTransX = V; UpdateCLoseOpen();}
	void SetOpenTransY(float V)
	{_OpenTransY = V; UpdateCLoseOpen();}
	void SetOpenTransZ(float V)
	{_OpenTransZ = V; UpdateCLoseOpen();}
	void SetOpenTransSpeedX(float V)
	{_OpenTransSpeedX = V;}
	void SetOpenTransSpeedY(float V)
	{_OpenTransSpeedY = V;}
	void SetOpenTransSpeedZ(float V)
	{_OpenTransSpeedZ = V;}

	//! render editor part
	virtual void RenderEditor();


	//! get current actor state
	//! return false if the actor is stateless
	virtual bool Getstate(ActorStateInfo & currState);

	//! set the actor state
	virtual void Setstate(const ActorStateInfo & currState);

protected:
	//! process zone activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);
	
	//! process zone desactivation
	virtual void ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);

	//! open the door
	bool Open();

	//! close the door
	bool Close();

	//! update info
	void UpdateCLoseOpen();

private:
	bool		_locked;
	long		_KeyId;
	bool		_destroykey;

	bool		_Hide;
	float		_OpenedX;
	float		_OpenedY;
	float		_OpenedZ;
	float		_ClosedX;
	float		_ClosedY;
	float		_ClosedZ;
	float		_OpenTransX;
	float		_OpenTransY;
	float		_OpenTransZ;
	float		_OpenTransSpeedX;
	float		_OpenTransSpeedY;
	float		_OpenTransSpeedZ;

	int			_opencounter;
	bool		_signalon;

	enum Doorstate { OPENING = 0, OPENED, CLOSING, CLOSED };
	Doorstate	_state;
};

#endif