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

#define TOP_WIN_HEIGHT 1
#define TOP_WIN_WIDTH COLS
#define TOP_WIN_Y 0
#define TOP_WIN_X 0
#define MAIN_WIN_HEIGHT (LINES - TOP_WIN_HEIGHT - STATUS_WIN_HEIGHT)
#define MAIN_WIN_WIDTH COLS
#define MAIN_WIN_Y TOP_WIN_HEIGHT
#define MAIN_WIN_X 0
#define STATUS_WIN_HEIGHT 1
#define STATUS_WIN_WIDTH COLS
#define STATUS_WIN_Y (MAIN_WIN_Y + MAIN_WIN_HEIGHT)
#define STATUS_WIN_X 0
#define HELP_WIN_HEIGHT 20
#define HELP_WIN_WIDTH 35
#define HELP_WIN_Y ((LINES - HELP_WIN_HEIGHT) / 2)
#define HELP_WIN_X ((COLS - HELP_WIN_WIDTH) / 2)

#include <ncurses.h>
#include <panel.h>
#include "ur_common.h"
#include "rectangle.h"
#include "city.h"

Rectangle displaybounds;
bool curses_running = false;

WINDOW * main_window;
WINDOW * status_window;
WINDOW * top_window;
WINDOW * help_window;

PANEL * main_win_panel;
PANEL * status_win_panel;
PANEL * top_win_panel;
PANEL * help_win_panel;

enum class timeouttype_t {
	Wait = -1,
	NoWait = 0,
	Tick = 10
} TimeoutType;

void initcolors() ;
void start_curses() ;
void stop_curses() ;
inline char environment_object_symbol(EnvironmentObject _ob) ;
inline int environment_object_colour(EnvironmentObject _ob) ;
void refreshmap(City &_city) ;
void redrawtopwin();
void redrawstatuswin();
void updatemap(City &_city) ;
inline char furniture_char(const Furniture& _f);
void toggleTimeout();
void centre_on(point centre);
void init_help_win();
void show_help();

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
	init_pair(12, COLOR_BLACK, COLOR_WHITE);
}

void start_curses() {
	if (curses_running)
		return;
	initscr();
	TimeoutType = timeouttype_t::Tick;
	start_color();
	initcolors();
	cbreak();
	wtimeout(main_window, (int)TimeoutType);
	noecho();
	curs_set(0);
	top_window = newwin(TOP_WIN_HEIGHT, TOP_WIN_WIDTH, TOP_WIN_Y, TOP_WIN_X);
	main_window = newwin(MAIN_WIN_HEIGHT, MAIN_WIN_WIDTH, MAIN_WIN_Y, MAIN_WIN_X);
	status_window = newwin(STATUS_WIN_HEIGHT, STATUS_WIN_WIDTH, STATUS_WIN_Y, STATUS_WIN_X);
	help_window = newwin(HELP_WIN_HEIGHT, HELP_WIN_WIDTH, HELP_WIN_Y, HELP_WIN_X);
	help_win_panel = new_panel(help_window);
	main_win_panel = new_panel(main_window);
	top_win_panel = new_panel(top_window);
	status_win_panel = new_panel(status_window);
	hide_panel(help_win_panel);
	init_help_win();
	centre_on(std::make_pair(CITY_SIZE / 2, CITY_SIZE / 2));
	curses_running = true;
	update_panels();
	doupdate();
}

void stop_curses() {
	if (curses_running) {
		TimeoutType = timeouttype_t::Wait;
		wtimeout(main_window, (int)TimeoutType);
		delwin(top_window);
		delwin(main_window);
		delwin(status_window);
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
//	init_help_win();
	move_panel(help_win_panel, HELP_WIN_Y, HELP_WIN_X);
	redrawtopwin();
	redrawstatuswin();
	_city.clear_unprocessed_points();
	displaybounds = Rectangle(displaybounds.ylow(), displaybounds.xlow(), MAIN_WIN_HEIGHT - 1, MAIN_WIN_WIDTH - 1);
	delwin(main_window);
	main_window = newwin(MAIN_WIN_HEIGHT, MAIN_WIN_WIDTH, MAIN_WIN_Y, MAIN_WIN_X);
	replace_panel(main_win_panel, main_window);
	wtimeout(main_window, (int)TimeoutType);
	for (int y = 0; y < displaybounds.height(); y++)
		for (int x = 0; x < displaybounds.width(); x++) {
			EnvironmentObject objat = _city.get(y + displaybounds.ylow(), x + displaybounds.xlow());
			wattron(main_window, environment_object_colour(objat));
			mvwaddch(main_window, y, x, environment_object_symbol(objat));
			Furniture fobj = _city.junk_get(std::make_pair(y + displaybounds.ylow(), x + displaybounds.xlow()));
			if (fobj != Furniture::None)
				mvwaddch(main_window, y, x, furniture_char(fobj));
		}
	wattron(main_window, PERSON_COLOUR);
	for (unsigned int i = 0; i < _city.number_of_people(); i++) {
		std::pair<int, int> p_pos = _city.get_person(i);
		if (displaybounds.contains(p_pos))
				mvwaddch(main_window, p_pos.first - displaybounds.ylow(), p_pos.second - displaybounds.xlow(), PERSON_CHAR);
	}
	wattron(main_window, COLOR_PAIR(6));
//	wrefresh(main_window);
	update_panels();
	doupdate();
}

void updatemap(City &_city) {
	while (_city.points_unprocessed()) {
		point pt = _city.pop_unprocessed_points();
		if (!displaybounds.contains(pt))
			continue;
		EnvironmentObject eobj = _city.get(pt.first, pt.second);
		wattron(main_window, environment_object_colour(eobj));
		mvwaddch(main_window, pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), environment_object_symbol(eobj));
		Furniture fobj = _city.junk_get(pt);
		if (fobj != Furniture::None)
			mvwaddch(main_window, pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), furniture_char(fobj));
		if (_city.point_hasperson(pt)) {
			wattron(main_window, PERSON_COLOUR);
			mvwaddch(main_window, pt.first - displaybounds.ylow(), pt.second - displaybounds.xlow(), PERSON_CHAR);
		}
	}
	wattron(main_window, COLOR_PAIR(6));
