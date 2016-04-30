/*
 * =====================================================================================
 *
 *       Filename:  field.cpp
 *
 *    Description:  Code for Field class
 *
 *        Version:  1.0
 *        Created:  30/04/16 15:19:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "field.h"
#include <cstdlib>
#include <ctime>

Field::Field() {
	_building_type = Building::Field;
}

bool Field::create() {
	return create(_bounds, _building_type);
}

bool Field::create(Building _b_t) {

	_bounds = Rectangle(0, 0, 10 + rand() % 5 - rand() % 5, 15 + rand() % 5 - rand() % 5);
	
	return create(_bounds, _b_t);
}

bool Field::create(const Rectangle& _bound, Building _b_t) {
	// Fill out later
	if (_b_t != Building::Field)
		return false;
	_bounds = _bound;
	field = _bounds;

	return true;
}

void Field::changebounds(int buffer) {
	_bounds = Rectangle(field.ylow() - buffer, field.xlow() - buffer, field.height() + buffer - 1, field.width() + buffer - 1);
}

void Field::move_to(std::pair<int, int> point) {
	std::pair<int, int> diff = std::make_pair(_bounds.ylow() - point.first, _bounds.xlow() - point.second);
	_bounds -= diff;
	field -= diff;
}

std::map<point, EnvironmentObject> * Field::all_locations() {
	std::map<point, EnvironmentObject> * al = new std::map<point, EnvironmentObject>;
	std::vector<point> * fl = field.contents();
	for (point pt : *fl)
		(*al)[pt] = EnvironmentObject::Field;
	delete fl;
	return al;
}
