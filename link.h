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

        ///Predyktat sprawdzaj¹cy czy wêze³ jest koñcem linka
	    struct IsEnd : public std::binary_function<Link, Node, bool> {
	       bool operator() (const Link& link, const Node& node) const {
	           return (link.A == node || link.B == node);
	       }
	    };

	    //masa ³acza
        double mass;

        //wsp. sprê¿ystoœæi (modu³ Younga) * pole przekroju poprzecznego / originalLength
        //F = przyrost d³ugoœci*alfa
        double alfa;

        //oryginalna d³ugoœæ prêta
        double originalLength;

        //aktualna d³ugoœæ prêta
        double length;

        //aktualny przyrost d³ugoœæ prêta
        double deltaLength;

        //koñce prêta
        Node &A, &B;

		//maksymalny wzglêdny przyrost d³ugoœci jaki mo¿e wytrzymaæ belka
        double maxDeltaLength;

   		/**
            Konstruuje belke o koñcach A, B, gêstoœci massden, przekroju 1.0, której
            wsp. sprê¿ystoœci (modu³ Younga) resilence
		*/
		//gêstoœæ stali to 7850.0, 1/300.0 to przekrój w m^2, 2.19 to modu³ Younga stali
		Link(Node &A, Node &B, const double massden = 7850.0 / 300.0, const double resilence = 2.19E11 / 300.0, const double maxDeltaLength = 0.015);

        //Link(const Link& toCopy);

        //resetuje d³ugoœæ prêta i po³o¿enie, zeruje si³y
        void reset() {
                A.reset();
                B.reset();
                length = originalLength;
                deltaLength = 0.0;
        }

        ///dodaje do koñców prêta si³e wynik³¹ z naprê¿enia prêta, czas jest potrzebny do obliczenia t³umienia
        void calcForces() {
            //Fx = (len - orgLen) * alfa * (lX / len) = (1 - orgLen/len) * alfa * lX
            //Fy = (1 - orgLen/len) * alfa * lY
            double lX = B.pos.x - A.pos.x; //d³ugoœæ X
            double lY = B.pos.y - A.pos.y; //d³ugoœæ Y
            length = sqrt(lX * lX + lY * lY);   //aktualna d³ugoœæ prêta
            this->deltaLength = (1.0 - originalLength/length);
            double delta = this->deltaLength * alfa; //Wzglêdny przyrost d³ugoœci * alfa

            lX *= delta;
            //lX += (B.Vx - A.Vx) * 200.0; //t³umienie sprê¿ystoœci

            A.force.x += lX;
            B.force.x -= lX;

            lY *= delta;
            //lY += (B.Vy - A.Vy) * 200.0; //t³umienie sprê¿ystoœci

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

        //@return czy belka jest zbyt d³uga (i powinna sie zerwaæ)
        bool toLong() {
             return fabs(deltaLength) > maxDeltaLength;
        }

		/**
		  Przesówa punkt materialny o wsp. x, y oraz siebie tak by punkt znalaz³ sie na belce,
		  uwzglêdnia przy tym masy punktu i swoj¹ (przesówa proporcjonalnie do mas).
		  @param x wsp. x punktu
		  @param y wsp. y punktu
		  @param invMass odwrotnoœæ masy punktu (jeœli 0.0 to jedynie belka jest przesówana)
		*/
        void correctColision(double& x, double& y, double invMass = 0.0) {
            double lx(B.pos.x - A.pos.x), ly(B.pos.y - A.pos.y);        //wymiary belki
            double q = getPropNorm(x, y, A.pos.x, A.pos.y, lx, ly);
            lx = A.pos.x + lx * q;            //Punkt na belce, najbli¿szy do x, y
            ly = A.pos.y + ly * q;
            if (invMass != 0.0) {            //jeœli trzeba ruszyæ punkt
                 double pointMass = 1.0 / invMass;
                 double sumMass = mass + pointMass;
                 x = (x * pointMass + lx * mass) / sumMass; //przesówam punkt w w³aœciwe miejsce
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

