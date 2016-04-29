/*
 * =====================================================================================
 *
 *       Filename:  house.cpp
 *
 *    Description:  Code file for the house class
 *
 *        Version:  1.0
 *        Created:  04/11/15 22:21:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
//#include <stdlib.h>
#include "house.h"
#include <algorithm>
#include <climits>
#include <stdlib.h>
#include <set>
//#include <iostream>

/*
 *--------------------------------------------------------------------------------------
 *       Class:  House
 *      Method:  House
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
House::House ()
{
	areas = new std::vector<Rectangle>;
	rooms = new std::vector<Rectangle>;
	doors = new std::set<point>;
	_bounds = Rectangle();
}  /* -----  end of method House::House  (constructor)  ----- */

//House::House(const Rectangle& _bounds) {
//	areas = new std::vector<Rectangle>;
//	rooms = new std::vector<Rectangle>;
//	doors = new std::set<point>;
//	bounds = _bounds;
//}
//
//House::House(const Rectangle& _bounds, Building b_t) {
//	areas = new std::vector<Rectangle>;
//	rooms = new std::vector<Rectangle>;
//	doors = new std::set<point>;
//	bounds = _bounds;
//	building_type = b_t;
//}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  House
 *      Method:  House
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
House::House ( House &other )
{
	bounds() = other.bounds();
	areas = new std::vector<Rectangle>;
	areas->insert(areas->end(), other.areas->begin(), other.areas->end());
	rooms = new std::vector<Rectangle>;
	rooms->insert(rooms->end(), other.rooms->begin(), other.rooms->end());
	doors = new std::set<point>(*(other.doors));
//	doors->insert(doors->end(), other.doors->begin(), other.doors->end());
	
}  /* -----  end of method House::House  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  House
 *      Method:  ~House
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
House::~House ()
{
	delete areas;
	delete rooms;
	delete doors;
}  /* -----  end of method House::~House  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  House
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	House&
House::operator = ( const House &other )
{
	if ( this != &other ) {
		_bounds = other._bounds;
		delete areas;
		delete rooms;
		delete doors;
		areas = new std::vector<Rectangle>;
		areas->insert(areas->end(), other.areas->begin(), other.areas->end());
		rooms = new std::vector<Rectangle>;
		rooms->insert(rooms->end(), other.rooms->begin(), other.rooms->end());
		doors = new std::set<point>(*(other.doors));
//		doors->insert(doors->end(), other.doors->begin(), other.doors->end());
	}
	return *this;
}  /* -----  end of method House::operator =  (assignment operator)  ----- */

std::set<std::pair<int, int> > * House::perimeter() {
	std::set<std::pair<int, int> > * perim = new std::set<std::pair<int, int> >;

	for (unsigned int i = 0; i < number_of_rectangles(); i++) {

		std::vector<std::pair<int, int> > * recperim = rec_at(i).perimeter();
		if (recperim->empty()){
			delete recperim;
			continue;
		}
		std::set<std::pair<int, int> > * recborders = new std::set<std::pair<int, int> >;
		for (unsigned int q = 0; q < number_of_rectangles(); q++) {
			if (q == i) continue;
			std::vector<std::pair<int, int> > * rec_cont = rec_at(i).interlock(rec_at(q));
			if (rec_cont->empty()) {
				delete rec_cont;
			}
			else if (rec_cont->size() < 3){
				delete rec_cont;
			} else {
				for (unsigned int i = 1; i < rec_cont->size() - 1; i++)
					recborders->insert(rec_cont->at(i));
				delete rec_cont;
			}
		}
		for (unsigned int p = 0; p < recperim->size(); p++)
			if (std::find(recborders->begin(), recborders->end(), recperim->at(p)) == recborders->end())
				perim->insert(recperim->at(p));

		delete recperim;
		delete recborders;
	}


	return perim;
}

bool House::valid() {
	for (unsigned int i1 = 0; i1 < number_of_rectangles(); i1++) {
		if (!bounds().contains(rec_at(i1)))
			return false;
		for (unsigned int i2 = i1 + 1; i2 < number_of_rectangles(); i2++)
			if (overlap(rec_at(i1), rec_at(i2)) && !interlock(rec_at(i1), rec_at(i2)))
				return false;
	}
	return true;
}

int House::contact_length(Rectangle &rect) {
	int len = 0;

	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		len += edge_contact(rec_at(i), rect);

	return len;
}

