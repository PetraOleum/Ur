/*
 * =====================================================================================
 *
 *       Filename:  being.cpp
 *
 *    Description:  Code file for "being" class
 *
 *        Version:  1.0
 *        Created:  15/04/16 11:22:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "being.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  Being
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Being::Being (std::queue<std::pair<int, int> > * (*pathingfunction)(std::pair<int, int>, std::pair<int, int> ))
{
	pathing = pathingfunction;

}  /* -----  end of method Being::Being  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  Being
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
Being::Being ( const Being &other )
{
	position = other.position;
	pathing = other.pathing;
}  /* -----  end of method Being::Being  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  ~Being
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
Being::~Being ()
{
}  /* -----  end of method Being::~Being  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Being
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	Being&
Being::operator = ( const Being &other )
{
	if ( this != &other ) {
		position = other.position;
		pathing = other.pathing;
	}
	return *this;
}  /* -----  end of method Being::operator =  (assignment operator)  ----- */

