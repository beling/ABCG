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

#ifndef VICICLE_H
#define VICICLE_H

#include "node.h"
#include "link.h"
#include "terrain.h"

#include <cmath>

class World;

/***
 * Pojazd, ko�a w dolnych rogach mog� go nap�dza� (dzia�a� si�� do przodu).
 * Na ka�dy r�g mo�e dzia�a� niezale�na si�a.
 * Mo�na budowa� bardziej z�o�one pojazdy poprzez uwsp�lnianie wie�cho�k�w.
 */
class Vicicle
{
  protected:
        ///inicjalizuje d�ugo�ci na aktualne
        void initLengths();
        
        ///poprawia odleg�o�� pomi�dzy A i B na distance
        void satisfyDistanceConstraint(Node& A, Node& B, double& distance);

  public:
        Node leftDown, rightDown, leftUp, rightUp;
    
        ///odleg�o�ci pomi�dzy w�z�ami
        double d, l, r, u, dl, ul;
	
		/// Konstruuje pojazd o podanych wie�cho�kach. Wie�cho�ki powinny stanowi� figure wypuk��.
		Vicicle(const Node& leftDown, const Node& rightDown, const Node& rightUp, const Node& leftUp);
		
		/// Konstruuje pojazd w kszta�cie prostok�ta o masie ca�kowitej 2*upMass + 2*downMass
		Vicicle(double down, double up, double left, double right, double downMass, double upMass);
		
		/// Rysuje pojazd
		void draw();
		
		/// Resetuje po�o�enie i si�y dzia�aj�ce na pojazd
		void reset();
		
		/// Poprawia odleg�o�ci mi�dzy w�z�ami tak by by�y odpowiednio d, l, r, u, dr, lu
		void satisfyConstraints();
		
		/// Poprawia w�z�y by znalaz�y sie na planszy
		void satisfyTerrainConstraints(Terrain& terrain);
		
		bool satisfyLineConstraint(Link& link, Node& a);
		
		/**
           Poprawia by pojazd znalaz� si� na podanym kawa�ku trasy (przes�wa te� trase)
           je�li zachodzi kolizja.
        */
		void satisfyLineConstraints(Link& link);
		
		/// Porusza wie�cho�kami pojazdu
		void go(const double time, const World& world);
		
		void goAndZeroForce(const double timeXtime, const World& world);
};

#endif // VICICLE_H

