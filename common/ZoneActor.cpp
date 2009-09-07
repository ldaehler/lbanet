#include "ZoneActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

/***********************************************************
	Constructor
***********************************************************/
ZoneActor::ZoneActor(float zoneSizeX, float zoneSizeY, float zoneSizeZ)
: _zoneSizeX(zoneSizeX), _zoneSizeY(zoneSizeY), _zoneSizeZ(zoneSizeZ), _activated(false)
{
	
}

/***********************************************************
	Destructor
***********************************************************/
ZoneActor::~ZoneActor()
{

}


/***********************************************************
check zone activation
***********************************************************/
int ZoneActor::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation)
{
	float posX = GetZoneCenterX();
	float posY = GetZoneCenterY();
	float posZ = GetZoneCenterZ();


	if( (PlayerPosX >= (posX-_zoneSizeX) && PlayerPosX < (posX+_zoneSizeX)) && 
		(PlayerPosY >= (posY)				&& PlayerPosY < (posY+_zoneSizeY)) &&
		(PlayerPosZ >= (posZ-_zoneSizeZ) && PlayerPosZ < (posZ+_zoneSizeZ)))
	{
		if(!_activated)
		{
			if(DirectActivation)
				ProcessActivation(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation);
			_activated = true;
			return 1;
		}

		return 0;
	}

	if(_activated)
	{
		if(DirectActivation)
			ProcessDesactivation(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation);
		_activated = false;
		return -1;
	}
		

	return 0;
}


/***********************************************************
render editor part
***********************************************************/
void ZoneActor::RenderEditor()
{
#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(GetZoneCenterX(), GetZoneCenterY()/2. + 0.5, GetZoneCenterZ());
	glColor4f(0.4f,0.4f,1.0f, 1.f);

	glBegin(GL_LINES);

		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);

		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);

		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Actor::RenderEditor();
#endif
}

