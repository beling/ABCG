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
 * Reprezentuje œwiat gry, zawiera ca³¹ fizyke i umie siê narysowaæ
 */
class World {
	
    private:
        std::list<Node>::iterator addNode(const Node& toAdd);
        
        /// czyœci links i kopiuje links_all na jego miejsce
        void clone_links_list();
    
	public:
		/// Krok czasowy
		static const double timeStep = 0.00003;
		
		static const double water_y = -10.0;
		
		static const Enviroment& enviroment(const double y) {
			static Enviroment normal(0.000001, gravForce);
			static Enviroment under_water(0.00002, gravForce/6.0);
			return y > water_y ? normal : under_water;
		}
		
	   /// aktualny czas
	    double time;
	    
	   /// Iloœæ kasy do wydania
	    double money_limit;
	
	   /// teren
        Terrain terrain;
        
       /// poci¹g 
        Train train;
        
       /// wszystkie ³¹cza
        std::list<Link> links_all;
        
       /// ³¹cza (aktywne), zawieraj¹ te¿ belki mostu
        std::list<Link> links;
        
       /// ³acza (nieaktywne, nie poruszamy nimi, po zerwaniu)
        //std::list<Link> unactive_links;
        
       /// ³acza stanowi¹ce sam most (na wysokoœci 0.0) 
        std::list<Link*> bridge;
        
       /// wêz³y
        std::list<Node> nodes;

       /// wêz³y (nieaktywne, nie poruszamy nimi, g³ównie "zakopane w ziemi")
        std::list<Node*> unactive_nodes;
        
	   /// konstruktor œwiata
		World();
		
	   /// koniec œwiata
		~World();
		
	  /// Rysuje wode
		void draw_water(const Camera2d& c);
		
		void draw_sky(const Camera2d& c);
        
       /// Rysuje ca³y œwiat
		void draw(const Camera2d& c);
        
       /// Oblicza stan œwiata po chwili timeStep (powinna byæ doœæ krótka)
        void go();
        
       /// Resetuje czas, wszelkie po³o¿enia, prêdkoœci, si³y... nale¿y wywo³aæ przed rozpoczêciem animacji
        void start();
       
       /// Resetuje. Do wywo³ania po animacji.
        void stop();
        
       /// Usówa wszelkie belki... 
        void clear();
       
       /**
          Szuka wêz³a mieszcz¹cego siê w podanym prostok¹cie
          @return wskaŸnik na znaleziony wêze³ lub nodes->end() gdy ¿aden nie spe³nia kryteriów
       */
        std::list<Node>::iterator findNode(const double left, const double top, const double right, const double bottom);
          
       /**
          Szuka wêz³a o podanych wspó³rzêdnych z dok³adnoœci¹ do precysion.
          x wsp. x pozycji
          @param y wsp. y pozycji
          @param prec wymagana precyzja (maksymalna ró¿nica dla ka¿dej ze wspó³rzêdnych)
          @return wskaŸnik na znaleziony wêze³ lub nodes->end() gdy ¿aden nie spe³nia kryteriów
       */
        std::list<Node>::iterator findNode(const double x, const double y, const double prec);
        
       /**
        Dodaje ³¹cze jeœli nie istnieje ju¿ takie. Dodaje wêz³y w razie potrzeby.
        @param prec precyzja przy wyszukiwaniu ju¿ istniej¹cych
        @return dodany link
       */
        std::list<Link>::iterator addLink(const double x0, const double y0, const double x1, const double y1, const double prec = 0.0);  
        
        bool addLinkIfHaveMonay(const double x0, const double y0, const double x1, const double y1, const double prec = 0.0);  
                
       ///Usuwa ³acze lub wêze³ w miejscu o podanych wspó³rzêdnych
        void delAt(const double x, const double y, const double prec = 0.0);
        
       ///Wczytuje teren ze strumienia. Krok, iloœæ wysokoœci i kolejne wysokoœci. A nastêpnie most (pary punktów). 
        friend std::istream& operator>>(std::istream& in, World &V);
        
        ///Zapisuje teren do strumienia. Krok, iloœæ wysokoœci i kolejne wysokoœci. A nastêpnie most (pary punktów).
        friend std::ostream& operator<<(std::ostream& out, World &t);
        
        static double link_prize(double len);
        
        static double link_prize(double x1, double y1, double x2, double y2) { return link_prize(vec2d<double>(x1, y1).distans(x2, y2)); }
        
        ///Ile kasy pozosta³o
	    double money_left() const;
	    
	    bool has_money_for(double len) { return money_left() >= link_prize(len); }
	    
	    bool is_level_complited() const;
		
};

#endif // WORLD_H

