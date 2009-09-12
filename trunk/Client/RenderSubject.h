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

/***********************************************************************
 * Module:  RenderSubject.h
 * Author:  vivien
 * Modified: mercredi 15 juillet 2009 11:28:09
 * Purpose: Declaration of the class RenderSubject
 ***********************************************************************/

#if !defined(__LbaNetModel_1_RenderSubject_h)
#define __LbaNetModel_1_RenderSubject_h

class RenderObserver;

#include <RenderObserver.h>

class RenderSubject
{
public:
   void RegisterRO(RenderObserver* Observer);
   void UnregisterRO(RenderObserver* Observer);
   void Render(void);
   RenderSubject();
   virtual ~RenderSubject();

protected:
private:
   std::vector<RenderObserver *> renderObserver;


};

#endif