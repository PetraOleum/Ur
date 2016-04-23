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

	for (int i = 0; i < 20; i++)
		city.add_building(Building::Mansion);

	for (int i = 0; i < 350; i++)
		city.add_building(Building::Cottage);

	for (int i = 0; i < 80; i++)
		city.add_building(Building::Apartment);

	for (int i = 0; i < 80; i++)
		city.add_building(Building::Shop);

	for (int i = 0; i < 40; i++)
		city.add_building(Building::Warehouse);

	for (int i = 0; i < 30; i++)
		city.add_building(Building::Tavern);

	for (int i = 0; i < 1000; i++)
		city.add_building(Building::Hovel);

	city.add_people(30);

	for (int i = 0; i < 1000; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Barrel);
	}
	for (int i = 0; i < 1000; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Bed);
	}
	for (int i = 0; i < 1000; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Table);
	}
	for (int i = 0; i < 1000; i++) {
		point pt;
		do {
			pt = std::make_pair(rand() % CITY_SIZE, rand() % CITY_SIZE);
		} while (city.get(pt.first, pt.second) != EnvironmentObject::OpenGround || city.junk_get(pt) != Furniture::None);
		city.junk_set(pt, Furniture::Chair);
	}
	for (int i = 0; i < 1000; i++) {
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
				displaybounds = Rectangle((CITY_SIZE - LINES) / 2, (CITY_SIZE - COLS) / 2, LINES - 1, COLS - 1);
				refreshmap(city);
				break;
			case 'r':
//				displaybounds = Rectangle(rand() % (CITY_SIZE - LINES), rand() % (CITY_SIZE - COLS), LINES - 1, COLS - 1);
				refreshmap(city);
				break;
			case 'f':
				{
					unsigned int rselect = rand() % city.number_of_people();
					displaybounds = Rectangle(city.get_person(rselect).first - LINES / 2, city.get_person(rselect).second - COLS / 2, LINES - 1, COLS - 1);
					refreshmap(city);

				}
				break;
			case '.':
				city.step();
				updatemap(city);
				break;
			default:
				break;
		}
	}

	stop_curses();
//	refresh();

//	Rectangle screen_area(std::make_pair(1, 1), std::make_pair(LINES - 2, COLS - 1));

