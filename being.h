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
		/* ====================  LIFECYCLE     ======================================= */
		Being ();                             /* constructor      */
		Being ( const Being &other );   /* copy constructor */
		~Being ();                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

		Being& operator = ( const Being &other ); /* assignment operator */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class Being  ----- */

#endif   /* ----- #ifndef BEING_INC  ----- */
