#include "ZoneActivableActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

/***********************************************************
	Constructor
***********************************************************/
ZoneActivableActor::ZoneActivableActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ)
: _ZoneSizeX(ZoneSizeX), _ZoneSizeY(ZoneSizeY), _ZoneSizeZ(ZoneSizeZ)
{

}



/***********************************************************
	Destructor
***********************************************************/
ZoneActivableActor::~ZoneActivableActor()
{

}

/***********************************************************
activate an actor
***********************************************************/
bool ZoneActivableActor::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
												bool DirectActivation)
{
	float Xbl = _posX-_ZoneSizeX;
	float Ybl = _posY;
	float Zbl = _posZ-_ZoneSizeZ;

	float Xtr = _posX+_ZoneSizeX;
	float Ytr = _posY+_ZoneSizeY;
	float Ztr = _posZ+_ZoneSizeZ;

	if (	(PlayerPosX >= Xbl && PlayerPosX <= Xtr) && 
			(PlayerPosY >= Ybl && PlayerPosY <= Ytr) &&
			(PlayerPosZ >= Zbl && PlayerPosZ <= Ztr)	)  // if the main actor is in zone
	{ 
		if(DirectActivation)
 			ProcessActivation(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation);

		return true;
	}

	return false;
}



/***********************************************************
render editor part
***********************************************************/
void ZoneActivableActor::RenderEditor()
{
#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(_posX, _posY/2. + 0.5, _posZ);
	glColor4f(1.0f,1.0f,0.0f, 1.f);

	glBegin(GL_LINES);

		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Actor::RenderEditor();
#endif
}
