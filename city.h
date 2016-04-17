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


/*
 * =====================================================================================
 *        Class:  City
 *  Description:  Class for the city object
 * =====================================================================================
 */
class City
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

		inline unsigned int number_of_buildings() {
			if (buildings->empty())
				return 0;
			else
				return buildings->size();
		}

		inline House * building_at(const unsigned int index) {
			if (index >= number_of_buildings())
				throw std::out_of_range("Attempting to access non-existant building House in City. Out of range error std::out_of_range");
			else
				return buildings->at(index);
		}

		inline EnvironmentObject& at(int _yvalue, int _xvalue) {
			if (_yvalue < 0 || _yvalue >= CITY_SIZE || _xvalue < 0 || _xvalue >= CITY_SIZE)
				throw std::out_of_range("Attempting to access reference to non-existant EnvironmentObject in City.\n Did you mean to call City::get(int, int) instead of City::at(int, int)?\n std::out_of_range");
			else return objectmap[_yvalue][_xvalue];
		}

		inline EnvironmentObject get(int _yvalue, int _xvalue) const {
			if (_yvalue < 0 || _yvalue >= CITY_SIZE || _xvalue < 0 || _xvalue >= CITY_SIZE)
				return EnvironmentObject::Nothingness;
			else return objectmap[_yvalue][_xvalue];
		}

		/* ====================  MUTATORS      ======================================= */
		
		bool add_building(Building building_type);


		/* ====================  OPERATORS     ======================================= */

		City& operator = ( const City &other ); /* assignment operator */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		bool add_house_object(House * _building);

		/* ====================  DATA MEMBERS  ======================================= */
		
		std::vector<House *> * buildings;

		EnvironmentObject** objectmap;

}; /* -----  end of class City  ----- */

#endif   /* ----- #ifndef CITY_H_INC  ----- */
