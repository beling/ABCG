#ifndef MAIN_INIT_H_
#define MAIN_INIT_H_

#include "libs/Menu.h"
#include "libs/camera2d.h"
#include "libs/timer.h"


Timer timer;
Camera2d camera(50.0, 0.0, 3.0);

Menu menu(camera, 10, 10);

World world;
LevelsProvider levels;

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

void init_menu() {
	menu.add_glut_menu("level", read_level_menu());
}

#endif /*MAIN_INIT_H_*/
