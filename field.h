/*
 * =====================================================================================
 *
 *       Filename:  field.h
 *
 *    Description:  Class for Field construction
 *
 *        Version:  1.0
 *        Created:  30/04/16 15:09:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef FIELD_H
#define FIELD_H

#include "construction.h"

class Field : public Construction {
	public:
		Field();

		bool create();

		bool create(Building _b_t);

		bool create(const Rectangle& _bound, Building _b_t);

		void changebounds(int buffer);

		void move_to(std::pair<int, int> point);

		std::map<point, EnvironmentObject> * all_locations();
	
	private:
		Rectangle field;
};



#endif
