/*
    Open Bridge Builder
*/

#include <cstdlib>
#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#include <fstream>
#include "world.h"
#include "libs/utils.h"
#include "LevelsProvider.h"

#include "main_init.h"

vec2d<double> linkStart(NAN, NAN), linkEnd(NAN, NAN); //jeœli rysujemy link to wyznacza jego wsp.

const double gridStep = 5.0;

bool fullScreen; //okreœla czy tryb pe³noekranowy

void setFullScreen(bool doFullScreen = true) {
    if (doFullScreen == fullScreen) return;
    if (doFullScreen) glutFullScreen();
    else {
        glutReshapeWindow(500, 400); // Go Into A 500 By 500 Window
        glutPositionWindow(50, 50);
    }
    fullScreen = doFullScreen;
}

double snapToGrid(double sth) {
    return rint(sth / gridStep) * gridStep;
}

void init() {    // Create Some Everyday Functions
	glDisable(GL_DEPTH_TEST);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(2.0);
	
	//glShadeModel(GL_SMOOTH);// Enable Smooth Shading	
    //glColor4f(1.0f,1.0f,1.0f,0.5f); 			
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
	glClearColor(0.4f, 0.67f, 0.96f, 0.0f);
	glMatrixMode(GL_MODELVIEW);                         // Select The Model View Matrix
    glLoadIdentity();                                   // Reset The Model View Matrix
}

void display () {   // Create The Display Function
  glClear(GL_COLOR_BUFFER_BIT);	// Clear Screen And Depth Buffer
  camera.zoomedLine();
  world.draw(camera);
  if (mode == m_edit) {
    camera.normalLine();  //rysuje siatke
    if (grid) {
	    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
	    double l(camera.realLeft()), r(camera.realRight()), t(camera.realTop()), d(camera.realBottom());
	    glBegin(GL_LINES); 
	      for (double x = snapToGrid(l); x <= r; x += gridStep) {
	            glVertex2d(x, t);
	            glVertex2d(x, d);
	      }
	      for (double y = snapToGrid(d); y <= t; y += gridStep) {
	            glVertex2d(l, y);
	            glVertex2d(r, y);
	      }
	    glEnd();
    }
    if (!isnan(linkStart.x) && !isnan(linkEnd.x)) {
    	const double link_len = linkStart.distans(linkEnd);
    	if (world.has_money_for(link_len))
    		glColor4f(0.5f, 0.8f, 0.5f, 0.9f);
    	else
    		glColor4f(0.8f, 0.5f, 0.5f, 0.9f);
        camera.zoomedLine();
        glBegin(GL_LINES);      
          glVertex2d(linkStart.x, linkStart.y);
          glVertex2d(linkEnd.x, linkEnd.y);
        glEnd();
        const vec2d<double> msgplace = (linkStart + linkEnd) / 2.0;
        nice_output(camera.screenX(msgplace.x), camera.screenY(msgplace.y) - 30, std::string("$") + cast<std::string>(world.link_prize(link_len)));
    }
    menu.draw();
  }
  glutSwapBuffers();  // Swap The Buffers To Not Be Left With A Clear Screen
}

void phisicalStep() {
    if (mode != m_anim) return;
    static const unsigned physicalIterationPerStep = 1000;
    for (unsigned i = physicalIterationPerStep; i > 0; --i)
        world.go();   
    if (world.is_level_complited()) {
    	//level complated!!
    	//mode = m_pause;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {   // Create The Reshape Function (the viewport)
  camera.resize(w, h);
}

void keyboard(unsigned char key, int x, int y) {  // Create Keyboard Function
  switch (tolower(key)) {
    case '+':
      if (camera.zoom > 10.0) return;
      camera.zoomIn(1.04);
      glutPostRedisplay();
      break;
    case '-':
      if (camera.zoom < 2.0) return;
      camera.zoomIn(0.96);
      glutPostRedisplay();
      break;
    case 32:    //start/stop animacji
      linkStart.x = NAN; //anulujemy rysowanie linka
      if (mode == m_edit) {
            world.start();      
            mode = m_anim;     
      } else {
            mode = m_edit;
            world.stop();
            glutPostRedisplay();
      }
      break;
    /*case 's':
      if (mode == m_edit) fileSave();
      break;
    case 'l':
      if (mode != m_edit) return;
      fileLoad();
      glutPostRedisplay();
      break;*/
    case 'c': //czyœci œwiat
      if (mode != m_edit) return;
      world.clear();
      glutPostRedisplay();
      break;
    case 'p':    //pause
        if (mode == m_pause)
              mode = m_anim;
        else if (mode == m_anim)
              mode = m_pause;  
        break;
    case 13:     //ENTER   
        if (!(glutGetModifiers() & GLUT_ACTIVE_ALT)) break; //bez alt-a
    case 'w':    //pe³ny ekran/okno
    case 'f':
        setFullScreen(!fullScreen);
      break;  
    case 27:        // When Escape Is Pressed...
    case 'q':
    	just_exit();   // Exit The Program
    	break;        // Ready For Next Case

  }
}

void arrow_keys(int a_keys, int x, int y)  // Create Special Function (required for arrow keys)
{
  switch (a_keys) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
      camera.translate(0.0, 1.0);
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
      camera.translate(0.0, -1.0);
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      camera.translate(-1.0, 0.0);
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      camera.translate(1.0, 0.0);
      glutPostRedisplay();
      break;
  }
}

void mouse(int button, int state, int x, int y) {
	if (mode == m_edit) if (menu.mouse_evt(button, state, x, y)) return;
    if (state != GLUT_UP) return;
    if (button == GLUT_MIDDLE_BUTTON) {
        camera.centerToScreen(x, y);
        glutPostRedisplay();
    }
    if (mode != m_edit) return;
    vec2d<double> real(snapToGrid(camera.realX(x)), snapToGrid(camera.realY(y)));
    if (button == GLUT_LEFT_BUTTON) {
        if (!isnan(linkStart.x)) { //koñczymy rysowaæ link
        	if (world.addLinkIfHaveMonay(linkStart.x, linkStart.y, real.x, real.y, gridStep / 4.0))
        		linkStart = real;
        } else
        	linkStart = real;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (isnan(linkStart.x)) //usówamy link
            world.delAt(real.x, real.y);
        else                    //anulujemy rysowanie linka
            linkStart.x = linkEnd.x = NAN;
    }
    glutPostRedisplay();
}

void pasiveMouse(int x, int y) {
    if (isnan(linkStart.x)) {	//nothing draw now
    	if (mode == m_edit) menu.mouse_move_evt(x, y); //try menu
    	return;
    }
    linkEnd.set(snapToGrid(camera.realX(x)), snapToGrid(camera.realY(y)));
    glutPostRedisplay();
}

//void motionMouse(int x, int y) {
//	menu.mouse_move_evt(x, y); //try menu
//}

int main (int argc, char** argv)   // Create Main Function For Bringing It All Together
{
  glutInit(&argc, argv); // Erm Just Write It =)
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
  glutInitWindowSize(500, 500); // If glutFullScreen wasn't called this is the window size
  glutCreateWindow("Another Bridge Construct Game (Piotr Beling)"); // Window Title
  //glutFullScreen();          // Put Into Full Screen
  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);
  glutIdleFunc(phisicalStep);
  glutMouseFunc(mouse);
  glutPassiveMotionFunc(pasiveMouse);
  glutMotionFunc(pasiveMouse);
  
  //fileName = argc > 1 ? argv[1] : NULL;
  init();
  
  init_menu();
  
  glutMainLoop();          // Initialize The Main Loop
  return 0;
}

