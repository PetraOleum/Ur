/*
 * =====================================================================================
 *
 *       Filename:  ur_curses.h
 *
 *    Description:  Central header file for all the ncurses stuff
 *
 *        Version:  1.0
 *        Created:  13/04/16 20:56:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petra (P.L.), oleumpetra@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  UR_CURSES_INC
#define  UR_CURSES_INC

#define PERSON_CHAR '@'
#define PERSON_COLOUR COLOR_PAIR(2)

#include <ncurses.h>
#include "ur_common.h"
#include "rectangle.h"
#include "city.h"

Rectangle displaybounds;

void initcolors() {
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_YELLOW);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_CYAN);
	init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(8, COLOR_RED, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_YELLOW);
	init_pair(10, COLOR_CYAN, COLOR_BLACK);
}

void start_curses() {
	initscr();
	start_color();
	initcolors();
	cbreak();
	noecho();
	curs_set(0);
	displaybounds = Rectangle((CITY_SIZE - LINES) / 2, (CITY_SIZE - COLS) / 2, LINES - 1, COLS - 1);
}

void stop_curses() {
	endwin();
}

inline char environment_object_symbol(EnvironmentObject _ob) {
	switch (_ob) {
		case EnvironmentObject::Nothingness:
			return '~';
		case EnvironmentObject::OpenGround:
			return ' ';
		case EnvironmentObject::Floor:
			return '.';
		case EnvironmentObject::OutsideWall:
			return ' ';
		case EnvironmentObject::InsideWall:
			return ' ';
		case EnvironmentObject::Door:
			return '+';
	}
	return '\0';
}

inline int environment_object_colour(EnvironmentObject _ob) {
	switch (_ob) {
		case EnvironmentObject::Nothingness:
			return COLOR_PAIR(4);
		case EnvironmentObject::OpenGround:
			return COLOR_PAIR(7);
		case EnvironmentObject::Floor:
			return COLOR_PAIR(10);
		case EnvironmentObject::OutsideWall:
			return COLOR_PAIR(4);
		case EnvironmentObject::InsideWall:
			return COLOR_PAIR(3);
		case EnvironmentObject::Door:
			return COLOR_PAIR(9);
	}
	return COLOR_PAIR(7);
}

void updatemap(const City &_city) {
	displaybounds = Rectangle(displaybounds.ylow(), displaybounds.xlow(), LINES - 1, COLS - 1);
	erase();
	for (int y = 0; y < displaybounds.height(); y++)
		for (int x = 0; x < displaybounds.width(); x++) {
			EnvironmentObject objat = _city.get(y + displaybounds.ylow(), x + displaybounds.xlow());
			attron(environment_object_colour(objat));
			mvaddch(y, x, environment_object_symbol(objat));
		}
	attron(PERSON_COLOUR);
	for (unsigned int i = 0; i < _city.number_of_people(); i++) {
		std::pair<int, int> p_pos = _city.get_person(i);
		if (displaybounds.contains(p_pos))
				mvaddch(p_pos.first, p_pos.second, PERSON_CHAR);
	}
	attron(COLOR_PAIR(6));
	refresh();
}



#endif   /* ----- #ifndef UR_CURSES_INC  ----- */
