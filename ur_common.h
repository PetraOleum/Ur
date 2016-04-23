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
#include <stdexcept>

#define ABS(A) ((A > 0) ? (A) : (-(A)))
#define SIGN(A) ((A > 0) ? (1) : ((A < 0) ? (-1) : 0))
#define MIN(A, B) ((A > B) ? (B) : (A))
#define MAX(A, B) ((A > B) ? (A) : (B))
#define DIAGONAL_COST 0.4142

//#define CITY_SIZE 1000 // We'll try a 1000x1000 city first off

#define CITY_SIZE 500

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

enum class Furniture : uint8_t {
	None,
	Table,
	Chair,
	Barrel,
	Bed,
	Dresser,
	Basin
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
		default:
			throw std::out_of_range("Attempting to access the passibility of an invalid EnvironmentObject. std::out_of_range");
	}
}

using movement_cost_t = double;

inline movement_cost_t movement_cost(const EnvironmentObject& _ob) {
	switch (_ob) {
		case EnvironmentObject::Nothingness:
			return INT_MAX;
		case EnvironmentObject::OpenGround:
			return 1;
		case EnvironmentObject::Floor:
			return 1;
		case EnvironmentObject::OutsideWall:
			return 10000;
		case EnvironmentObject::InsideWall:
			return 10000;
		case EnvironmentObject::Door:
			return 1.5;
		default:
			throw std::out_of_range("Attempting to access the movement cost of an invalid EnvironmentObject. std::out_of_range");
	}

}
#endif   /* ----- #ifndef UR_COMMON_INC  ----- */