bool House::overlaps(Rectangle &rect) {
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		if (overlap(rec_at(i), rect))
			return true;
	return false;
}

int House::true_ylow() {
	int yl = INT_MAX;
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		yl = MIN(rec_at(i).ylow(), yl);
	return yl;
}

int House::true_xlow() {
	int xl = INT_MAX;
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		xl = MIN(rec_at(i).xlow(), xl);
	return xl;
}

int House::true_yhigh() {
	int yh = INT_MIN;
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		yh = MAX(rec_at(i).yhigh(), yh);
	return yh;
}

int House::true_xhigh() {
	int xh = INT_MIN;
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		xh = MAX(rec_at(i).xhigh(), xh);
	return xh;
}

void House::changebounds(int buffer) {
	bounds() = Rectangle(std::make_pair(true_ylow() - buffer, true_xlow() - buffer), std::make_pair(true_yhigh() + buffer, true_xhigh() + buffer));
}

int House::floor_area() {
	int far = 0;
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		far += rec_at(i).area();
	return far;
}

int House::accurate_floor_area() {
	std::vector<std::pair<int, int> > * fa = floor_locations();
	if (fa->empty()) {
		delete fa;
		return 0;
	}
	int far = fa->size();
	delete fa;
	return far;
}

std::vector<std::pair<int, int> > * House::floor_locations() {
	std::vector<std::pair< int, int> > * fa = new std::vector<std::pair<int, int> >;

	for (unsigned int i = 0; i < number_of_rectangles(); i++) {
		std::vector<std::pair<int, int> > * recar = rec_at(i).contents();
		if (!recar->empty())
			for (unsigned int q = 0; q < recar->size(); q++)
				if (std::find(fa->begin(), fa->end(), recar->at(q)) == fa->end())
					fa->push_back(recar->at(q));
//		fa->insert(fa->end(), recar->begin(), recar->end());
		delete recar;
	}

	return fa;
}

bool House::contains(std::pair<int, int> point) {
	for (unsigned int i = 0; i < number_of_rectangles(); i++)
		if (rec_at(i).contains(point)) return true;
	return false;
}

bool House::contains(Rectangle &rect) {
	std::vector<std::pair<int, int> > * thisarea = floor_locations();
	std::vector<std::pair<int, int> > * recarea = rect.contents();
	for (unsigned int i = 0; i < recarea->size(); i++)
		if (std::find(thisarea->begin(), thisarea->end(), recarea->at(i)) == thisarea->end()) {
			delete thisarea;
			delete recarea;
			return false;
		}

	delete thisarea;
	delete recarea;
	return true;

}

int House::fill_with_rooms() {
	unsigned int failures = 0;
	int yl = true_ylow();
	int xl = true_xlow();
	int yh = true_yhigh();
	int xh = true_xhigh();
	delete rooms;
	rooms = new std::vector<Rectangle>;

	Rectangle cur_rect;
	do {
		int side = rand() % 4;
		int nw = rand() % 6 + rand() % 6 + 2;
		int nh = rand() % 6 + rand() % 6 + 2;
		
		switch ( side ) {
			case 0:	
				cur_rect = Rectangle(yl, rand() % (xh - xl - nw) + xl, nh, nw);
				break;

			case 1:	
				cur_rect = Rectangle(rand() % (yh - yl - nh) + yl, xl, nh, nw);
				break;

			case 2:	
				cur_rect = Rectangle(yh, rand() % (xh - xl - nw) + xl, -nh, nw);
				break;

			default:	
				cur_rect = Rectangle(rand() % (yh - yl - nh) + yl, xh, nh, -nw);
				break;
		}				/* -----  end switch  ----- */
	} while (!this->contains(cur_rect) && (failures++ < 1000)) ;
	if (!(failures < 1000))
		return 0;
	else
		failures = 0;

	rooms->push_back(cur_rect);


	unsigned int crn = 0;
	do {
		Rectangle next_rec;
		crn = crn % number_of_rooms();
		cur_rect = room_at(crn);
		crn++;

		int side = rand() % 4;
		int nw = rand() % 5 + rand() % 5 + 2;
		int nh = rand() % 5 + rand() % 5 + 2;
		
		int nyp = 0;
		int nxp = 0;
		std::pair<int, int> dsp;

		switch ( side ) {
			case 0:
				nyp = cur_rect.ylow();
				nxp = rand() % cur_rect.width() + cur_rect.xlow();
				nh = -nh;
				dsp = std::make_pair(1, 0);
				break;
			case 1:
				nyp = rand() % cur_rect.height() + cur_rect.ylow();
				nxp = cur_rect.xlow();
				nw = -nw;
				dsp = std::make_pair(0, 1);
				break;
			case 2:
				nyp = cur_rect.yhigh();
				nxp = rand() % cur_rect.width() + cur_rect.xlow();
				dsp = std::make_pair(-1, 0);
				break;
			default:
				nyp = rand() % cur_rect.height() + cur_rect.ylow();
				nxp = cur_rect.xhigh();
				dsp = std::make_pair(0, -1);
				break;
		}

		next_rec = Rectangle(nyp, nxp, nh, nw);
		if (!valid_room_addition(next_rec)) {
			failures++;
			continue;
		}

		Rectangle nr2;

		do {
			nr2 = next_rec;
			next_rec = Rectangle(next_rec.ylow(), next_rec.xlow(), next_rec.height() + 1, next_rec.width() + 1) - dsp;
		} while (valid_room_addition(next_rec));
		
		rooms->push_back(nr2);
		failures = 0;


		
//		if (nr_valid && contains(next_rec)) {
//			rooms->push_back(next_rec);
//			failures = 0;
//		} else {
//			failures++;
//		}
		
	} while (failures < number_of_rooms() * 5 + 20);





	return number_of_rooms();
}

