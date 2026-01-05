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

#ifndef TIMER_H
#define TIMER_H

#include <ctime>	//clock_t, clock(), CLOCKS_PER_SEC, time_t, time
#include <iostream>
#include <iomanip>  

/**
 * Zegarek, mierzy odst�py czasowe z dok�adno�ci� do 0.001 sec.
 * Powy�ej 1/2h precyzja maleje do 1sec.
 */
class Timer {
	
    clock_t start_clock;
    time_t start_time;

	public:
		
		///Tworzy nowy zegar i go startuje.
		Timer(): start_clock(clock()), start_time(time(0)) {};
        
        ///Restartuje zegar.
        void restart() {
        	start_clock = clock();
        	start_time  = time(0);
        }
        
        /**
         * Restartuje zegar.
         * @return ile sekund up�yne�o od poprzedniego restartu zegara.
         */
        double intervalSec() {
        	double result = getSec();
        	restart();
        	return result;
        }
        
        ///@return ile sekund up�yne�o od poprzedniego restartu zegara.
        double getSec() const {
        	time_t acc_sec = time(0) - start_time;
			if (acc_sec < 3600)	//mamy dok�adny czas?
				return double(clock() - start_clock) / double(CLOCKS_PER_SEC);
			else
				return double(acc_sec);
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Timer& t) {
        	os << std::setprecision(2) << std::setiosflags(std::ios::fixed) << t.getSec();
  			return os;
        }
};


#endif // TIMER_H

