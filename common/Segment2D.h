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

#if !defined(__LbaNetModel_1_Segment2D_h)
#define __LbaNetModel_1_Segment2D_h


#include "Point2D.h"


class Segment2D
{
public:

    // Constructor
    explicit Segment2D (float x1 = 0, float y1 = 0, 
						 float x2 = 0, float y2 = 0)
        : p1 (x1, y1), p2 (x2, y2) {}

    // Constructor
    explicit Segment2D (const Point2D & _p1, const Point2D & _p2)
        : p1 (_p1), p2 (_p2) {}


	// test if 2 segments intersect
    bool intersects (const Segment2D &) const;

	// returns true if point is on the segment
    bool contains (const Point2D &) const;
            

    // Comparison operators
    bool operator== (const Segment2D & s) const;
    bool operator!= (const Segment2D & s) const;

    // get utility functions
    const Point2D & get_p1 () const {return p1;}
    const Point2D & get_p2 () const {return p2;}

private:
    Point2D p1;
	Point2D p2;
};

#endif
