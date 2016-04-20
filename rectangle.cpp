/*
 * =====================================================================================
 *
 *       Filename:  rectangle.cpp
 *
 *    Description:  Class file for the rectangle class
 *
 *        Version:  1.0
 *        Created:  05/04/16 16:08:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "rectangle.h"
#include <utility>
/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rectangle
 *      Method:  Rectangle
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Rectangle::Rectangle (int _y, int _x, int _ydelta, int _xdelta)
{
	y = _y;
	x = _x;
	ydelta = _ydelta;
	xdelta = _xdelta;
}  /* -----  end of method Rectangle::Rectangle  (constructor)  ----- */

Rectangle::Rectangle (std::pair<int, int> corner, std::pair<int, int> oppositecorner) {
	y = corner.first;
	x = corner.second;
	ydelta = oppositecorner.first - corner.first;
	xdelta = oppositecorner.second - corner.second;
}

Rectangle::Rectangle() {
	y = x = ydelta = xdelta = 0;
}

Rectangle::Rectangle(const Rectangle &r1, const Rectangle &r2) {
	if (!overlap(r1, r2)) {
		y = x = ydelta = xdelta = 0;
	} else {
		y = MAX(r1.ylow(), r2.ylow());
		x = MAX(r1.xlow(), r2.xlow());
		ydelta = MIN(r1.yhigh(), r2.yhigh()) - y;
		xdelta = MIN(r1.xhigh(), r2.xhigh()) - x;
	}
}

Rectangle::Rectangle(const Rectangle &other) {
	y = other.y;
	x = other.x;
	ydelta = other.ydelta;
	xdelta = other.xdelta;
}




Rectangle& Rectangle::operator=(const Rectangle &other) {
	y = other.y;
	ydelta = other.ydelta;
	x = other.x;
	xdelta = other.xdelta;
	return *this;
}


std::vector<std::pair<int, int> > * Rectangle::perimeter() {
	std::vector<std::pair<int, int> > * prm 
		= new std::vector<std::pair<int, int> >;
	for (int i = ylow(); i <= yhigh(); i++)
		prm->push_back(std::make_pair(i, xlow()));
	for (int i = xlow() + 1; i <= xhigh(); i++)
		prm->push_back(std::make_pair(yhigh(), i));
	for (int i = yhigh() - 1; i >= ylow(); i--)
		prm->push_back(std::make_pair(i, xhigh()));
	for (int i = xhigh() - 1; i > xlow(); i--)
		prm->push_back(std::make_pair(ylow(), i));
	return prm;
}

std::vector<std::pair<int, int> > * Rectangle::contact(const Rectangle &neighbour) {
	std::vector<std::pair<int, int> > * cont = new std::vector<std::pair<int, int> >;

	if (neighbour.ylow() == this->yhigh() + 1) {
		for (int i = MAX(neighbour.xlow(), this->xlow()); i <= MIN(neighbour.xhigh(), this->xhigh()); i++)
			cont->push_back(std::make_pair(this->yhigh(), i));
	} else if (neighbour.yhigh() + 1 == this->ylow()) {
		for (int i = MAX(neighbour.xlow(), this->xlow()); i <= MIN(neighbour.xhigh(), this->xhigh()); i++)
			cont->push_back(std::make_pair(this->ylow(), i));
	} else if (neighbour.xlow() == this->xhigh() + 1) {
		for (int i = MAX(neighbour.ylow(), this->ylow()); i <= MIN(neighbour.yhigh(), this->yhigh()); i++)
			cont->push_back(std::make_pair(i, this->xhigh()));
	} else if (neighbour.xhigh() + 1 == this->xlow()) {
		for (int i = MAX(neighbour.ylow(), this->ylow()); i <= MIN(neighbour.yhigh(), this->yhigh()); i++)
			cont->push_back(std::make_pair(i, this->xlow()));
	}

	return cont;
}

std::vector<std::pair<int, int> > * Rectangle::interlock(const Rectangle &neighbour) {
	std::vector<std::pair<int, int> > * inl = new std::vector<std::pair<int, int> >;

	
	if (neighbour.ylow() == this->yhigh()) {
		for (int i = MAX(neighbour.xlow(), this->xlow()); i <= MIN(neighbour.xhigh(), this->xhigh()); i++)
			inl->push_back(std::make_pair(this->yhigh(), i));
	} else if (neighbour.yhigh() == this->ylow()) {
		for (int i = MAX(neighbour.xlow(), this->xlow()); i <= MIN(neighbour.xhigh(), this->xhigh()); i++)
			inl->push_back(std::make_pair(this->ylow(), i));
	} else if (neighbour.xlow() == this->xhigh()) {
		for (int i = MAX(neighbour.ylow(), this->ylow()); i <= MIN(neighbour.yhigh(), this->yhigh()); i++)
			inl->push_back(std::make_pair(i, this->xhigh()));
	} else if (neighbour.xhigh() == this->xlow()) {
		for (int i = MAX(neighbour.ylow(), this->ylow()); i <= MIN(neighbour.yhigh(), this->yhigh()); i++)
			inl->push_back(std::make_pair(i, this->xlow()));
	}

	return inl;

}

std::vector<std::pair<int, int> > * Rectangle::contents() {
	std::vector<std::pair<int, int> > * conts = new std::vector<std::pair<int, int> >;

	for (int p = ylow(); p < yhigh() + 1; p++)
		for (int q = xlow(); q < xhigh() + 1; q++)
			conts->push_back(std::make_pair(p, q));
	return conts;
}
