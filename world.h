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

#ifndef WORLD_H
#define WORLD_H

#include "node.h"
#include "link.h"
#include "terrain.h"
#include "train.h"

#include <list>
#include <vector>
#include "libs/vec2d.h"

#include "Enviroment.h"

/**
 * Reprezentuje �wiat gry, zawiera ca�� fizyke i umie si� narysowa�
 */
class World {
	
    private:
        std::list<Node>::iterator addNode(const Node& toAdd);
        
        /// czy�ci links i kopiuje links_all na jego miejsce
        void clone_links_list();
    
	public:
		/// Krok czasowy
		constexpr static const double timeStep = 0.00003;
		
		constexpr static const double water_y = -10.0;
		
		static const Enviroment& enviroment(const double y) {
			static Enviroment normal(0.000001, gravForce);
			static Enviroment under_water(0.00002, gravForce/6.0);
			return y > water_y ? normal : under_water;
		}
		
	   /// aktualny czas
	    double time;
	    
	   /// Ilo�� kasy do wydania
	    double money_limit;
	
	   /// teren
        Terrain terrain;
        
       /// poci�g 
        Train train;
        
       /// wszystkie ��cza
        std::list<Link> links_all;
        
       /// ��cza (aktywne), zawieraj� te� belki mostu
        std::list<Link> links;
        
       /// �acza (nieaktywne, nie poruszamy nimi, po zerwaniu)
        //std::list<Link> unactive_links;
        
       /// �acza stanowi�ce sam most (na wysoko�ci 0.0) 
        std::list<Link*> bridge;
        
       /// w�z�y
        std::list<Node> nodes;

       /// w�z�y (nieaktywne, nie poruszamy nimi, g��wnie "zakopane w ziemi")
        std::list<Node*> unactive_nodes;
        
	   /// konstruktor �wiata
		World();
		
	   /// koniec �wiata
		~World();
		
	  /// Rysuje wode
		void draw_water(const Camera2d& c);
		
		void draw_sky(const Camera2d& c);
        
       /// Rysuje ca�y �wiat
		void draw(const Camera2d& c);
        
       /// Oblicza stan �wiata po chwili timeStep (powinna by� do�� kr�tka)
        void go();
        
       /// Resetuje czas, wszelkie po�o�enia, pr�dko�ci, si�y... nale�y wywo�a� przed rozpocz�ciem animacji
        void start();
       
       /// Resetuje. Do wywo�ania po animacji.
        void stop();
        
       /// Us�wa wszelkie belki... 
        void clear();
       
       /**
          Szuka w�z�a mieszcz�cego si� w podanym prostok�cie
          @return wska�nik na znaleziony w�ze� lub nodes->end() gdy �aden nie spe�nia kryteri�w
       */
        std::list<Node>::iterator findNode(const double left, const double top, const double right, const double bottom);
          
       /**
          Szuka w�z�a o podanych wsp�rz�dnych z dok�adno�ci� do precysion.
          x wsp. x pozycji
          @param y wsp. y pozycji
          @param prec wymagana precyzja (maksymalna r�nica dla ka�dej ze wsp�rz�dnych)
          @return wska�nik na znaleziony w�ze� lub nodes->end() gdy �aden nie spe�nia kryteri�w
       */
        std::list<Node>::iterator findNode(const double x, const double y, const double prec);
        
       /**
        Dodaje ��cze je�li nie istnieje ju� takie. Dodaje w�z�y w razie potrzeby.
        @param prec precyzja przy wyszukiwaniu ju� istniej�cych
        @return dodany link
       */
        std::list<Link>::iterator addLink(const double x0, const double y0, const double x1, const double y1, const double prec = 0.0);  
        
        bool addLinkIfHaveMonay(const double x0, const double y0, const double x1, const double y1, const double prec = 0.0);  
                
       ///Usuwa �acze lub w�ze� w miejscu o podanych wsp�rz�dnych
        void delAt(const double x, const double y, const double prec = 0.0);
        
       ///Wczytuje teren ze strumienia. Krok, ilo�� wysoko�ci i kolejne wysoko�ci. A nast�pnie most (pary punkt�w). 
        friend std::istream& operator>>(std::istream& in, World &V);
        
        ///Zapisuje teren do strumienia. Krok, ilo�� wysoko�ci i kolejne wysoko�ci. A nast�pnie most (pary punkt�w).
        friend std::ostream& operator<<(std::ostream& out, World &t);
        
        static double link_prize(double len);
        
        static double link_prize(double x1, double y1, double x2, double y2) { return link_prize(vec2d<double>(x1, y1).distans(x2, y2)); }
        
        ///Ile kasy pozosta�o
	    double money_left() const;
	    
	    bool has_money_for(double len) { return money_left() >= link_prize(len); }
	    
	    bool is_level_complited() const;
		
};

#endif // WORLD_H

