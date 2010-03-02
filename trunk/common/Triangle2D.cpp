/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: code adaptation from mochima.com
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

#include "Triangle2D.h"
#include "Segment2D.h"

#include <math.h>



/***********************************************************
	signed_area
***********************************************************/
float Triangle2D::signed_area () const
{
    return (v1.get_x() * (v2.get_y() - v3.get_y()) +
            v2.get_x() * (v3.get_y() - v1.get_y()) +
            v3.get_x() * (v1.get_y() - v2.get_y()));
}



/***********************************************************
	test if contains point
***********************************************************/
bool Triangle2D::contains (const Point2D & p) const
{
    return turn (v1,v2,p) == turn (v2,v3,p) &&
           turn (v2,v3,p) == turn (v3,v1,p);
}





/***********************************************************
	area
***********************************************************/
float Triangle2D::area () const
{
    return fabs (signed_area());
}



/***********************************************************
	orientation
***********************************************************/
int Triangle2D::orientation () const
{
    float s_a = signed_area();
    return s_a > 0 ? 1 : (s_a < 0 ? -1 : 0);
}



/***********************************************************
	Comparison operators
***********************************************************/
bool Triangle2D::operator== (const Triangle2D & t) const
{
    return v1 == t.v1 && v2 == t.v2 && v3 == t.v3;
}



/***********************************************************
	Comparison operators
***********************************************************/
bool Triangle2D::operator!= (const Triangle2D & t) const
{
    return !(*this == t);
}
