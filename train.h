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

#ifndef TRAIN_H
#define TRAIN_H

#include "vicicle.h"
#include "link.h"

#include <list>

/**
 * Poci�g (lokomotywa i kilka wagon�w)
 */
class Train
{        
    protected:
        void speedUpIfNeed(Node& node, const double& dx, const double& dy, const double& len);

	public:
        /// na pozycji 0 lokomotywa, 1-... - wagony 
	    std::list<Vicicle> wagons;
	    
	    /// �acza pomi�dzy wagonami
	    std::list<Link> links;
	    
	    /// maksymalna zmiana po�o�nia poci�gu podniesiona do kwadratu = (Vmax * timeStep) ^ 2
	    double max_delta_r_sqr;
	    
		/// odleg�o�� miedzy wagonami
	    double interval;
	    
	    /// si�a przyspieszaj�ca w N (na w�ze�)
	    static const double acc_force = 400000.0;
	    
		/** 
           Tworzy poci�g gdzie lokomotywa ma podane parametry.
           @param speed po��dana szybko�� poci�gu
           @param interval odst�p mi�dzy wagonami
        */
		Train(double max_delta_r, double interval, double left, double down, double width, double height, double upMass, double downMass);
	
	    ///dodaje wagon
	    void addWagon(double width, double height, double upMass, double downMass);
	
		///resetuje po�o�enia, si�y...    
 	    void reset();
 	    
 	    ///rysuje poci�g
 	    void draw();
 	    
 	    ///oblicza si�y, porusza wagonami po czym zeruje si�y 	    
 	    void go(const double time, const World& world);
 	    
		/**
           Poprawia by poci�g (wszystkie wagony) znalaz�y si� na podanym kawa�ku trasy
           (przes�wa te� trase) je�li zachodzi kolizja.
        */
		void satisfyLineConstraints(Link& link);
 	    
 	    ///kolizja z terenem
 	    void satisfyTerrainConstraints(Terrain& terrain);
 	    
		/// Poprawia kszta�t wszystkich wagon�w
		void satisfyConstraints();
};

#endif // TRAIN_H

