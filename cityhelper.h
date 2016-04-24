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
#include <set>
#include <functional>
#include "ur_common.h"

using point = std::pair<int, int>;

class CityHelper {
	public:
		virtual std::queue<point> * astar(point, point) = 0;
		virtual point find_nearest(point, std::function<bool(point, EnvironmentObject, Furniture)>) = 0;
		virtual Furniture pickup(point) = 0;
		virtual bool drop(point, Furniture) = 0;
		virtual std::set<point> * contig(point, std::function<bool(EnvironmentObject)>) = 0;
		virtual bool containsvalid(std::set<point> *, std::function<bool(EnvironmentObject, Furniture)>) = 0;
};

#endif
