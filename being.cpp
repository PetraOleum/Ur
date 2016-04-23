/*
 * =====================================================================================
 *
 *       Filename:  being.cpp
 *
 *    Description:  Code file for "being" class
 *
 *        Version:  1.0
 *        Created:  15/04/16 11:22:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <ctime>
#include <cstdlib>
#include "being.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  Being
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Being::Being (CityHelper * _helper)
{
	helper = _helper;
	planned_path = new std::queue<std::pair<int, int> >;
	carrying_furniture = Furniture::None;

}  /* -----  end of method Being::Being  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  Being
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
Being::Being ( const Being &other )
{
	position = other.position;
	initialposition = other.initialposition;
	helper = other.helper;
	planned_path = new std::queue<std::pair<int, int> >;
	std::queue<std::pair<int, int> > tqueue;
	while (!other.planned_path->empty()) {
		tqueue.push(other.planned_path->front());
		planned_path->push(other.planned_path->front());
		other.planned_path->pop();
	}
	while (!tqueue.empty()) {
		other.planned_path->push(tqueue.front());
		tqueue.pop();
	}
	carrying_furniture = other.carrying_furniture;
}  /* -----  end of method Being::Being  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  ~Being
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
Being::~Being ()
{
	delete planned_path;
}  /* -----  end of method Being::~Being  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	Being&
Being::operator = ( const Being &other )
{
	if ( this != &other ) {
		position = other.position;
		initialposition = other.initialposition;
		helper = other.helper;
		delete planned_path;
		planned_path = new std::queue<std::pair<int, int> >;
		std::queue<std::pair<int, int> > tqueue;
		while (!other.planned_path->empty()) {
			tqueue.push(other.planned_path->front());
			planned_path->push(other.planned_path->front());
			other.planned_path->pop();
		}
		while (!tqueue.empty()) {
			other.planned_path->push(tqueue.front());
			tqueue.pop();
		}
		carrying_furniture = other.carrying_furniture;
	}
	return *this;
}  /* -----  end of method Being::operator =  (assignment operator)  ----- */

std::pair<int, int> Being::propose_action() {
	using point = std::pair<int, int>;
	if (planned_path->empty()) {
		delete planned_path;
		point dest;
		if (carrying_furniture != Furniture::None) {
			if (helper->drop(position, carrying_furniture)) {
				carrying_furniture = Furniture::None;
				dest = helper->find_nearest(position, [](EnvironmentObject _obj, Furniture _f) { return (_obj != EnvironmentObject::Floor) && (_f != Furniture::None);});
			} else {
				dest = helper->find_nearest(initialposition, [](EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None);});
			}
		} else {
			carrying_furniture = helper->pickup(position);
			if (carrying_furniture == Furniture::None)
				dest = helper->find_nearest(position, [](EnvironmentObject _obj, Furniture _f) { return (_obj != EnvironmentObject::Floor) && (_f != Furniture::None);});
			else
				dest = helper->find_nearest(initialposition, [](EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None);});
		}
		if (dest == position)
			dest = std::make_pair(rand() % 10 - rand() % 10 + position.first, rand() % 10 - rand() % 10 + position.second);
		planned_path = pathto(dest);
		if (planned_path->empty())
			return std::make_pair(-1, -1);
	}
	point next = planned_path->front();
	if (ABS(position.first - next.first) > 1 || ABS(position.second - next.second) > 1) {
		while (!planned_path->empty())
			planned_path->pop();
		return std::make_pair(-1, -1);
	}
	planned_path->pop();
	return next;
}
