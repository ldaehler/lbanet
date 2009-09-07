#include "LivingActor.h"


#ifndef _LBANET_SERVER_SIDE_
#include "TextWritter.h"
#include "CharacterRenderer.h"
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

//#ifndef _LBANET_SET_EDITOR_
//#define _LBANET_SET_EDITOR_
//#endif


/***********************************************************
	Constructor
***********************************************************/
LivingActor::LivingActor(float animationSpeed)
{
#ifndef _LBANET_SERVER_SIDE_
	_Renderer = new CharacterRenderer(animationSpeed);
#endif
}

/***********************************************************
	Destructor
***********************************************************/
LivingActor::~LivingActor()
{

}

/***********************************************************
	render object
***********************************************************/
void LivingActor::Render(int RoomCut)
{
#ifndef _LBANET_SERVER_SIDE_
	Actor::Render(RoomCut);
	
	if(_DisplayName && _Name != "")
	{
		glEnable(GL_DEPTH_TEST);
		{
		double X = _posX + 0.5;
		double Y = _posY + GetTextHeight();
		double Z = _posZ + 0.5;

		double xOffset = _Name.length() / 8.0;
		double zOffset = _Name.length() / 8.0;

		glPushMatrix();
		glTranslated(X-xOffset,Y/2,Z+zOffset);
		glRotatef( 40, 0.0, 1.0, 0.0 );
		glScalef(0.04f, 0.04f, 0.04f);




		#ifdef _LBANET_SET_EDITOR_
		std::stringstream strs;
		strs<<_Name<<" X:"<<_posX<<" Y:"<<_posY<<" Z:"<<_posZ;
		TextWritter::getInstance()->glPrintText(strs.str(), 0);
		#else
		TextWritter::getInstance()->glPrintText(_Name, 0);
		#endif


		glPopMatrix();
		}
		glDisable(GL_DEPTH_TEST);
		{
		double X = _posX + 0.5;
		double Y = _posY + GetTextHeight();
		double Z = _posZ + 0.5;

		double xOffset = _Name.length() / 8.0;
		double zOffset = _Name.length() / 8.0;



		glPushMatrix();
		glTranslated(X-xOffset,Y/2,Z+zOffset);
		glRotatef( 40, 0.0, 1.0, 0.0 );
		glScalef(0.04f, 0.04f, 0.04f);


		#ifdef _LBANET_SET_EDITOR_
		std::stringstream strs;
		strs<<_Name<<" X:"<<_posX<<" Y:"<<_posY<<" Z:"<<_posZ;
		TextWritter::getInstance()->glPrintText(strs.str(), 0);
		#else
		TextWritter::getInstance()->glPrintText(_Name, 0);
		#endif



		glPopMatrix();
		}
		glEnable(GL_DEPTH_TEST);
	}
#endif
}

/***********************************************************
	do all check to be done when idle
***********************************************************/
int LivingActor::Process(double tnow, float tdiff)
{
	return Actor::Process(tnow, tdiff);
} 


/***********************************************************
get the place where to draw the text
***********************************************************/
float LivingActor::GetTextHeight()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
	{
		int model = static_cast<CharacterRenderer *>(_Renderer)->GetModel();

		if(model == 47)
			return 12;

		if(model == 64)
			return 11.5f;

		if(model == 4)
			return 9.5;
	}
#endif



	return 9;
}



/***********************************************************
change the actor to display
***********************************************************/
void LivingActor::changeAnimEntity(int entityNum, int bodyNum)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		static_cast<CharacterRenderer *>(_Renderer)->changeAnimEntity(entityNum, bodyNum);
#endif
}


/***********************************************************
set the current animation to be played
***********************************************************/
void LivingActor::setActorAnimation(int animNb)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		static_cast<CharacterRenderer *>(_Renderer)->setActorAnimation(animNb);
#endif
}


/***********************************************************
accessors
***********************************************************/
int LivingActor::GetModel()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return static_cast<CharacterRenderer *>(_Renderer)->GetModel();
#endif

	return 0;
}

/***********************************************************
accessors
***********************************************************/
int LivingActor::GetBody()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return static_cast<CharacterRenderer *>(_Renderer)->GetBody();
#endif

	return 0;
}

/***********************************************************
accessors
***********************************************************/
int LivingActor::GetAnimation()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return static_cast<CharacterRenderer *>(_Renderer)->GetAnimation();
#endif

	return 0;
}

/***********************************************************
accessors
***********************************************************/
int LivingActor::getKeyframe()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return static_cast<CharacterRenderer *>(_Renderer)->getKeyframe();
#endif

	return 0;
}


/***********************************************************
set body color
***********************************************************/
void LivingActor::SetBodyColor(short newColor)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		static_cast<CharacterRenderer *>(_Renderer)->SetBodyColor(newColor);
#endif
}

/***********************************************************
get body color
***********************************************************/
short LivingActor::GetBodyColor()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return static_cast<CharacterRenderer *>(_Renderer)->GetBodyColor();
#endif

	return -1;
}

