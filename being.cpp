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
	home = new std::set<point>;
	carrying_furniture = Furniture::None;

	role = BeingRole::Farmer;
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
	if (other.planned_path != 0) {
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
	}
	if (other.home != 0) {
		home = new std::set<point>;
		home->insert(other.home->begin(), other.home->end());
	}
	carrying_furniture = other.carrying_furniture;
	role = other.role;
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
	delete home;
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
		if (other.planned_path != 0) {
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
		}
		delete home;
		if (other.home != 0) {
			home = new std::set<point>;
			home->insert(other.home->begin(), other.home->end());
		}
		carrying_furniture = other.carrying_furniture;
		role = other.role;
	}
	return *this;
}  /* -----  end of method Being::operator =  (assignment operator)  ----- */

