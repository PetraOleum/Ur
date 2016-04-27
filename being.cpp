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
	home = new std::set<point>;
	carrying_furniture = Furniture::None;
	failed_action = false;
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
	failed_action = other.failed_action;
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
		failed_action = other.failed_action;
	}
	return *this;
}  /* -----  end of method Being::operator =  (assignment operator)  ----- */

void Being::act() {
//	beingmeta_t mymeta = helper->getmeta(position);
	if (home->empty()) {
		delete home;
		home = helper->contig(position, [](EnvironmentObject _obj) { return _obj == EnvironmentObject::Floor; });
	}
	if (planned_path->empty()) {
		delete planned_path;
		point dest = position;
		if (home->empty()) {
			dest = helper->find_nearest(position, [](point _p, EnvironmentObject _obj, Furniture _f) {return (_obj == EnvironmentObject::Floor);});
		}
		else if (carrying_furniture != Furniture::None) {
			if (helper->drop(position, carrying_furniture)) {
				carrying_furniture = Furniture::None;
				dest = helper->find_nearest(position, [this](point _p, EnvironmentObject _obj, Furniture _f) { return (_obj != EnvironmentObject::Nothingness) && (_f != Furniture::None) && (home->find(_p) == home->end()) && !helper->point_hasperson(_p);});
			} else if (helper->containsvalid(home, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && !helper->point_hasperson(_pt);})){
				dest = helper->find_nearest(position, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (home->find(_pt) != home->end()) && !helper->point_hasperson(_pt);});
			} else {
				point newaddition = helper->find_nearest(initialposition, [this](point _p, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (home->find(_p) == home->end()); });
				std::set<point> * nadd = helper->contig(newaddition, [](EnvironmentObject _obj) { return _obj == EnvironmentObject::Floor; });
				home->insert(nadd->begin(), nadd->end());
				delete nadd;
				dest = helper->find_nearest(position, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return _obj == EnvironmentObject::Floor && _f == Furniture::None && (home->find(_pt) != home->end()) && !helper->point_hasperson(_pt);});
			}
		} else {
			carrying_furniture = helper->pickup(position);
			if (carrying_furniture == Furniture::None)
				dest = helper->find_nearest(position, [this](point _p, EnvironmentObject _obj, Furniture _f) { return (_obj != EnvironmentObject::Nothingness) && (_f != Furniture::None) && (home->find(_p) == home->end()) && !helper->point_hasperson(_p);});
			else if (helper->containsvalid(home, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && !helper->point_hasperson(_pt);}))
				dest = helper->find_nearest(position, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (home->find(_pt) != home->end()) && !helper->point_hasperson(_pt);});
			else {
				point newaddition = helper->find_nearest(initialposition, [this](point _p, EnvironmentObject _obj, Furniture _f) { return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (home->find(_p) == home->end()); });
				std::set<point> * nadd = helper->contig(newaddition, [](EnvironmentObject _obj) { return _obj == EnvironmentObject::Floor; });
				home->insert(nadd->begin(), nadd->end());
				delete nadd;
				dest = helper->find_nearest(position, [this](point _pt, EnvironmentObject _obj, Furniture _f) { return _obj == EnvironmentObject::Floor && _f == Furniture::None && (home->find(_pt) != home->end()) && !helper->point_hasperson(_pt);});
			}
		}
		if (dest == position) 
			do
				dest = std::make_pair(rand() % 10 - rand() % 10 + position.first, rand() % 10 - rand() % 10 + position.second);
			while (helper->point_hasperson(dest));

		planned_path = pathto(dest);
		return;
	}
	point next = planned_path->front();
//	if (ABS(position.first - next.first) + ABS(position.second - next.second) == 2 && mymeta.movement_left < 1 + DIAGONAL_COST)
//		return;
//	if (ABS(position.first - next.first) > 1 || ABS(position.second - next.second) > 1) {
//		while (!planned_path->empty())
//			planned_path->pop();
//		return std::make_pair(-1, -1);
//	}
	MovementOutcome action_worked = helper->propose_action(position, next);
//	if (action_worked) {
//		failed_action = false;
//		planned_path->pop();
//	} else if (failed_action) {
//		while (!planned_path->empty())
//			planned_path->pop();
//	} else
//		failed_action = true;
	switch(action_worked) {
		case MovementOutcome::Legal:
			planned_path->pop();
			break;
		case MovementOutcome::InsufficientMovement:
			failed_action = true;
			break;
		case MovementOutcome::Blocked:
		case MovementOutcome::Illegal:
			while (!planned_path->empty())
				planned_path->pop();
			break;
	}
}
