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

#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <cmath>
#include <GL/gl.h>     // The GL Header File

#include "libs/geometry.h"
#include "libs/camera2d.h"
#include <iostream>

#include "libs/vec2d.h"

/**
 * Pod�o�e jest opisane za pomoc� ci�gu wysoko�ci odleg�ych o pewien krok (step), 
 * wsp. kolejnych punkt�w to:
 * ..., (-step, 0.0), (0.0, heights[0]), (step, heights[1]), ..., (n*step, heights[n]), ((n+1)*step, 0.0), ...
 * gdzie n = heights.size - 1
 */
class Terrain
{
		///najni�szy punkt terenu
		double _lowest;
	
	public:
		
		double lowest() { return _lowest; }
	
	    ///szeroko�� jednego segmentu
	    double step;
	    
	    ///wysoko�ci dla kolejnych punkt�w
	    std::vector<double> heights;
	    
	    ///pobiera wysoko�� wierzcho�ka
	    const double operator[](unsigned index) const {
	       return (index >= heights.size()) ? 0.0 : heights[index];
	    }
	    
	    ///sprawdza czy dany punkt dotyka ziemi (lub jest w niej)
	    bool touch(const double& x, const double& y) const;
	    
	    bool touch(const vec2d<double>& p) const {
	    	return touch(p.x, p.y);
	    }
	    
	    /**
          Koryguje po�o�enie punktu do najbli�szego punktu na planszy
          @return true je�li punkt zosta� przesuni�ty (kolidowa�)
        */
	    bool correctPoint(double& x, double& y) const {
           int i = (int)floor(x / step);
           double x0(step * i), y0((*this)[i]), y1((*this)[i+1]);
           double deltay(y1 - y0);
           //sprawdzamy czy punkt ma dobre po�o�enie
	       //warunek po || by wystarczy� ale war. z && odrzuca wi�kszo�� przypadk�w
	       if ((y > y0 && y > y1) || isOverOn(x, y, x0, y0, step, deltay))
               return false; 
           //nast�pi�a kolizja, przes�wamy pnunkt w najbli�sze dozwolone wsp��dne
           //to nie dzia�a za szybko wi�c miejmy nadzieje �e wyst�pi �adko: 
           double q = getPropNorm(x, y, x0, y0, step, deltay);
           x0 += q * step;          //potencjalne nowe wsp�rz�dne
           y0 += q * deltay;
           //double dist = hypot(x - x0, y - y0);//sqrt(sqr(x - x0) + sqr(y - y0));
           
           x = x0;  //TODO: sprawdzi� odcinki oddalone nie dalej ni� dist
           y = y0;
           
           return true;      
	    }
	    
	    bool correctPoint(vec2d<double>& p) const {
	    	return correctPoint(p.x, p.y);
	    }
	
		// class constructor
		Terrain(double step = 1.0)
        : step(step) {};
		// class destructor
		~Terrain();
		
		/**
         rezerwuje teren tak by d�ugo�� wynosi�a conajmniej minLen
		 nowy kawa�ek terenu ma wysoko�� height
		*/
		void reserve(unsigned minLen, double height = 0.0) {
		  if (minLen > heights.size()) heights.resize(minLen, height);
		}
		
		/**
         dr��y d�/g�rke o g��boko�ci/wysoko�ci |height| i d�ugo�ci steps odcink�w od start
         (na polach start, start+1, ..., start + steps)
         aproksymuje parabol�, skrajne punkty pozostaj� nie zmienione
         @param height g��boko�� do�u (ujemna dla wg��bie�)
         @param start punkt od kt�rego powinien si� zaczyna� d�
         @param steps ilo�� odcink�w jakie nale�y zmieni�
        */ 
		//void createHole(double height, unsigned start, unsigned steps);
		
		/**
		 dr��y d� w oparciu o krzyw� Bezier'a 3 stopnia
		*/
		//void createHole(double height1, double height2, unsigned start, unsigned steps);
		
		///Rysuje teren
		void draw(const Camera2d& c) const;
		
		///@return gdzie sie zaczyna wg��bienie
		double fromX() const {
		  return -step;
		}
		
		///@return gdzie sie ko�czy wg��bienie
		double toX() const {
		  return heights.size() * step;
		}
		
		///Wczytuje teren ze strumienia. Krok, ilo�� wysoko�ci i kolejne wysoko�ci.
        friend std::istream &operator>>(std::istream& in, Terrain &t);
        
        ///Zapisuje teren do strumienia. Krok, ilo�� wysoko�ci i kolejne wysoko�ci.
        friend std::ostream& operator<<(std::ostream& out, Terrain &t);
		
};

#endif // TERRAIN_H

