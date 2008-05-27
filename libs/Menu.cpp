#include "Menu.h"

#include <GL/gl.h>

void bitmap_output(int x, int y, const std::string& string, void *font)
{
	glRasterPos2f(x, y);
	for (unsigned i = 0; i < string.length(); i++)
		glutBitmapCharacter(font, string[i]);
}

unsigned bitmap_output_width(const std::string& string, void *font) {
	unsigned result = 0;
	for (unsigned i = 0; i < string.length(); i++) result += glutBitmapWidth(font, string[i]);
	return result;
}

void nice_output(int x, int y, const std::string& string, int bg_width, void *font) {
	glPushMatrix();
	glLoadIdentity();
	const double h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, h);
	y = h - y;
	if (BG_NONE != bg_width) {
		if (BR_AUTO_WIDTH == bg_width) bg_width = bitmap_output_width(string, font) + 20;
		glColor4f(0.3, 0.3, 0.6, 0.4);
		glBegin(GL_QUADS);
			glVertex2f(x, y + 30);
			glVertex2f(x + bg_width, y + 30);
			glVertex2f(x + bg_width, y);
			glVertex2f(x, y);
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 0.1f);
	bitmap_output(x + 10, y + 8, string);
	
	glPopMatrix();
}

Menu::Menu(const int left, const int top)
: _left(left), _top(top), last_mouse_over_btn_nr(-1) {
}

Menu::~Menu()
{
}

int Menu::xy_to_btn(int x, int y) const {
	if (y < top() || y > bottom()) return -1;
	int btn_start = left();
	for (unsigned i = 0; i < buttons.size(); ++i) {
		if (x < btn_start) return -1;
		btn_start += buttons[i]->length();
		if (x < btn_start) return i;
		btn_start += btn_pad;
	}
	return -1;
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int new_btn = xy_to_btn(x, y);
		if (new_btn != -1)
			buttons[new_btn]->on_click();
	}
	return mouse_move_evt(x, y) || was_in_menu;
}

void Menu::add(Button* button) {
	buttons.push_back(button);
}

void Menu::draw() {
	glPushMatrix();
	glLoadIdentity();
	const double w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0.0, w, 0.0, h);
	
	int btn_start = left();
	for (unsigned i = 0; i < buttons.size(); ++i) {
		const int btn_width = buttons[i]->length();
		const std::string& text = buttons[i]->text();
		if (!text.empty()) {
		glColor4f(0.3, 0.3, 0.6, 0.4);
		glBegin(GL_QUADS);
			glVertex2f(btn_start, h - top());
			glVertex2f(btn_start + btn_width, h - top());
			glVertex2f(btn_start + btn_width, h - bottom());
			glVertex2f(btn_start, h - bottom());
		glEnd();
		glColor3f(1.0f, 1.0f, 0.1f);
		//bitmap_output(left() + i * btn_width, top(), buttons[i]->text);
		bitmap_output(btn_start + 10, h - bottom() + 8, text);
		}
		btn_start += btn_width + btn_pad;
	}
	glPopMatrix();
}
