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

#ifndef VICICLE_H
#define VICICLE_H

#include "node.h"
#include "link.h"
#include "terrain.h"

#include <cmath>

class World;

/***
 * Pojazd, ko³a w dolnych rogach mog¹ go napêdzaæ (dzia³aæ si³¹ do przodu).
 * Na ka¿dy róg mo¿e dzia³aæ niezale¿na si³a.
 * Mo¿na budowaæ bardziej z³o¿one pojazdy poprzez uwspólnianie wie¿cho³ków.
 */
class Vicicle
{
  protected:
        ///inicjalizuje d³ugo¶ci na aktualne
        void initLengths();
        
        ///poprawia odleg³o¶æ pomiêdzy A i B na distance
        void satisfyDistanceConstraint(Node& A, Node& B, double& distance);

  public:
        Node leftDown, rightDown, leftUp, rightUp;
    
        ///odleg³o¶ci pomiêdzy wêz³ami
        double d, l, r, u, dl, ul;
	
		/// Konstruuje pojazd o podanych wie¿cho³kach. Wie¿cho³ki powinny stanowiæ figure wypuk³¹.
		Vicicle(const Node& leftDown, const Node& rightDown, const Node& rightUp, const Node& leftUp);
		
		/// Konstruuje pojazd w kszta³cie prostok¹ta o masie ca³kowitej 2*upMass + 2*downMass
		Vicicle(double down, double up, double left, double right, double downMass, double upMass);
		
		/// Rysuje pojazd
		void draw();
		
		/// Resetuje po³o¿enie i si³y dzia³aj¹ce na pojazd
		void reset();
		
		/// Poprawia odleg³oœci miêdzy wêz³ami tak by by³y odpowiednio d, l, r, u, dr, lu
		void satisfyConstraints();
		
		/// Poprawia wêz³y by znalaz³y sie na planszy
		void satisfyTerrainConstraints(Terrain& terrain);
		
		bool satisfyLineConstraint(Link& link, Node& a);
		
		/**
           Poprawia by pojazd znalaz³ siê na podanym kawa³ku trasy (przesówa te¿ trase)
           je¶li zachodzi kolizja.
        */
		void satisfyLineConstraints(Link& link);
		
		/// Porusza wie¿cho³kami pojazdu
		void go(const double time, const World& world);
		
		void goAndZeroForce(const double timeXtime, const World& world);
};

#endif // VICICLE_H

