#ifndef MENU_H_
#define MENU_H_

#include <string>
#include <vector>

#include <GL/glut.h>

void bitmap_output(int x, int y, const std::string& string, void *font = GLUT_BITMAP_HELVETICA_18);
unsigned bitmap_output_width(const std::string& string, void *font = GLUT_BITMAP_HELVETICA_18);
//void stroke_output(int x, int y, const char *format);

enum { BG_NONE = -2, BR_AUTO_WIDTH = -1 };
void nice_output(int x, int y, const std::string& string, int draw_bg = BR_AUTO_WIDTH, void *font = GLUT_BITMAP_HELVETICA_18);

struct Button {
	
	virtual void on_mouse_enter() {};
	
	virtual void on_mouse_escape() {};
	
	virtual void on_click() {};
	
	virtual std::string text() const { return std::string(); }
	
	virtual int length() const { return bitmap_output_width(text().c_str()) + 20; };
	
};

struct Space: public Button {
	
	unsigned w;
	
	Space(unsigned w = 10): w(w) {}
	
	virtual int length() const { return w; }
	
};

struct TextButton: public Button {
	
	std::string _text;
	
	TextButton(const std::string& text): _text(text) {}
	
	virtual std::string text() const { return _text; }
	
	void text(const std::string& str) { _text = str; }
	
};

struct GlutMenuButton: public TextButton {
	
	int glut_menu_nr;
	
	GlutMenuButton(const std::string& text, int glut_menu_nr): TextButton(text), glut_menu_nr(glut_menu_nr) {}
	
	virtual void on_mouse_enter() {
		glutSetMenu(glut_menu_nr);
		glutAttachMenu(GLUT_LEFT_BUTTON);
	}
	
	virtual void on_mouse_escape() {
		glutSetMenu(glut_menu_nr);
		glutDetachMenu(GLUT_LEFT_BUTTON);
	}

};

struct SimpleFuncButton: public TextButton {
	
	void (*fun)(void);
	
	SimpleFuncButton(const std::string& text, void (*fun)(void)): TextButton(text), fun(fun) {}
	
	virtual void on_click() { fun(); }
	
};

class Menu {
	
	int _left, _top, last_mouse_over_btn_nr;
	
	static const int btn_height = 30;
	
	static const int btn_pad = 2;
	
	std::vector<Button*> buttons;
	
public:
	
	Menu(const int left, const int top);
	
	virtual ~Menu();
	
	int left() const { return _left; }
	
	int top() const { return _top; }
	
	//int right() const { return _left + btn_width * buttons.size(); }
	
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
	
	void add_text(const std::string& text) { add(new TextButton(text)); }
	
	void add_space(const unsigned width = 10) { add(new Space(width)); }
	
	void draw();
};

#endif /*MENU_H_*/