//	wrefresh(main_window);
	update_panels();
	doupdate();
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


void toggleTimeout() {
	timeouttype_t ttemp;
	switch (TimeoutType) {
		case timeouttype_t::Wait:
			ttemp = timeouttype_t::NoWait;
			break;
		case timeouttype_t::Tick:
			ttemp = timeouttype_t::Wait;
			break;
		case timeouttype_t::NoWait:
		default:
			ttemp = timeouttype_t::Tick;
	}
	TimeoutType = ttemp;
	wtimeout(main_window, (int)TimeoutType);
	switch (TimeoutType) {
		case timeouttype_t::Tick:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "   TICK");
			break;
		case timeouttype_t::Wait:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "   WAIT");
			break;
		case timeouttype_t::NoWait:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "NO WAIT");
			break;
		default:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "???????");
	}
//	wrefresh(status_window);
	update_panels();
	doupdate();
}

void centre_on(point centre) {
	displaybounds = Rectangle(centre.first - LINES / 2, centre.second - COLS / 2, LINES - 1, COLS - 1);
}

void redrawtopwin() {
	delwin(top_window);
	top_window = newwin(TOP_WIN_HEIGHT, TOP_WIN_WIDTH, TOP_WIN_Y, TOP_WIN_X);
	replace_panel(top_win_panel, top_window);
	wbkgd(top_window, COLOR_PAIR(12));
	mvwhline(top_window, 0, 2, '=', TOP_WIN_WIDTH - 4);
	mvwprintw(top_window, 0, (TOP_WIN_WIDTH - 4) / 2, " UR ");
//	wrefresh(top_window);
}

void redrawstatuswin() {
	delwin(status_window);
	status_window = newwin(STATUS_WIN_HEIGHT, STATUS_WIN_WIDTH, STATUS_WIN_Y, STATUS_WIN_X);
	replace_panel(status_win_panel, status_window);
	wbkgd(status_window, COLOR_PAIR(12));
	switch (TimeoutType) {
		case timeouttype_t::Tick:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "   TICK");
			break;
		case timeouttype_t::Wait:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "   WAIT");
			break;
		case timeouttype_t::NoWait:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "NO WAIT");
			break;
		default:
			mvwprintw(status_window, 0, STATUS_WIN_WIDTH - 8, "???????");
	}
	mvwprintw(status_window, 0, 1, "<%d, %d> - <%d, %d>", displaybounds.ylow(), displaybounds.xlow(), displaybounds.yhigh(), displaybounds.xhigh());
	mvwprintw(status_window, 0, STATUS_WIN_WIDTH - (8 + 12), "[HELP: ?]");
//	wrefresh(status_window);
}
void init_help_win() {
	delwin(help_window);
	help_window = newwin(HELP_WIN_HEIGHT, HELP_WIN_WIDTH, HELP_WIN_Y, HELP_WIN_X);
	replace_panel(help_win_panel, help_window);
	box(help_window, 0, 0);
	mvwprintw(help_window, 0, (HELP_WIN_WIDTH - 6) / 2, " HELP ");
	mvwprintw(help_window, 3, 3,  "Move Window          hjklyubn");
	mvwprintw(help_window, 5, 3,  "Centre Window         <space>");
	mvwprintw(help_window, 7, 3,  "Centre Random Being         f");
	mvwprintw(help_window, 9, 3,  "Add Furniture               a");
	mvwprintw(help_window, 11, 3, "Toggle Timeout Style        p");
	mvwprintw(help_window, 13, 3, "Step                        .");
	mvwprintw(help_window, 15, 3, "Help                        ?");
	mvwprintw(help_window, 17, 3, "Quit                        q");
}

void show_help() {
	show_panel(help_win_panel);
	update_panels();
	doupdate();
	char ch;
	do
		ch = wgetch(help_window);
	while (ch != 'q' && ch != '?');
	hide_panel(help_win_panel);
	update_panels();
	doupdate();
}
#endif   /* ----- #ifndef UR_CURSES_INC  ----- */
