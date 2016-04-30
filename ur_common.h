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

using point = std::pair<int, int>;
using movement_cost_t = double;

enum class Building {
	None,
	Hovel,
	Cottage,
	Tavern,
	Warehouse,
	Shop,
	Apartment,
	Mansion,
	Palace,
	Field
};

enum class EnvironmentObject : uint8_t {
	Nothingness,
	OpenGround,
	Floor,
	OutsideWall,
	InsideWall,
	Door,
	Field
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

enum class MovementOutcome : uint8_t {
	Illegal,
	InsufficientMovement,
	Blocked,
	Legal
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
		case EnvironmentObject::Field:
			return true;
		default:
			throw std::out_of_range("Attempting to access the passibility of an invalid EnvironmentObject. std::out_of_range");
	}
}

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
		case EnvironmentObject::Field:
			return 2;
		default:
			throw std::out_of_range("Attempting to access the movement cost of an invalid EnvironmentObject. std::out_of_range");
	}

}

struct beingmeta_t {
		point position;
		movement_cost_t movement_left;

		beingmeta_t() {
			position = std::make_pair(0, 0);
			movement_left = 0;
		}

		beingmeta_t(point _loc) {
			position = _loc;
			movement_left = 0;
		}

		beingmeta_t(point _loc, movement_cost_t mp) {
			position = _loc;
			movement_left = mp;
		}

		beingmeta_t(const beingmeta_t &other) {
			position = other.position;
			movement_left = other.movement_left;
		}

		inline beingmeta_t& operator = (const beingmeta_t &other) {
			if (this != &other) {
				position = other.position;
				movement_left = other.movement_left;
			}
			return *this;
		}
};

#endif   /* ----- #ifndef UR_COMMON_INC  ----- */
