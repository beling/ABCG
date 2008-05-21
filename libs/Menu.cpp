#include "Menu.h"

#include <GL/gl.h>

void bitmap_output(int x, int y, const std::string string, void *font)
{
	//glPushMatrix();
	//gluOrtho2D(0.0, 0.0, 600.0, 400.0);
	glRasterPos2f(x, y);
	for (unsigned i = 0; i < string.length(); i++) {
		glutBitmapCharacter(font, string[i]);
	}
	//glPopMatrix();
}

void
stroke_output(int x, int y, const char *format/*,...*/)
{
  /*va_list args;
  char buffer[1024];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);*/
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(2.0);
  
  glTranslatef(x, y, 0);
  //glScalef(0.005, 0.005, 0.005);
  for (const char * p = format; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
}

Menu::Menu(const Camera2d& camera, const int left, const int top)
: camera(camera), _left(left), _top(top), last_mouse_over_btn_nr(-1) {
}

Menu::~Menu()
{
}

int Menu::xy_to_btn(int x, int y) const {
	if (y < top() || y > bottom() || x < left() || x >= right()) return -1;
	return (x - left()) / btn_width;
}

bool Menu::mouse_move_evt(int x, int y) {
	int new_btn = xy_to_btn(x, y);
	if (new_btn != last_mouse_over_btn_nr) {
		if (last_mouse_over_btn_nr >= 0) buttons[last_mouse_over_btn_nr]->on_mouse_escape();
		if (new_btn >= 0) buttons[new_btn]->on_mouse_enter();
		last_mouse_over_btn_nr = new_btn;
	}
	return new_btn != -1;
}

bool Menu::mouse_evt(int button, int state, int x, int y) {
	const bool was_in_menu = last_mouse_over_btn_nr >= 0;
	return mouse_move_evt(x, y) || was_in_menu;
	//mouse click??
}

void Menu::add(Button* button) {
	buttons.push_back(button);
}

void Menu::draw() {
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, camera.w, 0.0, camera.h);
	
	camera.normalLine();
	for (unsigned i = 0; i < buttons.size(); ++i) {
		glColor4f(0.3, 0.3, 0.6, 0.4);
		glBegin(GL_QUADS);
			glVertex2f(left() + i * btn_width, camera.h - top());
			glVertex2f(left() + (i+1) * btn_width, camera.h - top());
			glVertex2f(left() + (i+1) * btn_width, camera.h - bottom());
			glVertex2f(left() + i * btn_width, camera.h - bottom());
		glEnd();
		glColor3f(1.0, 0.0, 0.0);
		//bitmap_output(left() + i * btn_width, top(), buttons[i]->text);
		bitmap_output(left() + i * btn_width + 10, camera.h - bottom() + 8, buttons[i]->text);
	}
	glPopMatrix();
}
