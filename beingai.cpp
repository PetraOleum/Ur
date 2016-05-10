/*
 * =====================================================================================
 *
 *       Filename:  beingai.cpp
 *
 *    Description:  AI code for Being class
 *
 *        Version:  1.0
 *        Created:  06/05/16 19:47:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "being.h"

void Being::act() {
//	beingmeta_t mymeta = helper->getmeta(position);

	// Is a floor (EnvironmentObject parameter only)
	auto isFloor = [](EnvironmentObject _obj) {
		return _obj == EnvironmentObject::Floor;
	};

	// Is a floor, with no Being on it
	auto isFloorb = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && !helper->point_hasperson(_pt);
	};

	// Is a floor, with no being on it, having no furniture on it, and within home
	auto clearFloorInside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && !helper->point_hasperson(_pt) && (home->find(_pt) != home->end());
	};

	// Is a floor, with no being on it, having no furniture on it
	auto clearFloor = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && !helper->point_hasperson(_pt);
	};

	// Is a floor, with no being on it, having no furniture on it, not within home
	auto clearFloorOutside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && !helper->point_hasperson(_pt) && (home->find(_pt) == home->end());
	};
	
	// Has no being on it, having a furniture on it, and not within home
	auto furnitureOutside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_f != Furniture::None) && !helper->point_hasperson(_pt) && (home->find(_pt) == home->end());
	};

	if (home->empty()) {
		delete home;
		home = helper->contig(position, isFloor); // set of points within room
	}
	if (planned_path->empty()) {
		delete planned_path;
		point dest = position;
		if (home->empty()) {
			dest = helper->find_nearest(position, isFloorb); // Nearest floor with no Being on it currently
		}
		else if (carrying_furniture != Furniture::None) {
			if (helper->drop(position, carrying_furniture)) {
				carrying_furniture = Furniture::None;
				dest = helper->find_nearest(position, furnitureOutside); // Nearest furniture outside of home
			} else if (helper->containsvalid(home, clearFloor)) // home contains a floor location with no furniture or beings on it
				dest = helper->find_nearest(position, clearFloorInside); // Nearest clear floor location inside home
			else {
				point newaddition = helper->find_nearest(initialposition, clearFloorOutside); // Nearest clear floor position outside of home
				std::set<point> * nadd = helper->contig(newaddition, isFloor); // set of points within the room that newaddition is in
				home->insert(nadd->begin(), nadd->end());
				delete nadd;
				dest = helper->find_nearest(position, clearFloorInside); // nearest point within expanded home with nothing on it
			}
		} else {
			carrying_furniture = helper->pickup(position);
			if (carrying_furniture == Furniture::None)
				dest = helper->find_nearest(position, furnitureOutside); // Nearest furniture outside home
			else if (helper->containsvalid(home, clearFloor))
				dest = helper->find_nearest(position, clearFloorInside); // Nearest clear floor location in home
			else {
				point newaddition = helper->find_nearest(initialposition, clearFloorOutside); // Nearest clear floor location ouside home
				std::set<point> * nadd = helper->contig(newaddition, isFloor); // Set of points of the room in which newaddition is contained
				home->insert(nadd->begin(), nadd->end());
				delete nadd;
				dest = helper->find_nearest(position, clearFloorInside); // nearest clear floor location within expanded home
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
	MovementOutcome action_worked = helper->propose_action(position, next);
	switch(action_worked) {
		case MovementOutcome::Legal:
			planned_path->pop();
			break;
		case MovementOutcome::InsufficientMovement:
			break;
		case MovementOutcome::Blocked:
		case MovementOutcome::Illegal:
			while (!planned_path->empty())
				planned_path->pop();
			break;
	}
}

void Being::Farmer_act() {
	auto clearFloorInside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (!helper->point_hasperson(_pt)  || _pt == position) && (home->find(_pt) != home->end());
	};

	auto dropofffurniture = [this, clearFloorInside]() {
		point dropoff = helper->find_nearest(position, clearFloorInside);
		if (dropoff == position) {
			if (helper->drop(position, carrying_furniture)) {
				carrying_furniture = Furniture::None;
				return true;
			} else
				return false;
		}
		else {
			delete planned_path;
			planned_path = helper->astar(position, dropoff);
			if (planned_path->empty())
				return false;
		}
		return true;
	};

	auto cabbageOutside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj != EnvironmentObject::Floor) && (_f == Furniture::Cabbage) && (!helper->point_hasperson(_pt) || _pt == position);
	};

	auto pickupCabbage = [this, cabbageOutside]() {
		point pickup = helper->find_nearest(position, cabbageOutside);
		if (pickup == position) {
			carrying_furniture = helper->pickup(position);
			if (carrying_furniture == Furniture::Cabbage)
				return true;
			else
				return false;
		} else {
			delete planned_path;
			planned_path = helper->astar(position, pickup);
			if (planned_path->empty())
				return false;
		}
		return true;
	};

	switch (carrying_furniture) {
		case Furniture::Cabbage:
			// Go home and deposit
			dropofffurniture();
			break;
		case Furniture::None:
			// Go get some
			pickupCabbage();
			break;
		default:
			// Drop it at home
			dropofffurniture();
			break;
	}
}

void Being::Cook_act() {
	auto cabbageInside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (!helper->point_hasperson(_pt) || _pt == position) && _f == Furniture::Cabbage && _obj == EnvironmentObject::Floor;
	};

	auto pickupCabbage = [this, cabbageInside]() {
		point pickup = helper->find_nearest(position, cabbageInside);
		if (pickup == position) {
			carrying_furniture = helper->pickup(position);
			if (carrying_furniture == Furniture::Cabbage)
				return true;
			else
				return false;
		} else {
			delete planned_path;
			planned_path = helper->astar(position, pickup);
			if (planned_path->empty())
				return false;
		}
		return true;
	};

	auto clearFloorInside = [this](point _pt, EnvironmentObject _obj, Furniture _f) {
		return (_obj == EnvironmentObject::Floor) && (_f == Furniture::None) && (!helper->point_hasperson(_pt)  || _pt == position) && (home->find(_pt) != home->end());
	};

	auto dropofffurniture = [this, clearFloorInside]() {
		point dropoff = helper->find_nearest(position, clearFloorInside);
		if (dropoff == position) {
			if (helper->drop(position, carrying_furniture)) {
				carrying_furniture = Furniture::None;
				return true;
			} else
				return false;
		}
		else {
			delete planned_path;
			planned_path = helper->astar(position, dropoff);
			if (planned_path->empty())
				return false;
		}
		return true;
	};

	switch (carrying_furniture) {
		case Furniture::Cabbage:
			carrying_furniture = Furniture::Food;
			break;
		case Furniture::Food:
			dropofffurniture();
			break;
		case Furniture::None:
			pickupCabbage();
			break;
		default:
			dropofffurniture();
			break;
	}
}

void Being::Lord_act() {
	int dist = rand() % 5 + 5;
	auto randloc = [this, dist](point _pt, EnvironmentObject _obj, Furniture _f) {
		int dy = _pt.first - position.first;
		int dx = _pt.second - position.second;
		dy = (dy > 0) ? dy : -dy;
		dx = (dx > 0) ? dx : -dx;
		return (!helper->point_hasperson(_pt) && (dy + dx > dist));
	};

	point dest = helper->find_nearest(position, randloc);
	if (dest == position)
		return;

	delete planned_path;
	planned_path = helper->astar(position, dest);
}