bool House::valid_room_addition(Rectangle& rm) {
	if (!contains(rm))
		return false;
	for (unsigned int q = 0; q < number_of_rooms(); q++)
		if (overlap(rm, rooms->at(q)) && interlock(rm, rooms->at(q)) < 3)
			return false;
	return true;

}

int House::fill_with_rooms_2() {
	int SPLIT_MIN = 9;

	delete rooms;
	delete doors;
	rooms = new std::vector<Rectangle>;
	doors = new std::set<point>;

	for (unsigned int q = 0; q < number_of_rectangles(); q++) {
		std::vector<Rectangle> * rec_rooms = new std::vector<Rectangle>;
		rec_rooms->push_back(rec_at(q));

		for (unsigned int i = 0; i < rec_rooms->size();){
			Rectangle rt = rec_rooms->at(i);
//			if (rt.width() < 6 || rt.height() < 6) {
//				i++;
//				continue;
//			}
			Rectangle ra;
			Rectangle rb;
			
			if (rand() % 2 && rt.height() > SPLIT_MIN) {
				int div = rand() % (rt.height() - 4) + 2 + rt.ylow();
				ra = Rectangle(std::make_pair(rt.ylow(), rt.xlow()), std::make_pair(div, rt.xhigh()));
				rb = Rectangle(std::make_pair(div, rt.xlow()), std::make_pair(rt.yhigh(), rt.xhigh()));
			} else if (rt.width() > SPLIT_MIN) {
				int div = rand() % (rt.width() - 4) + 2 + rt.xlow();
				ra = Rectangle(std::make_pair(rt.ylow(), rt.xlow()), std::make_pair(rt.yhigh(), div));
				rb = Rectangle(std::make_pair(rt.ylow(), div), std::make_pair(rt.yhigh(), rt.xhigh()));

			} else if (rt.height() <= SPLIT_MIN) {
				i++;
				continue;
			} else
				continue;
			rec_rooms->at(i) = ra;
			rec_rooms->push_back(rb);
		}



		rooms->insert(rooms->end(), rec_rooms->begin(), rec_rooms->end());
		delete rec_rooms;
	}




	return number_of_rooms();
}

int House::interlock_length(Rectangle &rect) {
	int ilc = 0;
	for (unsigned int i = 0; i < number_of_rectangles(); i++) {
		int il = interlock(rec_at(i), rect);
		if (il == 0 && overlap(rec_at(i), rect))
			return 0;
		ilc += il;
	}
	return ilc;
}

