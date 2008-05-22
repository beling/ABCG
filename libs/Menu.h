#ifndef MENU_H_
#define MENU_H_

#include <string>
#include <vector>

#include <GL/glut.h>

#include "camera2d.h"

void bitmap_output(int x, int y, const std::string string, void *font = GLUT_BITMAP_HELVETICA_18);
void stroke_output(int x, int y, const char *format);

struct Button {
	
	virtual void on_mouse_enter() {};
	
	virtual void on_mouse_escape() {};
	
	virtual void on_click() {};
	
	std::string text;
	
	Button(const std::string& text): text(text) {}
	
};

struct GlutMenuButton: public Button {
	
	int glut_menu_nr;
	
	GlutMenuButton(const std::string& text, int glut_menu_nr): Button(text), glut_menu_nr(glut_menu_nr) {}
	
	virtual void on_mouse_enter() {
		glutSetMenu(glut_menu_nr);
		glutAttachMenu(GLUT_LEFT_BUTTON);
	}
	
	virtual void on_mouse_escape() {
		glutSetMenu(glut_menu_nr);
		glutDetachMenu(GLUT_LEFT_BUTTON);
	}

};

struct SimpleFuncButton: public Button {
	
	void (*fun)(void);
	
	SimpleFuncButton(const std::string& text, void (*fun)(void)): Button(text), fun(fun) {}
	
	virtual void on_click() { fun(); }
	
};

class Menu {
	
	const Camera2d& camera;
	
	int _left, _top, last_mouse_over_btn_nr;
	
	static const int btn_width = 100;
	
	static const int btn_height = 30;
	
	std::vector<Button*> buttons;
	
public:
	
	Menu(const Camera2d& camera, const int left, const int top);
	
	virtual ~Menu();
	
	int left() const { return _left; }
	
	int top() const { return _top; }
	
	int right() const { return _left + btn_width * buttons.size(); }
	
	int bottom() const { return _top + btn_height; }
	
	/**
	 * @return -1 - no button; 0, 1, ... - button 0, 1, ...
	 */
	int xy_to_btn(int x, int y) const;
	
	/**
	 * @return true <=> mouse is over menu
	 */
	bool mouse_evt(int button, int state, int x, int y);
	
	bool mouse_move_evt(int x, int y);
	
	/**
	 * Add button to menu.
	 */
	void add(Button* button);
	
	void add_glut_menu(const std::string& text, int menu_id) { add(new GlutMenuButton(text, menu_id)); }
	
	void add_fun(const std::string& text, void (*fun)(void)) { add(new SimpleFuncButton(text, fun)); }
	
	void draw();
};

#endif /*MENU_H_*/
