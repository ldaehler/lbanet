#if !defined(__LbaNetModel_1_LivingActor_h)
#define __LbaNetModel_1_LivingActor_h

#include "Actor.h"


/***********************************************************************
 * Module:  LivingActor.h
 * Author:  vivien
 * Modified: mercredi 15 juillet 2009 11:52:16
 * Purpose: Declaration of the class LivingActor
 ***********************************************************************/
class LivingActor : public Actor
{
public:
	//! constructor
	LivingActor(float animationSpeed);

	//! destructor
	virtual ~LivingActor();

	//! render object
	virtual void Render(int RoomCut);

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff); 

	// set the actor name
	void DisplayName(bool display)
	{ _DisplayName = display; }
	void SetName(const std::string & name)
	{ _Name = name; }

	//! change the actor to display
	void changeAnimEntity(int entityNum, int bodyNum);

	//! set the current animation to be played
	void setActorAnimation(int animNb);

	// accessors
	int GetModel();
	int GetBody();
	int GetAnimation();
	int getKeyframe();
	std::string GetName()
	{return _Name;}
	bool GetDisplayName()
	{return _DisplayName;}

	//! set body color
	void SetBodyColor(short newColor);

	//! get body color
	short GetBodyColor();

protected:
	std::string		_Name;
	bool			_DisplayName;
	int				_MaxLife;
	int				_MaxMana;
	int				_CurrentLife;
	int				_CurrentMana;


	// get the place where to draw the text
	float GetTextHeight();
};

#endif