int House::add_doors() {
	delete doors;
	doors = new std::set<point>;
	for (unsigned int i = 0; i < number_of_rooms(); i++) {
//		int ndr = (rand() % 9 == 0) ? 2 : 1;
//		std::vector<std::pair<int, int> > * rperim = room_at(i).perimeter();
//		for (int q = 0; q < ndr; q++)
//			doors->push_back(rperim->at(rand() % rperim->size()));
//		delete rperim;
//
		Rectangle tr = room_at(i);
		for (unsigned int q = i + 1; q < number_of_rooms(); q++) {
			Rectangle otr = room_at(q);
			int inter = interlock(tr, otr);
			if (inter > 2) {
				std::vector<std::pair<int, int> > * ilock = tr.interlock(otr);
				doors->insert(ilock->at(rand() % (inter - 2) + 1));
				delete ilock;
			}
		}
	}

	return number_of_doors();
}

void House::add_outside_doors() {
	int yl = true_ylow();
	int xl = true_xlow();
	int yh = true_yhigh();
	int xh = true_xhigh();

	unsigned int nr = number_of_rooms();

	Rectangle cr;

	do {
		cr = room_at(rand() % nr);
	} while (cr.ylow() != yl || cr.width() < 3);

	doors->insert(std::make_pair(yl, rand() % (cr.width() - 2) + cr.xlow() + 1));

	do {
		cr = room_at(rand() % nr);
	} while (cr.xlow() != xl || cr.height() < 3);

	doors->insert(std::make_pair(rand() % (cr.height() - 2) + cr.ylow() + 1, xl));


	do {
		cr = room_at(rand() % nr);
	} while (cr.yhigh() != yh || cr.width() < 3);

	doors->insert(std::make_pair(yh, rand() % (cr.width() - 2) + cr.xlow() + 1));

	do {
		cr = room_at(rand() % nr);
	} while (cr.xhigh() != xh || cr.height() < 3);

	doors->insert(std::make_pair(rand() % (cr.height() - 2) + cr.ylow() + 1, xh));

}

std::vector<Rectangle> * build_areas(Rectangle& _bound, int numrecs, int averecsize, int recspread = 0, int _overlaps = 4) {
	std::vector<Rectangle> * ars = new std::vector<Rectangle>;
	for (int i = 0; i < numrecs; i++) {
		int dy = averecsize + rand() % (recspread + 1) - rand() % (recspread + 1);
		int dx = averecsize + rand() % (recspread + 1) - rand() % (recspread + 1);
		int yl, xl;
		Rectangle nc;
		if (ars->empty()) {
			yl = _bound.ylow() + rand() % (_bound.height() - dy);
			xl = _bound.xlow() + rand() % (_bound.width() - dx);
			nc = Rectangle(yl, xl, dy, dx);
		} else {
			bool notwork;
			int failures = 0;
			do {
				int rn1 = rand() % i;
				int rn2 = rand() % i;
				int rnd = ABS(rn1 - rn2);
				Rectangle rr = ars->at(rnd);
				int side = rand() % 4;
				switch (side) {
					case 0:
						yl = rr.yhigh();
						xl = rand() % (dx + rr.width() - (_overlaps * 2) + 2) + rr.xlow() - dx - 1 + _overlaps;
						break;
					case 1:
						yl = rand() % (dy + rr.height() - (_overlaps * 2) + 2) + rr.ylow() - dy - 1 + _overlaps;
						xl = rr.xhigh();
						break;
					case 2:
						yl = rr.ylow() - dy;
						xl = rand() % (dx + rr.width() - (_overlaps * 2) + 2) + rr.xlow() - dx - 1 + _overlaps;
						break;
					case 3:
						yl = rand() % (dy + rr.height() - (_overlaps * 2) + 2) + rr.ylow() - dy - 1 + _overlaps;
						xl = rr.xlow() - dx;
						break;
					default:
						return ars;
				}
				nc = Rectangle(yl, xl, dy, dx);
				notwork = !_bound.contains(nc);
				for (int q = 0; q < i && !notwork; q++)
					if (overlap(ars->at(q), nc) && interlock(ars->at(q), nc) < 3)
						notwork = true;
				if (notwork)
					failures++;
			} while (notwork && failures < 1000);
			if (notwork)
				return ars;
		}
		ars->push_back(nc);
	}

	return ars;
}

