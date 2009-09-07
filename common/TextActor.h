#if !defined(__LbaNetModel_1_TextActor_h)
#define __LbaNetModel_1_TextActor_h

#include "ActivableActor.h"


/***********************************************************************
 * Module:  TextActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class TextActor : public ActivableActor
{
public:
	//! constructor
	TextActor(float activationdistance, long textid);

	//! destructor
	virtual ~TextActor();

	//!accessors
	long GetTextId()
	{return _textid;}

	void SetTextId(long id)
	{_textid = id;}

protected:
	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation);

private:
	long _textid;
};

#endif