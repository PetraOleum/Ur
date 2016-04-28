/*
 * =====================================================================================
 *
 *       Filename:  city.h
 *
 *    Description:  Header file for city class
 *
 *        Version:  1.0
 *        Created:  12/04/16 18:12:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  CITY_H_INC
#define  CITY_H_INC


#include "house.h"
#include "ur_common.h"
#include "being.h"
#include "cityhelper.h"
#include "construction.h"

#include <queue>
#include <map>

/*
 * =====================================================================================
 *        Class:  City
 *  Description:  Class for the city object
 * =====================================================================================
 */
class City : CityHelper
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		City ();                             /* constructor      */
		City ( const City &other );   /* copy constructor */
		~City ();                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		inline Rectangle bounds() const {
			return Rectangle(0, 0, CITY_SIZE - 1, CITY_SIZE - 1);
		}

		inline unsigned int number_of_buildings() const {
			if (buildings->empty())
				return 0;
			else
				return buildings->size();
		}

		inline unsigned int number_of_people() const {
			if (people->empty())
				return 0;
			else
				return people->size();
		}

		inline House * building_at(const unsigned int index) {
			if (index >= number_of_buildings())
				throw std::out_of_range("Attempting to access non-existant building House in City. Out of range error std::out_of_range");
			else
				return buildings->at(index);
		}

		inline EnvironmentObject& at(int _yvalue, int _xvalue) const {
			if (_yvalue < 0 || _yvalue >= CITY_SIZE || _xvalue < 0 || _xvalue >= CITY_SIZE)
				throw std::out_of_range("Attempting to access reference to non-existant EnvironmentObject in City.\n Did you mean to call City::get(int, int) instead of City::at(int, int)?\n std::out_of_range");
			else return objectmap[_yvalue][_xvalue];
		}

		inline EnvironmentObject get(int _yvalue, int _xvalue) const {
			if (_yvalue < 0 || _yvalue >= CITY_SIZE || _xvalue < 0 || _xvalue >= CITY_SIZE)
				return EnvironmentObject::Nothingness;
			else return objectmap[_yvalue][_xvalue];
		}

		inline Furniture& junk_at(const point _location) {
			if (_location.first < 0 || _location.second < 0 || _location.first >= CITY_SIZE || _location.second >= CITY_SIZE)
				throw std::out_of_range("Attempting to access a reference to a Furniture object outside of city limits.\n Did you mean to call City::junk_get(point)? (Would return instance of Furniture::None.)\n std::out_of_range");
			else
				return junk[_location];
		}

		inline Furniture junk_get(point _location) const {
			if (_location.first < 0 || _location.second < 0 || _location.first >= CITY_SIZE || _location.second >= CITY_SIZE)
				return Furniture::None;
			else {
				std::map<point, Furniture>::const_iterator it = junk.find(_location);
				if (it == junk.end())
					return Furniture::None;
				else
					return it->second;
			}
		}

		inline void junk_set(const point _location, const Furniture _f) {
			if (_location.first < 0 || _location.second < 0 || _location.first >= CITY_SIZE || _location.second >= CITY_SIZE)
				return;
			else {
				junk[_location] = _f;
				to_be_updated.push(_location);
			}

		}

		inline std::pair<int, int> get_person(unsigned int index) const { 
			if (index >= number_of_people())
				throw std::out_of_range("Attempting to access reference to non-existant Being in City.\n std::out_of_range");
			else
				return people->at(index)->position;
		}

		std::queue<std::pair<int, int> > * astar(std::pair<int, int> start, std::pair<int, int> finish);

		point find_nearest(point, std::function<bool(point, EnvironmentObject, Furniture)>);

		std::set<point> * contig(point, std::function<bool(EnvironmentObject)>);

		bool containsvalid(std::set<point> * area, std::function<bool(point, EnvironmentObject, Furniture)> criterion);

		inline bool points_unprocessed() const {
			return !to_be_updated.empty();
		}

		inline bool point_hasperson(point _loc) const {
			auto it = bmap->find(_loc);
			if (it == bmap->end())
				return false;
			else if (bmap->at(_loc) == 0)
				return false;
			else
				return true;
		}

		inline beingmeta_t getmeta(point position) const {
			if (!point_hasperson(position))
				throw std::out_of_range("Not a valid position to getmeta() (no being here). std::out_of_range");
			return (*bpoints)[(*bmap)[position]];
		}

		/* ====================  MUTATORS      ======================================= */
		
		bool add_building(Building building_type);

		inline void add_people(int number) {
			for (int i = 0; i < number; i++) {
				Being * _bn = new Being(this);
				do {
					_bn->set_init_position(std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE));
				} while (get(_bn->position.first, _bn->position.second) != EnvironmentObject::Floor && !point_hasperson(_bn->position));
				people->push_back(_bn);
				(*bpoints)[_bn] = _bn->position;
				(*bmap)[_bn->position] = _bn;
			}
		}

		void step();

		MovementOutcome propose_action(point start, point end);

		Furniture pickup(point _pt);

		bool drop(point _pt, Furniture _f);

		/* ====================  OPERATORS     ======================================= */

		City& operator = ( const City &other ); /* assignment operator */

		inline void clear_unprocessed_points() {
			while (!to_be_updated.empty())
				to_be_updated.pop();
		}

		inline point pop_unprocessed_points() {
			if (points_unprocessed()) {
				point np = to_be_updated.front();
				to_be_updated.pop();
				return np;
			} else
				return std::make_pair(INT_MIN, INT_MIN);
		}

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		bool add_house_object(House * _building);

		/* ====================  DATA MEMBERS  ======================================= */
		
		std::vector<House *> * buildings;

		EnvironmentObject** objectmap;

		std::vector<Being *> * people;

		std::map<point, Being *> * bmap;

		std::map<Being *, beingmeta_t> * bpoints;

		std::map<point, Furniture> junk;

		std::queue<point> to_be_updated;

}; /* -----  end of class City  ----- */


#endif   /* ----- #ifndef CITY_H_INC  ----- */
