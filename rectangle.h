/*
 * =====================================================================================
 *
 *       Filename:  rectangle.h
 *
 *    Description:  Simple rectangle class
 *
 *        Version:  1.0
 *        Created:  05/04/16 10:18:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <utility>
#include <vector>
#include "ur_common.h"


/*
 * =====================================================================================
 *        Class:  Rectangle
 *  Description:  Basic class for a rectangle
 *        Notes:  Need to think about whether it is inclusive or exclusive
 * =====================================================================================
 */
class Rectangle
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Rectangle (int _y, int _x, int _ydelta, int _xdelta);                             /* constructor */
		Rectangle (std::pair<int, int> corner, std::pair<int, int> oppositecorner);

		// Create a rectangle from the intersection between
		// the two rectangles provided. If no intersection,
		// returns the "zero" rectangle
		Rectangle (Rectangle &r1, Rectangle &r2);

		Rectangle ();

		Rectangle (const Rectangle &other);

		/* ====================  ACCESSORS     ======================================= */

		// The rectange is the same regardless of how it is
		// defined; you don't actually get access to the raw
		// values
		inline int ylow() { return MIN(y, y + ydelta); }
		inline int xlow() { return MIN(x, x + xdelta); }
		inline int yhigh() { return MAX(y, y + ydelta); }
		inline int xhigh() { return MAX(x, x + xdelta); }

		inline int height() { return ABS(ydelta) + 1; }
		inline int width() { return ABS(xdelta) + 1; }

		inline int area() {return height() * width(); }

		// Return true or false depending on whether the
		// point or rectangle is (entirely) within *this
		inline bool contains(std::pair<int, int> point);
		inline bool contains(Rectangle &rect);

		std::vector<std::pair<int, int> > * contact(Rectangle &neighbour);

		std::vector<std::pair<int, int> > * interlock(Rectangle &neighbour);

		// Return a vector of all points on the perimeter of the
		// rectangle, in an order. Must be deleted after use
		std::vector<std::pair<int, int> > * perimeter();

		// Return a vector of all points contained within
		// the rectangle. Must be deleted after use
		std::vector<std::pair<int, int> > * contents();

//		inline bool zero() {return ((ydelta == 0) || (xdelta == 0)); }

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

	Rectangle& operator=(const Rectangle &other);

	Rectangle& operator+=(const std::pair<int, int>& rhs) {
		y += rhs.first;
		x += rhs.second;
		return *this;
	}

	friend Rectangle operator+(Rectangle lhs, const std::pair<int, int>& rhs) {
		lhs += rhs;
		return lhs;
	}

	Rectangle& operator-=(const std::pair<int, int>& rhs) {
		y -= rhs.first;
		x -= rhs.second;
		return *this;
	}

	friend Rectangle operator-(Rectangle lhs, const std::pair<int, int>& rhs) {
		lhs -= rhs;
		return lhs;
	}

	inline bool operator>(Rectangle& other) {
		return this->area() > other.area();
	}

	inline bool operator<(Rectangle& other) {
		return this->area() < other.area();
	}

	inline bool operator>=(Rectangle& other) {
		return !((*this) < other);
	}

	inline bool operator<=(Rectangle& other) {
		return !((*this) > other);
	}


	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */

		int y, x, ydelta, xdelta;

}; /* -----  end of class Rectangle  ----- */

bool overlap(Rectangle &r1, Rectangle &r2);


inline bool operator==(Rectangle &lhs, Rectangle &rhs) {
	return ((lhs.ylow() == rhs.ylow()) && 
			(lhs.yhigh() == rhs.yhigh()) &&
			(lhs.xhigh() == rhs.xhigh()) && 
			(lhs.xlow() == rhs.xlow()));
}

inline bool Rectangle::contains(std::pair<int, int> point) {
	if (point.first > yhigh())
		return false;
	else if (point.first < ylow())
		return false;
	else if (point.second > xhigh())
		return false;
	else if (point.second < xlow())
		return false;
	else
		return true;
}

inline bool Rectangle::contains(Rectangle &rect) {
	if (rect.ylow() < this->ylow())
		return false;
	else if (rect.yhigh() > this->yhigh())
		return false;
	else if (rect.xlow() < this->xlow())
		return false;
	else if (rect.xhigh() > this->xhigh())
		return false;
	else
		return true;
}

inline bool overlap(Rectangle &r1, Rectangle &r2) {
	if (r1.yhigh() < r2.ylow())
		return false;
	else if (r1.ylow() > r2.yhigh())
		return false;
	else if (r1.xhigh() < r2.xlow())
		return false;
	else if (r1.xlow() > r2.xhigh())
		return false;
	else
		return true;
}

inline int edge_contact(Rectangle &r1, Rectangle &r2) {
	if (r1.ylow() == r2.yhigh() + 1)
		return MAX(0, 1 + MIN(r1.xhigh(), r2.xhigh()) - MAX(r1.xlow(), r2.xlow()));
	else if (r1.yhigh() + 1 == r2.ylow())
		return MAX(0, 1 + MIN(r1.xhigh(), r2.xhigh()) - MAX(r1.xlow(), r2.xlow()));
	else if (r1.xlow() == r2.xhigh() + 1)
		return MAX(0, 1 + MIN(r1.yhigh(), r2.yhigh()) - MAX(r1.ylow(), r2.ylow()));
	else if (r1.xhigh() + 1 == r2.xlow())
		return MAX(0, 1 + MIN(r1.yhigh(), r2.yhigh()) - MAX(r1.ylow(), r2.ylow()));
	else
		return 0;
}

inline int interlock(Rectangle &r1, Rectangle &r2) {
	if (r1.ylow() == r2.yhigh())
		return MAX(0, 1 + MIN(r1.xhigh(), r2.xhigh()) - MAX(r1.xlow(), r2.xlow()));
	else if (r1.yhigh() == r2.ylow())
		return MAX(0, 1 + MIN(r1.xhigh(), r2.xhigh()) - MAX(r1.xlow(), r2.xlow()));
	else if (r1.xlow() == r2.xhigh())
		return MAX(0, 1 + MIN(r1.yhigh(), r2.yhigh()) - MAX(r1.ylow(), r2.ylow()));
	else if (r1.xhigh() == r2.xlow())
		return MAX(0, 1 + MIN(r1.yhigh(), r2.yhigh()) - MAX(r1.ylow(), r2.ylow()));
	else
		return 0;
}

#endif