//	do {
//	erase();
//
//	House testhouse;
//
////	int attempts = 0;
////	while (testhouse.valid() && (testhouse.floor_area() < FILL_AREA) && attempts < 1000000) {
////		attempts++;
////		Rectangle retest(rand() % LINES, rand() % COLS, rand() % 15 + rand() % 15 + 3, rand() % 15 + rand() % 15 + 3);
////		if (!testhouse.bounds.contains(retest))
////			continue;
//////		if (testhouse.overlaps(retest))
//////			continue;
//////		if (testhouse.contact_length(retest) < 9 && testhouse.number_of_rectangles() != 0)
//////			continue;
////		if (testhouse.number_of_rectangles() != 0 && testhouse.interlock_length(retest) < 9)
////			continue;
////
////		testhouse += retest;
////	}
////	mvprintw(0, 0, "Validity: %d; floor area: %d; attempts: %d; number of rectangles: %d; FA: %d", testhouse.valid(), testhouse.accurate_floor_area(), attempts, testhouse.number_of_rectangles(), FILL_AREA);
//
////	std::cout << "test" << std::endl;
//	if (!testhouse.create(Building::Shop))
//		std::cout << "failed" << std::endl;
//	else {
//		testhouse.changebounds();
//		testhouse.move_to(std::make_pair(1, 1));
//	}
//
//	refresh();
//
//	std::vector<std::pair<int, int> > * harea = testhouse.floor_locations();
//	attron(COLOR_PAIR(5));
//	for (unsigned int q = 0; q < harea->size(); q++)
//		mvaddch(harea->at(q).first, harea->at(q).second, 'A');
//	attroff(COLOR_PAIR(5));
//	delete harea;
//	refresh();
//	
////	while (getch() != 'n') ;
//
////	for (unsigned int i = 0; i < testhouse.number_of_rectangles(); i++) {
////		attron(COLOR_PAIR(i % 2 + 5));
////		std::vector<std::pair<int,int> > * recc = testhouse[i].contents();
////		for (unsigned int q = 0; q < recc->size(); q++)
////			mvaddch(recc->at(q).first, recc->at(q).second, (char)('A' + i % 3));
////		attroff(COLOR_PAIR(i % 2 + 5));
////		delete recc;
////	}
////	refresh();
//
////	while (getch() != 'n') ;
//
//
//
////	testhouse.fill_with_rooms_2();
//
////	std::cout << "filled " << std::endl;
//	for (unsigned int i = 0; i < testhouse.number_of_rooms(); i++) {
//		attron(COLOR_PAIR(1 + i % 3));
//		std::vector<std::pair<int, int> > * recc = testhouse.room_at(i).contents();
//		for (unsigned int q = 0; q < recc->size(); q++)
//			mvaddch(recc->at(q).first, recc->at(q).second, (char)('P' + i % 2));
//		attroff(COLOR_PAIR(1 + i % 3));
//		delete recc;
//	}
//
//	refresh();
//
////	while (getch() != 'n') ;
//
//	for (unsigned int i = 0; i < testhouse.number_of_rooms(); i++) {
//		attron(COLOR_PAIR(6));
//		std::vector<std::pair<int, int> > * recc = testhouse.room_at(i).perimeter();
//		for (unsigned int q = 0; q < recc->size(); q++)
//			mvaddch(recc->at(q).first, recc->at(q).second, '*');
//		attroff(COLOR_PAIR(6));
//		delete recc;
//	}
//	refresh();
//
////	while (getch() != 'n') ;
//
//	std::vector<std::pair<int, int> > * hperim = testhouse.perimeter();
//
//	attron(COLOR_PAIR(4));
//	for (unsigned int q = 0; q < hperim->size(); q++)
//		mvaddch(hperim->at(q).first, hperim->at(q).second, 'X');
//	attroff(COLOR_PAIR(4));
//	delete hperim;
//	refresh();
//
////	while (getch() != 'n') ;
//
////	testhouse.add_doors();
////	testhouse.add_outside_doors();
//
//	std::vector<std::pair<int, int> > * hdoors = testhouse.get_doors();
//
//	attron(COLOR_PAIR(5));
//	for (unsigned int q = 0; q < hdoors->size(); q++)
//		mvaddch(hdoors->at(q).first, hdoors->at(q).second, '+');
//	attroff(COLOR_PAIR(5));
//	delete hdoors;
//	refresh();
//
//	} while (getch() != 'q') ;
//
///* 	Rectangle rec1(std::make_pair(rand() % LINES, rand() % COLS), std::make_pair(rand() % LINES, rand() % COLS));
// * 
// * 	std::vector<std::pair<int, int> > * rec1_contents = rec1.perimeter();
// * 
// * 	attron(COLOR_PAIR(1));
// * 	for (unsigned int i = 0; i < rec1_contents->size(); i++)
// * 		mvaddch(rec1_contents->at(i).first, rec1_contents->at(i).second, 'A');
// * 	attroff(COLOR_PAIR(1));
// * 
// * 	refresh();
// * 
// * 	delete rec1_contents;
// * 	
// * 	while (getch() != 'n') ;
// * 
// * 
// * 	Rectangle rec2(std::make_pair(rec1.yhigh() + 1, rand() % COLS), std::make_pair(rand() % LINES, rand() % COLS));
// * 
// * 	
// * 	std::vector<std::pair<int, int> > * rec2_contents = rec2.perimeter();
// * 
// * 	
// * 	attron(COLOR_PAIR(2));
// * 	for (unsigned int i = 0; i < rec2_contents->size(); i++)
// * 		mvaddch(rec2_contents->at(i).first, rec2_contents->at(i).second, 'B');
// * 	attroff(COLOR_PAIR(2));
// * 
// * 	
// * 	refresh();
// * 
// * 	delete rec2_contents;
// * 
// * //	if (overlap(rec1, rec2)) {
// * 		while (getch() != 'n') ;
// * 
// * //		Rectangle rec3(rec1, rec2);
// * 
// * 
// * 		std::vector<std::pair<int, int> > * rec3_contents = rec2.contact(rec1);
// * 
// * //		attron(COLOR_PAIR(3));
// * 		if (!rec3_contents->empty())
// * 		for (unsigned int i = 1; i < (rec3_contents->size() - 1); i++)
// * 			mvaddch(rec3_contents->at(i).first, rec3_contents->at(i).second, ' ');
// * //		attroff(COLOR_PAIR(3));
// * //
// * 		std::vector<std::pair<int, int> > * pvec4 = rec1.contact(rec2);
// * 
// * 		if (!pvec4->empty())
// * 			for (unsigned int i = 1; i < (pvec4->size() - 1); i++)
// * 				mvaddch(pvec4->at(i).first, pvec4->at(i).second, ' ');
// * 		
// * 		refresh();
// * 		
// * 		delete rec3_contents;
// * 		delete pvec4;
// * //	}
// */
//
////	while (getch() != 'n') ;
//
//	endwin();


	return 0;
}



