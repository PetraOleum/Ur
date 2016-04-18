/*
 * =====================================================================================
 *
 *       Filename:  cityhelper.h
 *
 *    Description:  Header file for cityhelper
 *
 *        Version:  1.0
 *        Created:  19/04/16 10:08:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CITYHELPER_H
#define CITYHELPER_H

#include <queue>
#include <utility>

using point = std::pair<int, int>;

class CityHelper {
	public:
		virtual std::queue<point> * astar(point, point) = 0;
};

#endif
