/*
 * =====================================================================================
 *
 *       Filename:  ur_common.h
 *
 *    Description:  A common header file for (non-display related) stuff
 *
 *        Version:  1.0
 *        Created:  13/04/16 18:22:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  UR_COMMON_INC
#define  UR_COMMON_INC

#include <cstdint>
#include <climits>

#define ABS(A) ((A > 0) ? (A) : (-(A)))
#define SIGN(A) ((A > 0) ? (1) : ((A < 0) ? (-1) : 0))
#define MIN(A, B) ((A > B) ? (B) : (A))
#define MAX(A, B) ((A > B) ? (A) : (B))

//#define CITY_SIZE 1000 // We'll try a 1000x1000 city first off

#define CITY_SIZE 200

enum class Building {
	None,
	Hovel,
	Cottage,
	Tavern,
	Warehouse,
	Shop,
	Apartment,
	Mansion,
	Palace
};

enum class EnvironmentObject : uint8_t {
	Nothingness,
	OpenGround,
	Floor,
	OutsideWall,
	InsideWall,
	Door
		// Add more as they come
};

inline bool passible(const EnvironmentObject& _ob) {
	switch (_ob) {
		case EnvironmentObject::Nothingness:
			return false;
		case EnvironmentObject::OpenGround:
			return true;
		case EnvironmentObject::Floor:
			return true;
		case EnvironmentObject::OutsideWall:
			return false;
		case EnvironmentObject::InsideWall:
			return false;
		case EnvironmentObject::Door:
			return true;
	}
}

inline int movement_cost(const EnvironmentObject& _ob) {
	switch (_ob) {
		case EnvironmentObject::Nothingness:
			return INT_MAX;
		case EnvironmentObject::OpenGround:
			return 1;
		case EnvironmentObject::Floor:
			return 1;
		case EnvironmentObject::OutsideWall:
			return INT_MAX;
		case EnvironmentObject::InsideWall:
			return INT_MAX;
		case EnvironmentObject::Door:
			return 2;
	}

}
#endif   /* ----- #ifndef UR_COMMON_INC  ----- */
