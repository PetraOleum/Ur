/*
 * =====================================================================================
 *
 *       Filename:  city.cpp
 *
 *    Description:  Code file for city class
 *
 *        Version:  1.0
 *        Created:  12/04/16 19:54:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "city.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  City
 *      Method:  City
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
City::City ()
{
	buildings = new std::vector<House *>;
	objectmap = new EnvironmentObject*[CITY_SIZE];
	for (int y = 0; y < CITY_SIZE; y++) {
		objectmap[y] = new EnvironmentObject[CITY_SIZE];
		for (int x = 0; x < CITY_SIZE; x++)
			objectmap[y][x] = EnvironmentObject::OpenGround;
	}
}  /* -----  end of method City::City  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  City
 *      Method:  City
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
City::City ( const City &other )
{
	delete buildings;
	for (int y = 0; y < CITY_SIZE; y++)
		delete[] objectmap[y];
	delete[] objectmap;
	buildings = new std::vector<House *>;
	buildings->insert(buildings->end(), other.buildings->begin(), other.buildings->end());
	objectmap = new EnvironmentObject*[CITY_SIZE];
	for (int y = 0; y < CITY_SIZE; y++) {
		objectmap[y] = new EnvironmentObject[CITY_SIZE];
		for (int x = 0; x < CITY_SIZE; x++)
			objectmap[y][x] = other.objectmap[y][x];
	}
	
}  /* -----  end of method City::City  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  City
 *      Method:  ~City
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
City::~City ()
{
	delete buildings;
	for (int y = 0; y < CITY_SIZE; y++)
		delete[] objectmap[y];
	delete[] objectmap;
}  /* -----  end of method City::~City  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  City
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	City&
City::operator = ( const City &other )
{
	if ( this != &other ) {
		delete buildings;
		for (int y = 0; y < CITY_SIZE; y++)
			delete[] objectmap[y];
		delete[] objectmap;
		buildings = new std::vector<House *>;
		buildings->insert(buildings->end(), other.buildings->begin(), other.buildings->end());
		objectmap = new EnvironmentObject*[CITY_SIZE];
		for (int y = 0; y < CITY_SIZE; y++) {
			objectmap[y] = new EnvironmentObject[CITY_SIZE];
			for (int x = 0; x < CITY_SIZE; x++)
				objectmap[y][x] = other.objectmap[y][x];
	}
	}
	return *this;
}  /* -----  end of method City::operator =  (assignment operator)  ----- */

bool City::add_house_object(House * _building) {
	if (!bounds().contains(_building->bounds))
		return false;
	buildings->push_back(_building);

	std::vector<std::pair<int, int> > * building_floor = _building->floor_locations();
	if (building_floor->empty())
		return true;
	for (unsigned int i = 0; i < building_floor->size(); i++)
		objectmap[building_floor->at(i).first][building_floor->at(i).second] = EnvironmentObject::Floor;
	delete building_floor;
	
	std::vector<std::pair<int, int> > * internal_walls = _building->room_walls();
	if (!internal_walls->empty())
		for (unsigned int i = 0; i < internal_walls->size(); i++)
			objectmap[internal_walls->at(i).first][internal_walls->at(i).second] = EnvironmentObject::InsideWall;
	delete internal_walls;

	std::vector<std::pair<int, int> > * external_walls = _building->perimeter();
	if (!external_walls->empty())
		for (unsigned int i = 0; i < external_walls->size(); i++)
			objectmap[external_walls->at(i).first][external_walls->at(i).second] = EnvironmentObject::OutsideWall;
	delete external_walls;

	std::vector<std::pair<int, int> > * building_doors = _building->get_doors();
	if (!building_doors->empty())
		for (unsigned int i = 0; i < building_doors->size(); i++)
			objectmap[building_doors->at(i).first][building_doors->at(i).second] = EnvironmentObject::Door;
	delete building_doors;

	return true;
}

