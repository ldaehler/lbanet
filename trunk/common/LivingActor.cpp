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


#include "LivingActor.h"


#ifndef _LBANET_SERVER_SIDE_
#include "TextWritter.h"
#include "CharacterRenderer.h"
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

#ifndef _LBANET_SET_EDITOR_
#define _LBANET_SET_EDITOR_
#endif


/***********************************************************
	Constructor
***********************************************************/
LivingActor::LivingActor(float animationSpeed)
: _MaxLife(10), _MaxMana(10), _CurrentLife(10), _CurrentMana(10)
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
		std::string dispname = _Name;

		glEnable(GL_DEPTH_TEST);
		{
		double X = _posX + 0.5;
		double Y = _posY + GetTextHeight();
		double Z = _posZ + 0.5;

		double xOffset = dispname.length() / 8.0;
		double zOffset = dispname.length() / 8.0;

		glPushMatrix();
		glTranslated(X-xOffset,Y/2,Z+zOffset);
		glRotatef( 45, 0.0, 1.0, 0.0 );
		glRotatef( -30, 1.0, 0.0, 0.0 );
		glScalef(0.04f, 0.04f, 0.04f);




		#ifdef _LBANET_SET_EDITOR_
		std::stringstream strs;
		strs<<dispname<<" X:"<<_posX<<" Y:"<<_posY<<" Z:"<<_posZ;
		TextWritter::getInstance()->glPrintText(strs.str(), 0);
		#else
		TextWritter::getInstance()->glPrintText(dispname, 0, _nameR, _nameG, _nameB);
		#endif


		glPopMatrix();
		}
		glDisable(GL_DEPTH_TEST);
		{
		double X = _posX + 0.5;
		double Y = _posY + GetTextHeight();
		double Z = _posZ + 0.5;

		double xOffset = dispname.length() / 8.0;
		double zOffset = dispname.length() / 8.0;



		glPushMatrix();
		glTranslated(X-xOffset,Y/2,Z+zOffset);
		glRotatef( 45, 0.0, 1.0, 0.0 );
		glRotatef( -30, 1.0, 0.0, 0.0 );
		glScalef(0.04f, 0.04f, 0.04f);


		#ifdef _LBANET_SET_EDITOR_
		std::stringstream strs;
		strs<<dispname<<" X:"<<_posX<<" Y:"<<_posY<<" Z:"<<_posZ;
		TextWritter::getInstance()->glPrintText(strs.str(), 0);
		#else
		TextWritter::getInstance()->glPrintText(dispname, 0, _nameR, _nameG, _nameB);
		#endif

		glPopMatrix();
		}
		glEnable(GL_DEPTH_TEST);


		float sizebar=20;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslated(_posX, (_posY - 1)/2.0, _posZ);
		glRotatef( 45, 0.0, 1.0, 0.0 );
		glRotatef( -30, 1.0, 0.0, 0.0 );
		glScalef(0.04f, 0.04f, 0.04f);
		glBegin(GL_QUADS);

			glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

			glVertex2f(-sizebar,0);					
			glVertex2f(sizebar,0);					
			glVertex2f(sizebar,4);					
			glVertex2f(-sizebar,4);	

			glColor3f(11/255.f, 11/255.f, 71/255.f);
			glVertex2f(-sizebar,0);					
			glVertex2f(-sizebar+(sizebar*2*_CurrentMana/_MaxMana),0);	
			glColor3f(13/255.f, 12/255.f, 150/255.f);
			glVertex2f(-sizebar+(sizebar*2*_CurrentMana/_MaxMana),4);					
			glVertex2f(-sizebar,4);	


			glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
			glVertex2f(-sizebar,6);					
			glVertex2f(sizebar,6);					
			glVertex2f(sizebar,10);					
			glVertex2f(-sizebar,10);	

			glColor3f(115/255.f, 0.f, 2/255.f);
			glVertex2f(-sizebar,6);					
			glVertex2f(-sizebar+(sizebar*2*_CurrentLife/_MaxLife),6);	
			glColor3f(254/255.f, 0.f, 3/255.f);
			glVertex2f(-sizebar+(sizebar*2*_CurrentLife/_MaxLife),10);					
			glVertex2f(-sizebar,10);	
		glEnd();

		float lastwidth;
		glGetFloatv(GL_LINE_WIDTH, &lastwidth);
		glLineWidth(1.0);


		glBegin(GL_LINES);
			glColor4f(0.f, 104/255.f, 107/255.f, 0.7f);
			glVertex2f(-sizebar,0);			
			glVertex2f(sizebar,0);
			glVertex2f(sizebar,0);
			glVertex2f(sizebar,4);
			glColor4f(115/255.f, 252/255.f, 252/255.f, 0.7f);
			glVertex2f(sizebar,4);
			glVertex2f(-sizebar,4);	
			glVertex2f(-sizebar,4);	
			glVertex2f(-sizebar,0);
		glEnd();


		glBegin(GL_LINES);
			glColor4f(0.f, 104/255.f, 107/255.f, 0.7f);
			glVertex2f(-sizebar,6);					
			glVertex2f(sizebar,6);		
			glVertex2f(sizebar,6);		
			glVertex2f(sizebar,10);
			glColor4f(115/255.f, 252/255.f, 252/255.f, 0.7f);
			glVertex2f(sizebar,10);	
			glVertex2f(-sizebar,10);	
			glVertex2f(-sizebar,10);	
			glVertex2f(-sizebar,6);		
		glEnd();

		glLineWidth(lastwidth);

		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
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

