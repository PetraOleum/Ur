/*
 * =====================================================================================
 *
 *       Filename:  house.h
 *
 *    Description:  Header file for the "house" class - the first object in the universe
 *
 *        Version:  1.0
 *        Created:  04/11/15 22:19:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef HOUSE_H
#define HOUSE_H

#include <utility>
#include <vector>
#include "rectangle.h"
#include "ur_common.h"
#include <stdexcept>
#include "construction.h"


/*
 * =====================================================================================
 *        Class:  House
 *  Description:  House class header
 * =====================================================================================
 */
class House : public Construction
{
	public:
//		Rectangle bounds;
//		Building building_type;

		/* ====================  LIFECYCLE     ======================================= */
		House ();                             /* constructor      */
//		House ( const Rectangle& _bounds);
//		House ( const Rectangle& _bounds, Building b_t);
		House ( House &other );   /* copy constructor */
		~House ();                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		std::set<point> * perimeter();
		bool valid();

		std::set<point> * get_doors() {
			std::set<std::pair<int, int> > * d2 = new std::set<std::pair<int, int> >(*doors);
			return d2;
		}

		inline unsigned int number_of_rectangles() {
			if (areas->empty())
				return 0;
			return areas->size();
		}

		inline Rectangle& rec_at(const int index) {
			return this->operator[](index);
		}

		inline unsigned int number_of_rooms() {
			if (rooms->empty())
				return 0;
			return rooms->size();
		}

		inline Rectangle& room_at(const int index) {
			if (index < 0 || (unsigned int)index >= number_of_rooms())
				throw std::out_of_range("Attempting to access non-existant room Rectange in House, Out of Range error std::out_of_range");
			return rooms->at(index);
		}

		inline unsigned int number_of_doors() {
			if (doors->empty())
				return 0;
			return doors->size();
		}

//		inline std::pair<int, int>& door_at(const int index) {
//			if (index < 0 || (unsigned int)index >= number_of_doors())
//				throw std::out_of_range("Attempting to access non-existant door std::pair<int, int> in House, Out of Range error std::out_of_range");
//			return doors->at(index);
//		}

		int contact_length(Rectangle &rect);

		int interlock_length(Rectangle &rect);

		bool overlaps(Rectangle &rect);

		int true_ylow();
		int true_yhigh();
		int true_xlow();
		int true_xhigh();


		int floor_area();

		int accurate_floor_area();

		std::vector<std::pair<int, int> > * floor_locations();

		std::vector<std::pair<int, int> > * room_walls();

		bool contains(std::pair<int, int> point);

		bool contains(Rectangle &rect);


		/* ====================  MUTATORS      ======================================= */

		bool create() {
			return create(bounds(), building_type());
		}

		bool create(const Rectangle& _bound) {
			return create(_bound, building_type());
		}

		bool create(Building _b_t) ;

		bool create(const Rectangle& _bound, Building _b_t) ;

		void changebounds(int buffer = 0);

		inline void add(const Rectangle & rect) {
			areas->push_back(rect);
		}

		void delrecs() {
			delete areas;
			delete rooms;
			delete doors;
			rooms = new std::vector<Rectangle>;
			areas = new std::vector<Rectangle>;
			doors = new std::set<std::pair<int, int> >;
		}

		int fill_with_rooms();

		int fill_with_rooms_2();

		int add_doors();

		void add_outside_doors();

		void move_to(std::pair<int, int> point);

		/* ====================  OPERATORS     ======================================= */

		House& operator = ( const House &other ); /* assignment operator */
		inline Rectangle& operator [] (const int index) {
			if (index < 0 || (unsigned int)index >= number_of_rectangles())
				throw std::out_of_range("Attempting to access non-existant area Rectange in House, Out of Range error std::out_of_range");
			return areas->at(index);
		}

		House& operator+= (const Rectangle & rect) {
			areas->push_back(rect);
			return *this;
		}

		House& operator+= (const std::pair<int, int>& _point) {
			bounds() += _point;
			for (unsigned int i = 0; i < number_of_rectangles(); i++)
				areas->at(i) += _point;
			for (unsigned int i = 0; i < number_of_rooms(); i++)
				rooms->at(i) += _point;
			std::set<point> * ndoors = new std::set<point>;
			for (auto pt : *doors)
				ndoors->insert(std::make_pair(pt.first + _point.first, pt.second + _point.second));
			delete doors;
			doors = ndoors;
			return *this;
		}

		House& operator-= (const std::pair<int, int>& _point) {
			bounds() -= _point;
			for (unsigned int i = 0; i < number_of_rectangles(); i++)
				areas->at(i) -= _point;
			for (unsigned int i = 0; i < number_of_rooms(); i++)
				rooms->at(i) -= _point;
			std::set<point> * ndoors = new std::set<point>;
			for (auto pt : *doors)
				ndoors->insert(std::make_pair(pt.first - _point.first, pt.second - _point.second));
			delete doors;
			doors = ndoors;
			return *this;
		}

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		bool valid_room_addition(Rectangle& rm);
		/* ====================  DATA MEMBERS  ======================================= */

		std::vector<Rectangle> * areas;
		std::vector<Rectangle> * rooms;
		std::set<std::pair<int, int> > * doors;


}; /* -----  end of class House  ----- */

// XXXXX
// AAA
//   AAA

std::vector<Rectangle> * dividerec(Rectangle& rec, int minsize = 3, int maxsize = 4);


#endif
