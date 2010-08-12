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


#if !defined(__LbaNetModel_1_3DObjectRenderer_h)
#define __LbaNetModel_1_3DObjectRenderer_h


/***********************************************************************
 * Module:  3DObjectRenderer.h
 * Author:  vivien
 * Modified: mercredi 15 juillet 2009 13:34:13
 * Purpose: Declaration of the class 3DObjectRenderer
 **********************************************************************/
class D3ObjectRenderer
{
public:
	//! constructor
   D3ObjectRenderer();

   //! destructor
   virtual ~D3ObjectRenderer();

   //! show object
   void Show(void);

   //! hide object
   void Hide(void);

   //! check if object is visible
   bool IsVisible(void)
   {return Visible;}

   //! render object
   virtual void Render() = 0;

	// do all check to be done when idle
	virtual int Process(double tnow, float tdiff) = 0;

	//! called to reload element when resizing screen
	virtual void Reload(){}
	//! cleanup
	virtual void CleanUp(){}

	int GetType()
	{return _type;}

	// return speed asociated with animation
	virtual double GetCurrentSpeed() {return 0;}

	// return speed Yasociated with animation
	virtual double GetCurrentSpeedY() {return 0;}

	//! only used form sprites
	virtual long GetSpriteId() {return 0;}

protected:
   bool Visible;

   int _type;
};

#endif