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
#define PERSON_COLOUR COLOR_PAIR(11)
//#define MOVE_RATE 1


#include <ncurses.h>
#include "ur_common.h"
#include "rectangle.h"
#include "city.h"

Rectangle displaybounds;
bool curses_running = false;

void initcolors() ;
void start_curses() ;
void stop_curses() ;
void stop_curses() ;
inline char environment_object_symbol(EnvironmentObject _ob) ;
inline int environment_object_colour(EnvironmentObject _ob) ;
void refreshmap(City &_city) ;
void updatemap(City &_city) ;
inline char furniture_char(const Furniture& _f);

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
	init_pair(11, COLOR_RED, COLOR_GREEN);
}

void start_curses() {
	if (curses_running)
		return;
	initscr();
	start_color();
	initcolors();
	cbreak();
//	halfdelay(MOVE_RATE);
	timeout(10);
	noecho();
	curs_set(0);
	displaybounds = Rectangle((CITY_SIZE - LINES) / 2, (CITY_SIZE - COLS) / 2, LINES - 1, COLS - 1);
	curses_running = true;
}

void stop_curses() {
	if (curses_running) {
		timeout(-1);
		nocbreak();
		endwin();
	}
	curses_running = false;
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

void refreshmap(City &_city) {
	_city.clear_unprocessed_points();
	displaybounds = Rectangle(displaybounds.ylow(), displaybounds.xlow(), LINES - 1, COLS - 1);
	erase();
	for (int y = 0; y < displaybounds.height(); y++)
		for (int x = 0; x < displaybounds.width(); x++) {
			EnvironmentObject objat = _city.get(y + displaybounds.ylow(), x + displaybounds.xlow());
			attron(environment_object_colour(objat));
			mvaddch(y, x, environment_object_symbol(objat));
			Furniture fobj = _city.junk_get(std::make_pair(y + displaybounds.ylow(), x + displaybounds.xlow()));
			if (fobj != Furniture::None)
				mvaddch(y, x, furniture_char(fobj));
		}
	attron(PERSON_COLOUR);
	for (unsigned int i = 0; i < _city.number_of_people(); i++) {
		std::pair<int, int> p_pos = _city.get_person(i);
		if (displaybounds.contains(p_pos))
				mvaddch(p_pos.first - displaybounds.ylow(), p_pos.second - displaybounds.xlow(), PERSON_CHAR);
	}
	attron(COLOR_PAIR(6));
	refresh();
}

void updatemap(City &_city) {
	while (_city.points_unprocessed()) {
		point pt = _city.pop_unprocessed_points();
		if (!displaybounds.contains(pt))
			continue;
		EnvironmentObject eobj = _city.get(pt.first, pt.second);
		attron(environment_object_colour(eobj));
		mvaddch(pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), environment_object_symbol(eobj));
		Furniture fobj = _city.junk_get(pt);
		if (fobj != Furniture::None)
			mvaddch(pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), furniture_char(fobj));
		if (_city.point_hasperson(pt)) {
			attron(PERSON_COLOUR);
			mvaddch(pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), PERSON_CHAR);
		}
	}
	attron(COLOR_PAIR(6));
	refresh();
}

inline char furniture_char(const Furniture& _f) {
	switch (_f) {
		case Furniture::None:
			return ' ';
		case Furniture::Bed:
			return 'B';
		case Furniture::Dresser:
			return 'A';
		case Furniture::Table:
			return 'T';
		case Furniture::Barrel:
			return '%';
		case Furniture::Chair:
			return 'h';
		case Furniture::Basin:
			return 'U';
		default:
			throw std::out_of_range("furniture_char() given invalid furniture object. std::out_of_range");
	}
}



#endif   /* ----- #ifndef UR_CURSES_INC  ----- */