bool House::create(const Rectangle& _bound, Building _b_t) {
//	std::cout << "\tStart create()" << std::endl;
	_bounds = _bound;
//	std::cout << "\t\tBound area = " << _bounds.area() << std::endl;
	_building_type = _b_t;

	delrecs();

	switch (_building_type) {
		case Building::Hovel: {
			if (_bounds.width() < 10 || _bounds.height() < 10)
				return false;
			int dy = rand() % 3 + rand() % 3 + 5;
			int dx = rand() % 3 + rand() % 3 + 5;
			int yl = bounds().ylow() + ((bounds().height() == dy + 1) ? (0) : (rand() % (bounds().height() - dy - 1)));
			int xl = bounds().xlow() + ((bounds().width() == dx + 1) ? (0) : (rand() % (bounds().width() - dx - 1)));
			Rectangle hvlrec(yl, xl, dy, dx);
			areas->push_back(hvlrec);
			delete rooms;
			rooms  = dividerec(hvlrec, 4);
			add_doors();
			add_outside_doors();

		}
			break;
		case Building::Cottage:
			if (_bounds.width() < 25 || _bounds.height() < 25)
				return false;
			else {
				int numrecs = 3 + rand() % 2 + rand() % 2;
				delete areas;
				areas = build_areas(_bounds, numrecs, 6, 2, 5);
				if (areas->empty())
					return false;
				for (unsigned int i = 0; i < areas->size(); i++) {
					std::vector<Rectangle> * arooms = dividerec(areas->at(i), 4);
					rooms->insert(rooms->begin(), arooms->begin(), arooms->end());
					delete arooms;
				}
			}
			add_doors();
			add_outside_doors();

			break;
		case Building::Apartment:
			if (_bounds.width() < 30 || _bounds.height() < 30)
				return false;
			else {
				delete areas;
				areas = build_areas(_bounds, 1, 25, 4);
				if (areas->empty())
					return false;
				delete rooms;
				rooms = dividerec(areas->at(0), 3, 7);
			}
			add_doors();
			add_outside_doors();

			break;
		case Building::Tavern:
			if (_bounds.width() < 20 || _bounds.height() < 20)
				return false;
			else {
				int dy = rand() % 3 + rand() % 3 + 6;
				int dx = rand() % 3 + rand() % 3 + 6;
				
				int yl = _bounds.ylow() + (rand() % (_bounds.height() - dy - 10)) + 5;
				int xl = _bounds.xlow() + (rand() % (_bounds.width() - dx - 10)) + 5;

				Rectangle mainroom(yl, xl, dy, dx);
				areas->push_back(mainroom);
				rooms->push_back(mainroom);

				int no_side = rand() % 4;
				if (no_side != 0) { // side room below (> yhigh()) main room
					dy = rand() % 2 + rand() % 2 + 3; //Height of side room varies
					dx = mainroom.width() - 1; // Same x dimension as main room
					yl = mainroom.yhigh(); // Because below
					xl = mainroom.xlow(); // Bottom left corner
					Rectangle sidroom(yl, xl, dy, dx); // Make rectangle
					areas->push_back(sidroom); // Add rectangle
					std::vector<Rectangle> * srooms = dividerec(sidroom, 4); // Divide into rooms
					rooms->insert(rooms->end(), srooms->begin(), srooms->end()); // Add rooms
					delete srooms; // Delete vector
				} else { // Entrance room below main room
					dy = 4; // Height of 5
					dx = 4; // Width of 5
					yl = mainroom.yhigh(); // Above mainroom
					xl = mainroom.xlow() + rand() % (mainroom.width() - 4); // Random point along edge
					Rectangle entranceroom(yl, xl, dy, dx); // create rectangle
					areas->push_back(entranceroom); // add rectangle to areas
					rooms->push_back(entranceroom); // rectangle is single room
				}
				if (no_side != 1) { // Below mainroom
					dy = rand() % 2 + rand() % 2 + 3;
					dx = mainroom.width() - 1;
					yl = mainroom.ylow() - dy; //point above
					xl = mainroom.xlow();
					Rectangle sidroom(yl, xl, dy, dx);
					areas->push_back(sidroom);
					std::vector<Rectangle> * srooms = dividerec(sidroom, 4);
					rooms->insert(rooms->end(), srooms->begin(), srooms->end());
					delete srooms;
				} else {
					dy = 4;
					dx = 4;
					yl = mainroom.ylow() - dy;
					xl = mainroom.xlow() + rand() % (mainroom.width() - 4);
					Rectangle entranceroom(yl, xl, dy, dx); // create rectangle
					areas->push_back(entranceroom); // add rectangle to areas
					rooms->push_back(entranceroom); // rectangle is single room
				}
				if (no_side != 2) { // Left of mainroom
					dy = mainroom.height() - 1;
					dx = rand() %2 + rand() % 2 + 3;
					yl = mainroom.ylow();
					xl = mainroom.xlow() - dx;
					Rectangle sidroom(yl, xl, dy, dx);
					areas->push_back(sidroom);
					std::vector<Rectangle> * srooms = dividerec(sidroom, 4);
					rooms->insert(rooms->end(), srooms->begin(), srooms->end());
					delete srooms;
				} else {
					dy = 4;
					dx = 4;
					yl = mainroom.ylow() + rand() % (mainroom.height() - 4);
					xl = mainroom.xlow() - dx;
					Rectangle entranceroom(yl, xl, dy, dx); // create rectangle
					areas->push_back(entranceroom); // add rectangle to areas
					rooms->push_back(entranceroom); // rectangle is single room
				}
				if (no_side != 3) { // Right of mainroom
					dy = mainroom.height() - 1;
					dx = rand() % 2 + rand() % 2 + 3;
					yl = mainroom.ylow();
					xl = mainroom.xhigh();
					Rectangle sidroom(yl, xl, dy, dx);
					areas->push_back(sidroom);
					std::vector<Rectangle> * srooms = dividerec(sidroom, 4);
					rooms->insert(rooms->end(), srooms->begin(), srooms->end());
					delete srooms;
				} else {
					dy = 4;
					dx = 4;
					yl = mainroom.ylow() + rand() % (mainroom.height() - 4);
					xl = mainroom.xhigh();
					Rectangle entranceroom(yl, xl, dy, dx); // create rectangle
					areas->push_back(entranceroom); // add rectangle to areas
					rooms->push_back(entranceroom); // rectangle is single room
				}
				
				add_doors();
				add_outside_doors();


			}
			break;
		case Building::Warehouse:
			if (_bounds.width() < 15 || _bounds.height() < 15)
				return false;
			else {
				delete areas;
				areas = build_areas(_bounds, 1, 13, 2);
				if (areas->empty())
					return false;
				for (unsigned int i = 0; i < areas->size(); i++) {
					std::vector<Rectangle> * arooms = dividerec(areas->at(i), 4, 10);
					rooms->insert(rooms->begin(), arooms->begin(), arooms->end());
					delete arooms;
				}
			}
			add_doors();
			add_outside_doors();

			break;
		case Building::Shop:
			if (_bounds.width() < 15 || _bounds.height() < 15)
				return false;
			else {
				delete areas;
				areas = build_areas(_bounds, 2, 6, 2);
				if (areas->empty())
					return false;
				rooms->push_back(areas->at(0));
				if (areas->size() > 1) {
					std::vector<Rectangle> * arooms = dividerec(areas->at(1), 4);
					rooms->insert(rooms->begin(), arooms->begin(), arooms->end());
					delete arooms;
				}
			}
			add_doors();
			add_outside_doors();

			break;
		case Building::Mansion:
			if (_bounds.width() < 20 || _bounds.height() < 20 || _bounds.area() < 2500)
				return false;
			else {
				int numrecs = 8 + rand() % 2 - rand() % 2;
				delete areas;
				areas = build_areas(_bounds, numrecs, 8, 3, 4);
				if (areas->empty())
					return false;
				for (unsigned int i = 0; i < areas->size(); i++) {
					std::vector<Rectangle> * arooms = dividerec(areas->at(i), 4);
					rooms->insert(rooms->begin(), arooms->begin(), arooms->end());
					delete arooms;
				}
			}
			add_doors();
			add_outside_doors();

			break;
		case Building::Palace:
			if (_bounds.width() < 50 || _bounds.height() < 50 || _bounds.area() < 10000) {
//				std::cout << "\tPalace Fail 1" << std::endl;
				return false;
			}
			else {
				int numrecs = 15 + rand() % 4 - rand() % 4;
				delete areas;
				areas = build_areas(_bounds, numrecs, 12, 3, 7);
				if (areas->empty())
					return false;
				for (unsigned int i = 0; i < areas->size(); i++) {
					std::vector<Rectangle> * arooms = dividerec(areas->at(i), 4, 9);
					rooms->insert(rooms->begin(), arooms->begin(), arooms->end());
					delete arooms;
				}

			}
			add_doors();
			add_outside_doors();
			
			break;
		case Building::None:
//			std::cout << "Fail - none" << std::endl;
			return false;
			break;
	}

	return true;
}

