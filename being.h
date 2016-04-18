/*
 * =====================================================================================
 *
 *       Filename:  being.h
 *
 *    Description:  Header file for a "being"
 *
 *        Version:  1.0
 *        Created:  15/04/16 11:17:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  BEING_INC
#define  BEING_INC

#include "ur_common.h"
#include <utility>
#include <queue>
//#include "city.h"
#include "cityhelper.h"

/*
 * =====================================================================================
 *        Class:  Being
 *  Description:  Class for an individual "being"
 * =====================================================================================
 */
class Being
{
	public:
		std::pair<int, int> position;
		CityHelper * helper;
		/* ====================  LIFECYCLE     ======================================= */
		Being (CityHelper * _helper);                             /* constructor      */
		Being ( const Being &other );   /* copy constructor */
		~Being ();                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		std::pair<int, int> propose_action();

		/* ====================  OPERATORS     ======================================= */

		Being& operator = ( const Being &other ); /* assignment operator */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		inline std::queue<std::pair<int, int> > * pathto(std::pair<int, int> destination) {
			return helper->astar(position, destination);
		}
		/* ====================  DATA MEMBERS  ======================================= */

		std::queue<std::pair<int, int> > * planned_path;

}; /* -----  end of class Being  ----- */

#endif   /* ----- #ifndef BEING_INC  ----- */
