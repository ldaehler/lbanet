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

#include "Segment2D.h"
#include "Triangle2D.h"



/***********************************************************
	test if 2 segments intersect
***********************************************************/
bool Segment2D::intersects (const Segment2D & s) const
{
    return turn (p1, p2, s.p1) != turn (p1, p2, s.p2) &&
           turn (s.p1, s.p2, p1) != turn (s.p1, s.p2, p2);
}


/***********************************************************
	returns true if point is on the segment
***********************************************************/
bool Segment2D::contains (const Point2D & p) const
{
        // If the scalar product is negative, it means 
        // p1-p and p2-p are in opposite senses;  therefore, 
        // p is between p1 and p2

    return turn (p1, p2, p) == collinear &&
           ((p-p1) * (p-p2) < 0);
}


/***********************************************************
	Comparison operators
***********************************************************/
bool Segment2D::operator== (const Segment2D & s) const
{
    return p1 == s.p1 && p2 == s.p2;
}


/***********************************************************
	Comparison operators
***********************************************************/
bool Segment2D::operator!= (const Segment2D & s) const
{
    return !(*this == s);
}
