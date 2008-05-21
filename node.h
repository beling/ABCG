#ifndef NODE_H
#define NODE_H

#include <GL/gl.h>     // The GL Header File
#include "PhysicConsts.h"
#include "libs/vec2d.h"

/**
 * No description
 */
class Node
{
	public:
	    
	   ///Predyktat sprawdzaj¹cy czy wêze³ znajduje siê w danym obszarze
	    struct InRect {
	    
	       const double &left, &top, &right, &bottom;
	    
	       InRect(const double& left, const double& top, const double& right, const double& bottom)
           : left(left), top(top), right(right), bottom(bottom) {};
	    
	       bool operator() (Node& toTest) {
	    	   return toTest.pos.in_rect(left, top, right, bottom);
	       }
	    };
	    
	    bool movable; ///!!!!!!!
	
        ///node mass + 1/2 connected links mass
	    double mass;
	    
	    ///node mass (without links)
	    double selfMass;
	
	    ///Po³o¿enie, poprzednie po³o¿enie, po³o¿enie pocz¹tkowe, si³a
	    vec2d<double> pos, prev_pos, pos_0, force; /*V*/
        
		/// class constructor
		Node(const double x, const double y, const double mass = 100.0)
        : movable(true), mass(mass), selfMass(mass), pos_0(x, y) { reset(); };
        
		/// class destructor
		~Node();        
        
        /**
          ró¿niczkowanie metod¹ "Verlet":
          zobacz: 
          http://www.fisica.uniud.it/~ercolessi/md/md/node21.html
          http://www.ioi.dk/Homepages/thomasj/publications/gdc2001.htm
          http://en.wikipedia.org/wiki/Verlet_integration
        */
        void go(const double time, const double entropy, const double gravForce) {
            if (!movable) return;
            //const double f = 0.000001;	//f = [0.0, 1.0]
            const double timeXtime = time * time;
            double temp = pos.x;
            //x += x - prevX + (forceX / mass) * timeXtime; //normalne równanie
            pos.x = (2.0-entropy)*pos.x - (1.0-entropy)*prev_pos.x + (force.x / mass) * timeXtime; //z t³umieniem
            //Vx = (x - prevX) / (2.0 * time);	//ok, potrzebne do innego, ,,prawdziwszego'', sposobu liczenia t³umienia
            prev_pos.x = temp;
            temp = pos.y;
            //y += y - prevY + (forceY / mass - gravForce) * timeXtime;
            pos.y = (2.0-entropy)*pos.y - (1.0-entropy)*prev_pos.y + (force.y / mass - gravForce) * timeXtime; //z t³umieniem
            //Vy = (y - prevY) / (2.0 * time);	//ok, potrzebne do innego, ,,prawdziwszego'', sposobu liczenia t³umienia
            prev_pos.y = temp;  
        }
        
        ///Porusza i zeruje si³e:
        void goAndZeroForce(const double time, const double entropy, const double gravForce) {
            go(time, entropy, gravForce);
            force.set(0.0, 0.0);
        }
	
	    ///Ustawia po³o¿enie na wartoœæ pocz¹tkow¹ (x0, y0), zeruje prêdkoœæ i si³e
        void reset() {
        	pos = prev_pos = pos_0;
            //Vx = 0.0;	//ok
            //Vy = 0.0;
        	force.set(0.0, 0.0);
            mass = selfMass;
        }
        
        ///Sprawdza czy wêz³y s¹ równe, w sensie po³o¿enia
        bool operator==(const Node& node) {
            return node.pos == pos;
        }
        
        void draw(double size = 1.5) {
            glBegin(GL_QUADS);
              glVertex2d(pos.x + size, pos.y);
              glVertex2d(pos.x, pos.y + size);
              glVertex2d(pos.x - size, pos.y);
              glVertex2d(pos.x, pos.y - size);
            glEnd();
        }

};

#endif // NODE_H

