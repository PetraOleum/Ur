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

	for (int i = 0; i < 30; i++)
		city.add_building(Building::Mansion);

	for (int i = 0; i < 100; i++)
		city.add_building(Building::Cottage);

	for (int i = 0; i < 20; i++)
		city.add_building(Building::Apartment);

	for (int i = 0; i < 50; i++)
		city.add_building(Building::Shop);

	for (int i = 0; i < 30; i++)
		city.add_building(Building::Warehouse);

	for (int i = 0; i < 30; i++)
		city.add_building(Building::Tavern);

	for (int i = 0; i < 300; i++)
		city.add_building(Building::Hovel);

	for (int i = 0; i < 50; i++)
		city.add_building(Building::Field);

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

	int ch;

	start_curses();
	refreshmap(city);

	while ((ch = wgetch(main_window)) != 'q') {
		switch (ch) {
			case 'h':
			case KEY_LEFT:
				displaybounds += std::make_pair(0, -5);
				refreshmap(city);
				break;
			case 'l':
			case KEY_RIGHT:
				displaybounds += std::make_pair(0, 5);
				refreshmap(city);
				break;
			case 'j':
			case KEY_DOWN:
				displaybounds += std::make_pair(5, 0);
				refreshmap(city);
				break;
			case 'k':
			case KEY_UP:
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
			case KEY_HOME:
				centre_on(std::make_pair(CITY_SIZE / 2, CITY_SIZE / 2));
				refreshmap(city);
				break;
			case 'r':
			case KEY_RESIZE:
			case '':
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
			case '?':
				show_help();
				break;
			default:
				break;
		}
	}

	stop_curses();

	return 0;
}



