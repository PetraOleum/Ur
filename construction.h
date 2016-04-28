/*
 * =====================================================================================
 *
 *       Filename:  construction.h
 *
 *    Description:  Class for all house/farm/etc objects to inherit from
 *
 *        Version:  1.0
 *        Created:  28/04/16 14:11:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include <utility>
#include <vector>
#include "rectangle.h"
#include "ur_common.h"
#include <stdexcept>
#include <set>

class Construction {
	public:

		Construction() {
			bounds() = Rectangle();
			building_type() = Building::None;
		}

		virtual std::set<point> * perimeter() = 0;

		virtual std::set<point> * get_doors() = 0;

		virtual unsigned int number_of_rectangles() = 0;

		virtual bool create(const Rectangle& _bound, Building _b_t) = 0;

		inline Rectangle& bounds() {
			return _bounds;
		}

		inline Building& building_type() {
			return _building_type;
		}

		virtual ~Construction() {

		}

	protected:
		Rectangle _bounds;
		Building _building_type;


};

#endif
