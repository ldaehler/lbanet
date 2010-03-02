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

#include "Point2D.h"
#include "Segment2D.h"
#include "Triangle2D.h"



/***********************************************************
	test if on segment
***********************************************************/
bool Point2D::is_on (const Segment2D & s) const
{
    return s.contains (*this);
}


/***********************************************************
	test if inside triangle
***********************************************************/
bool Point2D::is_inside (const Triangle2D & t) const
{
    return t.contains (*this);
}


/***********************************************************
	operator+
***********************************************************/
Point2D Point2D::operator+ (const Point2D & p) const
{
    return Point2D (x + p.x, y + p.y);
}


/***********************************************************
	operator- 
***********************************************************/
Point2D Point2D::operator- (const Point2D & p) const
{
    return Point2D (x - p.x, y - p.y);
}


/***********************************************************
	operator*
***********************************************************/
Point2D Point2D::operator* (float r) const
{
    return Point2D (r*x, r*y);
}


/***********************************************************
	Scalar product
***********************************************************/
float Point2D::operator* (const Point2D & p) const
{
    return x * p.x + y * p.y;
}


/***********************************************************
	operator/
***********************************************************/
Point2D Point2D::operator/ (float r) const
{
    return Point2D (x/r, y/r);
}


/***********************************************************
	operator==
***********************************************************/
bool Point2D::operator== (const Point2D & p) const
{
    return x == p.x && y == p.y;
}


/***********************************************************
	operator!=
***********************************************************/
bool Point2D::operator!= (const Point2D & p) const
{
    return !(*this == p);
}
