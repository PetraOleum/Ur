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
#include <ctime>
#include <cstdlib>
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


		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  Being
		 *      Method:  get_carrying_furnature
		 *--------------------------------------------------------------------------------------
		 */
		inline Furniture get_carrying_furniture (  ) const
		{
			return carrying_furniture;
		}		/* -----  end of method Being::get_carrying_furnature  ----- */

		inline BeingRole get_role() const {
			return role;
		}

		
		/* ====================  MUTATORS      ======================================= */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  Being
		 *      Method:  set_carrying_furnature
		 *--------------------------------------------------------------------------------------
		 */
		inline void set_carrying_furniture ( Furniture value )
		{
			carrying_furniture	= value;
			return ;
		}		/* -----  end of method Being::set_carrying_furnature  ----- */

		inline void set_init_position(point _pos) {
			position = initialposition = _pos;
		}

		inline void set_role(BeingRole br) {
			role = br;
		}

		void act();

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

		Furniture carrying_furniture;

		point initialposition;

		std::set<point> * home;

		BeingRole role;

		// AI methods

		void Farmer_act();

		void Lord_act();

		void Baker_act();

}; /* -----  end of class Being  ----- */

#endif   /* ----- #ifndef BEING_INC  ----- */