bool City::add_building(Building building_type) {
	House * newbuilding = new House;
	if (!newbuilding->create(building_type)) {
		delete newbuilding;
		return false;
	}
	newbuilding->changebounds(1);

	bool workable = false;
	int buildingheight = newbuilding->bounds.height();
	int buildingwidth = newbuilding->bounds.width();

	for (unsigned int i = 0; i < CITY_SIZE * 10 && !workable; i++) {
		int by = (CITY_SIZE - buildingheight) / 2 + rand() % ((CITY_SIZE - buildingheight) / 2) - rand() % ((CITY_SIZE - buildingheight) / 2);
		int bx = (CITY_SIZE - buildingwidth) / 2 + rand() % ((CITY_SIZE - buildingwidth) / 2) - rand() % ((CITY_SIZE - buildingwidth) / 2);
//		int by = rand() % (CITY_SIZE - buildingheight);
//		int bx = rand() % (CITY_SIZE - buildingwidth);
		newbuilding->move_to(std::make_pair(by, bx));
		if (!bounds().contains(newbuilding->bounds))
			continue;
		workable = true;
		for (unsigned int q = 0; q < number_of_buildings(); q++)
			if (overlap(building_at(q)->bounds, newbuilding->bounds)) {
				workable = false;
				break;
			}
	}
	if (!workable) 
		return false;
	std::pair<int, int> centre = std::make_pair((CITY_SIZE - buildingheight) / 2, (CITY_SIZE - buildingwidth) / 2);
	Rectangle cbounds = newbuilding->bounds;
	std::pair<int, int> initial = std::make_pair(cbounds.ylow(), cbounds.xlow());
	Rectangle nbounds = cbounds;
	if (centre.second == initial.second) {
		while (workable && cbounds.ylow() != centre.first) {
			cbounds = nbounds;
			nbounds += std::make_pair(SIGN(centre.first - initial.first), 0);
			for (unsigned int q = 0; q < number_of_buildings(); q++)
				if (overlap(building_at(q)->bounds, nbounds)) {
					workable = false;
					break;
				}
		} ;
	} else if (centre.first == initial.first) {
		while (workable && cbounds.xlow() != centre.second) {
			cbounds = nbounds;
			nbounds += std::make_pair(0, SIGN(centre.second - initial.second));
			for (unsigned int q = 0; q < number_of_buildings(); q++)
				if (overlap(building_at(q)->bounds, nbounds)) {
					workable = false;
					break;
				}
		}
	} else  {
		int deltay = centre.first - initial.first;
		int deltax = centre.second - initial.second;
		double error = 0;
		if (ABS(deltay) > ABS(deltax)) {
			double deltaerror = ABS((double)deltax / (double)deltay);
			std::pair<int, int> majorchange = std::make_pair(SIGN(deltay), 0);
			std::pair<int, int> minorchange = std::make_pair(0, SIGN(deltax));
			while (workable && cbounds.ylow() != centre.first) {
				cbounds = nbounds;
				error += deltaerror;
				nbounds += majorchange;
				while (error >= 0.5) {
					error -= 1.0;
					nbounds += minorchange;
				}
				for (unsigned int q = 0; q < number_of_buildings(); q++)
					if (overlap(building_at(q)->bounds, nbounds)) {
						workable = false;
						break;
					}
			}

		} else {
			double deltaerror = ABS((double)deltay / (double)deltax);
			std::pair<int, int> majorchange = std::make_pair(0, SIGN(deltax));
			std::pair<int, int> minorchange = std::make_pair(SIGN(deltay), 0);
			while (workable && cbounds.xlow() != centre.second) {
				cbounds = nbounds;
				error += deltaerror;
				nbounds += majorchange;
				while (error >= 0.5) {
					error -= 1.0;
					nbounds += minorchange;
				}
				for (unsigned int q = 0; q < number_of_buildings(); q++)
					if (overlap(building_at(q)->bounds, nbounds)) {
						workable = false;
						break;
					}
			}
		}
	}
	newbuilding->move_to(std::make_pair(cbounds.ylow(), cbounds.xlow()));

	if (!add_house_object(newbuilding)) {
		delete newbuilding;
		return false;
	}
	return true;
}