bool House::create(Building _b_t) {
	Rectangle nbounds;

//	std::cout << (int)_building_type << std::endl;
	
	switch (_b_t) {
		case Building::Hovel: 
			nbounds = Rectangle(0, 0, 10, 10);
			break;
		case Building::Cottage:
			nbounds = Rectangle(0, 0, 25, 25);
			break;
		case Building::Apartment:
			nbounds = Rectangle(0, 0, 30, 30);
			break;
		case Building::Tavern:
			nbounds = Rectangle(0, 0, 20, 20);
			break;
		case Building::Warehouse:
			nbounds = Rectangle(0, 0, 15, 15);
			break;
		case Building::Shop:
			nbounds = Rectangle(0, 0, 15, 15);
			break;
		case Building::Mansion:
			nbounds = Rectangle(0, 0, 50, 50);
			break;
		case Building::Palace:
			nbounds = Rectangle(0, 0, 100, 100);
			break;
		case Building::None:
			break;
	}

	return create(nbounds, _b_t);
}

std::vector<Rectangle> * dividerec(Rectangle& rec, int minsize, int maxsize) {
	std::vector<Rectangle> * divrec = new std::vector<Rectangle>;
	divrec->push_back(rec);
	int min2 = minsize - 1;
	for (unsigned int i = 0; i < divrec->size(); i++) {
		Rectangle rt = divrec->at(i);
		while (rt.width() > MAX(maxsize, min2 * 2) || rt.height() > MAX(maxsize, min2 * 2)) {
			if (rand() % 2 && rt.height() > MAX(maxsize, min2 * 2)) {
				int div = rand() % (rt.height() - (min2 * 2)) + min2 + rt.ylow();
				Rectangle ra(std::make_pair(rt.ylow(), rt.xlow()), std::make_pair(div, rt.xhigh()));
				Rectangle rb(std::make_pair(div, rt.xlow()), std::make_pair(rt.yhigh(), rt.xhigh()));
				rt = ra;
				divrec->at(i) = ra;
				divrec->push_back(rb);
			} else if (rt.width() > MAX(maxsize, min2 * 2)) {
				int div = rand() % (rt.width() - (min2 * 2)) + min2 + rt.xlow();
				Rectangle ra(std::make_pair(rt.ylow(), rt.xlow()), std::make_pair(rt.yhigh(), div));
				Rectangle rb(std::make_pair(rt.ylow(), div), std::make_pair(rt.yhigh(), rt.xhigh()));
				rt = ra;
				divrec->at(i) = ra;
				divrec->push_back(rb);
			}

		}
	}


	return divrec;
}



