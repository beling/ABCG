/*
    ABCG (Another Bridge Constructing Game)
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

#ifndef LINK_H
#define LINK_H

#include "node.h"

#include <cmath>
#include <GL/gl.h>     // The GL Header File
#include <functional>

#include "libs/geometry.h"

/**
 * link from node A to node B
 */
class Node;

class Link
{
	public:

        ///Predyktat sprawdzaj�cy czy w�ze� jest ko�cem linka
	    struct IsEnd : public std::binary_function<Link, Node, bool> {
	       bool operator() (const Link& link, const Node& node) const {
	           return (link.A == node || link.B == node);
	       }
	    };

	    //masa �acza
        double mass;

        //wsp. spr�ysto��i (modu� Younga) * pole przekroju poprzecznego / originalLength
        //F = przyrost d�ugo�ci*alfa
        double alfa;

        //oryginalna d�ugo�� pr�ta
        double originalLength;

        //aktualna d�ugo�� pr�ta
        double length;

        //aktualny przyrost d�ugo�� pr�ta
        double deltaLength;

        //ko�ce pr�ta
        Node &A, &B;

		//maksymalny wzgl�dny przyrost d�ugo�ci jaki mo�e wytrzyma� belka
        double maxDeltaLength;

   		/**
            Konstruuje belke o ko�cach A, B, g�sto�ci massden, przekroju 1.0, kt�rej
            wsp. spr�ysto�ci (modu� Younga) resilence
		*/
		//g�sto�� stali to 7850.0, 1/300.0 to przekr�j w m^2, 2.19 to modu� Younga stali
		Link(Node &A, Node &B, const double massden = 7850.0 / 300.0, const double resilence = 2.19E11 / 300.0, const double maxDeltaLength = 0.015);

        //Link(const Link& toCopy);

        //resetuje d�ugo�� pr�ta i po�o�enie, zeruje si�y
        void reset() {
                A.reset();
                B.reset();
                length = originalLength;
                deltaLength = 0.0;
        }

        ///dodaje do ko�c�w pr�ta si�e wynik�� z napr�enia pr�ta, czas jest potrzebny do obliczenia t�umienia
        void calcForces() {
            //Fx = (len - orgLen) * alfa * (lX / len) = (1 - orgLen/len) * alfa * lX
            //Fy = (1 - orgLen/len) * alfa * lY
            double lX = B.pos.x - A.pos.x; //d�ugo�� X
            double lY = B.pos.y - A.pos.y; //d�ugo�� Y
            length = sqrt(lX * lX + lY * lY);   //aktualna d�ugo�� pr�ta
            this->deltaLength = (1.0 - originalLength/length);
            double delta = this->deltaLength * alfa; //Wzgl�dny przyrost d�ugo�ci * alfa

            lX *= delta;
            //lX += (B.Vx - A.Vx) * 200.0; //t�umienie spr�ysto�ci

            A.force.x += lX;
            B.force.x -= lX;

            lY *= delta;
            //lY += (B.Vy - A.Vy) * 200.0; //t�umienie spr�ysto�ci

            A.force.y += lY;
            B.force.y -= lY;
        } //ma = -kx - bV; b = beta * 2m; beta = 1/10

		//draw link
		void draw() {
            //double color = 4.0 * fabs((originalLength - length)/fmax(originalLength, length));
		    double color = fabs(deltaLength) / maxDeltaLength;
            glColor4d(color, 1.0 - color, 0.0, 0.7);
		    glBegin(GL_LINES);
		      glVertex2d(A.pos.x, A.pos.y);
		      glVertex2d(B.pos.x, B.pos.y);
		    glEnd();
        };

        //@return czy belka jest zbyt d�uga (i powinna sie zerwa�)
        bool toLong() {
             return fabs(deltaLength) > maxDeltaLength;
        }

		/**
		  Przes�wa punkt materialny o wsp. x, y oraz siebie tak by punkt znalaz� sie na belce,
		  uwzgl�dnia przy tym masy punktu i swoj� (przes�wa proporcjonalnie do mas).
		  @param x wsp. x punktu
		  @param y wsp. y punktu
		  @param invMass odwrotno�� masy punktu (je�li 0.0 to jedynie belka jest przes�wana)
		*/
        void correctColision(double& x, double& y, double invMass = 0.0) {
            double lx(B.pos.x - A.pos.x), ly(B.pos.y - A.pos.y);        //wymiary belki
            double q = getPropNorm(x, y, A.pos.x, A.pos.y, lx, ly);
            lx = A.pos.x + lx * q;            //Punkt na belce, najbli�szy do x, y
            ly = A.pos.y + ly * q;
            if (invMass != 0.0) {            //je�li trzeba ruszy� punkt
                 double pointMass = 1.0 / invMass;
                 double sumMass = mass + pointMass;
                 x = (x * pointMass + lx * mass) / sumMass; //przes�wam punkt w w�a�ciwe miejsce
                 y = (y * pointMass + ly * mass) / sumMass;
            }
            double correct = q * q + sqr(1.0 - q);
            double d = (x - lx) * correct;
            B.pos.x += q * d;
            A.pos.x += (1.0 - q) * d;
            d = (y - ly) * correct;
            B.pos.y += q * d;
            A.pos.y += (1.0 - q) * d;
        }

        void correctColision(vec2d<double>& pos, double invMass = 0.0) {
        	correctColision(pos.x, pos.y, invMass);
        }
};

#endif // LINK_H

