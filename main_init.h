/*
    ABCG (Another Bridge Construct Game)
    Copyright (C) 2010  Piotr Beling

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * This file should be only include in main.cpp.
 */

#ifndef MAIN_INIT_H_
#define MAIN_INIT_H_

#include "libs/Menu.h"
#include "libs/camera2d.h"
#include "libs/timer.h"
#include <sstream>


Timer timer;
Camera2d camera(50.0, 0.0, 3.0);

Menu menu(10, 10);

World world;
LevelsProvider levels;

///Czy rysowaæ siatkê w trybie edycji?
bool grid = true;

int current_level_id = 0;

enum Mode {
	m_anim, m_pause, m_edit //czy jesteœmy w trybie: 0-animacji, 1-znimacji (pause), 2-edycji
} mode = m_edit;

vec2d<double> linkStart(NAN, NAN), linkEnd(NAN, NAN); //jeœli rysujemy link to wyznacza jego wsp.

void camera_changed() {
	soundProvider().updateListenerPosition(vec2d<double>(camera.x, camera.y));
}

void refresh_world() {
	glutPostRedisplay();
}

void level_choose(int value) {
	current_level_id = value;
    std::ifstream f(levels.file_name(value).c_str());
    f >> world;
    f.close();
}

void load_current_level() {
	level_choose(current_level_id);
}

void save_current_level() {
	std::ofstream f(levels.file_name(current_level_id).c_str());
	f << world;
	f.close();
}

void next_level() {
	level_choose(levels.next_level_id(current_level_id));
	refresh_world();
}

void prev_level() {
	level_choose(levels.prev_level_id(current_level_id));
	refresh_world();
}

int read_level_menu() {
	int menu = glutCreateMenu(0);

	for (int e = 0; e < levels.episodes_len(); ++e) {
		int submenu = glutCreateMenu(level_choose);
		for (int l = 0; l < levels.episode(e).levels_len(); ++l)
			glutAddMenuEntry(levels.episode(e).level(l).c_str(), levels.id(e, l));
		glutSetMenu(menu);
		glutAddSubMenu(levels.episode(e).name.c_str(), submenu);
	}
	return menu;
}

void just_exit() { exit(0); }

void grid_on_off() {
	grid = !grid;
	glutPostRedisplay();
}

void clear() {
    if (mode != m_edit) return;
    world.clear();
    glutPostRedisplay();
}

void start_stop_simulation() {
    linkStart.x = NAN; //anulujemy rysowanie linka
    if (mode == m_edit) {
          world.start();
          mode = m_anim;
    } else {
          mode = m_edit;
          world.stop();
          glutPostRedisplay();
    }
}

struct MoneyText: public Button {

	virtual std::string text() const { std::stringstream s; s << "$" << world.money_left(); return s.str(); }

	virtual int length() const { return 120; };

} money_text;

void init_menu() {
	menu.add_fun("<", prev_level);
	menu.add_glut_menu("level", read_level_menu());
	menu.add_fun(">", next_level);
	menu.add_space();
	menu.add_fun("grid on/off", grid_on_off);
	menu.add_space(30);
	menu.add_fun("start", start_stop_simulation);
	menu.add(&money_text);
	menu.add_space(30);
	menu.add_fun("save", save_current_level);
	menu.add_fun("clear", clear);
	menu.add_fun("exit", just_exit);
}

#endif /*MAIN_INIT_H_*/