void House::move_to(std::pair<int, int> point) {
	std::pair<int, int> diff = std::make_pair(_bounds.ylow() - point.first, _bounds.xlow() - point.second);
	operator-=(diff);
}

std::vector<std::pair<int, int> > * House::room_walls() {
	std::set<std::pair<int, int> > * wls = new std::set<std::pair<int, int> >;
	for (unsigned int i = 0; i < number_of_rooms(); i++){
		std::vector<std::pair<int, int> > * roomperim = room_at(i).perimeter();
		wls->insert(roomperim->begin(), roomperim->end());
		delete roomperim;
	}
	std::vector<std::pair<int, int> > * rwlls = new std::vector<std::pair<int, int> >(wls->begin(), wls->end());
	delete wls;
	return rwlls;
		
}

std::map<point, EnvironmentObject> * House::all_locations() {
	std::map<point, EnvironmentObject> * _locs = new std::map<point, EnvironmentObject>;

	std::vector<point> * floors = floor_locations();
	for (point pt : *floors)
		(*_locs)[pt] = EnvironmentObject::Floor;
	delete floors;

	std::vector<point> * walls = room_walls();
	for (point pt : *walls)
		(*_locs)[pt] = EnvironmentObject::InsideWall;
	delete walls;

	std::set<point> * external_walls = perimeter();
	for (point pt : *external_walls)
		(*_locs)[pt] = EnvironmentObject::OutsideWall;
	delete external_walls;

	std::set<point> * _doors = get_doors();
	for (point pt : *_doors)
		(*_locs)[pt] = EnvironmentObject::Door;
	delete _doors;

	return _locs;
}
