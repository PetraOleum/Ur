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

#define ABS(A) ((A > 0) ? (A) : (-(A)))
#define SIGN(A) ((A > 0) ? (1) : ((A < 0) ? (-1) : 0))
#define MIN(A, B) ((A > B) ? (B) : (A))
#define MAX(A, B) ((A > B) ? (A) : (B))

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
#endif   /* ----- #ifndef UR_COMMON_INC  ----- */
