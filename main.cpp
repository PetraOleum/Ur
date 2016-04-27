#include <ncurses.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include "rectangle.h"
#include "house.h"
#include <vector>
#include <utility>
#include "ur_curses.h"
#include "city.h"


void initcolors();

int main() {
	srand(time(NULL));


	City city;

	while (!city.add_building(Building::Palace)) ;

	for (int i = 0; i < 40; i++)
		city.add_building(Building::Mansion);

	for (int i = 0; i < 200; i++)
		city.add_building(Building::Cottage);

	for (int i = 0; i < 150; i++)
		city.add_building(Building::Apartment);

	for (int i = 0; i < 80; i++)
		city.add_building(Building::Shop);

	for (int i = 0; i < 40; i++)
		city.add_building(Building::Warehouse);

	for (int i = 0; i < 30; i++)
		city.add_building(Building::Tavern);

	for (int i = 0; i < 800; i++)
		city.add_building(Building::Hovel);

	city.add_people(100);

	for (int i = 0; i < 1500; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Barrel);
	}
	for (int i = 0; i < 1500; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Bed);
	}
	for (int i = 0; i < 1500; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Table);
	}
	for (int i = 0; i < 1500; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Chair);
	}
	for (int i = 0; i < 1500; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Basin);
	}

	char ch;

	start_curses();
	refreshmap(city);


	while ((ch = getch()) != 'q') {
		switch (ch) {
			case 'h':
				displaybounds += std::make_pair(0, -5);
				refreshmap(city);
				break;
			case 'l':
				displaybounds += std::make_pair(0, 5);
				refreshmap(city);
				break;
			case 'j':
				displaybounds += std::make_pair(5, 0);
				refreshmap(city);
				break;
			case 'k':
				displaybounds += std::make_pair(-5, 0);
				refreshmap(city);
				break;
			case 'y':
				displaybounds += std::make_pair(-5, -5);
				refreshmap(city);
				break;
			case 'u':
				displaybounds += std::make_pair(-5, 5);
				refreshmap(city);
				break;
			case 'b':
				displaybounds += std::make_pair(5, -5);
				refreshmap(city);
				break;
			case 'n':
				displaybounds += std::make_pair(5, 5);
				refreshmap(city);
				break;
			case ' ':
				centre_on(std::make_pair(CITY_SIZE / 2, CITY_SIZE / 2));
				refreshmap(city);
				break;
			case 'r':
				refreshmap(city);
				break;
			case 'a':
				for (unsigned int i = 0; i < 10; i++){
					point pt;
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Barrel);
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Chair);
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Dresser);
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Table);
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Bed);
					do {
						pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
					} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
					city.junk_set(pt, Furniture::Basin);
					updatemap(city);
				}

				break;
			case 'f':
				{
					unsigned int rselect = rand() % city.number_of_people();
					centre_on(city.get_person(rselect));
					refreshmap(city);

				}
				break;
			case 'p':
				toggleTimeout();
				break;
			case ERR:
			case '.':
				city.step();
				updatemap(city);
				break;
			default:
				break;
		}
	}

	stop_curses();

	return 0;
}



