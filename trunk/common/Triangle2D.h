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

#if !defined(__LbaNetModel_1_Triangle2D_h)
#define __LbaNetModel_1_Triangle2D_h

#include "Point2D.h"


const int left_turn = +1;
const int right_turn = -1;
const int collinear = 0;

const int counter_clockwise = left_turn;
const int clockwise = right_turn;


class Triangle2D 
{
public:

    //Constructor
    explicit Triangle2D (const Point2D & p1 = origin, 
						   const Point2D & p2 = origin, 
						   const Point2D & p3 = origin)
        : v1 (p1), v2 (p2), v3 (p3) {}

	// tests
    bool contains (const Point2D &) const;
    float area () const;
    int orientation () const;

    // Comparison operators
    bool operator== (const Triangle2D & t) const;
    bool operator!= (const Triangle2D & t) const;
    bool operator> (const Triangle2D &) const;
    bool operator< (const Triangle2D &) const;

private:
    Point2D v1, v2, v3;

    float signed_area () const;
};


inline int turn (const Point2D & p1, const Point2D & p2, const Point2D & p3)
{
    return Triangle2D(p1,p2,p3).orientation();
}


#endif
