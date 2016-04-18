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
		std::queue<std::pair<int, int> > * (*pathing)(std::pair<int, int>, std::pair<int, int>);
		/* ====================  LIFECYCLE     ======================================= */
		Being (std::queue<std::pair<int, int> > * (*pathingfunction)(std::pair<int, int>, std::pair<int, int> ));                             /* constructor      */
		Being ( const Being &other );   /* copy constructor */
		~Being ();                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

		Being& operator = ( const Being &other ); /* assignment operator */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		inline std::queue<std::pair<int, int> > * pathto(std::pair<int, int> destination) {
			return pathing(position, destination);
		}
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class Being  ----- */

#endif   /* ----- #ifndef BEING_INC  ----- */
