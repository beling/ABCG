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

enum Mode {
	m_anim, m_pause, m_edit //czy jesteœmy w trybie: 0-animacji, 1-znimacji (pause), 2-edycji
} mode = m_edit;

void level_choose(int value) {
    std::ifstream f(levels.file_name(value).c_str());
    f >> world;
    f.close();
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

struct MoneyText: public Button {
	
	virtual std::string text() const { std::stringstream s; s << "$" << world.money_left(); return s.str(); }
	
	virtual int length() const { return 120; };
	
} money_text;

void init_menu() {
	menu.add_glut_menu("level", read_level_menu());
	menu.add_fun("grid on/off", grid_on_off);
	menu.add_space(40);	
	menu.add(&money_text);
	menu.add_space(40);
	menu.add_fun("exit", just_exit);
}

#endif /*MAIN_INIT_H_*/
