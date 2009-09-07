#include "TextActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
TextActor::TextActor(float activationdistance, long textid)
: _textid(textid), ActivableActor(activationdistance)
{

}


/***********************************************************
	Destructor
***********************************************************/
TextActor::~TextActor()
{

}

/***********************************************************
process activation
***********************************************************/
void TextActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGameTextEvent(_textid));
#endif
}