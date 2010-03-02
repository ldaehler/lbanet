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
#if !defined(__LbaNetModel_1_Point2D_h)
#define __LbaNetModel_1_Point2D_h


class Segment2D;
class Triangle2D;

class Point2D 
{
public:

    // Constructor
    explicit Point2D (float _x = 0, float _y = 0)
        : x(_x), y(_y) {}

	// tests
    bool is_on (const Segment2D &) const;
    bool is_inside (const Triangle2D &) const;

    // Arithmetic operators
    Point2D operator+ (const Point2D & p) const;
    Point2D operator- (const Point2D & p) const;
    Point2D operator* (float r) const;
    float operator* (const Point2D & p) const;  // Scalar product
    Point2D operator/ (float r) const;

    // Comparison operators
    bool operator== (const Point2D & p) const;
    bool operator!= (const Point2D & p) const;

    bool operator< (const Point2D & p) const;
    bool operator> (const Point2D & p) const;

    // accessors
    float get_x () const {return x;}
    float get_y () const {return y;}

private:
    float x;
	float y;
};


const Point2D origin (0,0);

#endif
