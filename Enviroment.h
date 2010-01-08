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

#ifndef ENVIROMENT_H_
#define ENVIROMENT_H_

/**
 * Store information about some enviroment constants.
 */
struct Enviroment
{
	const double entropy;
	
	const double gravity_force;
	
	Enviroment(const double entropy, const double gravity_force) : entropy(entropy), gravity_force(gravity_force) {};
};

#endif /*ENVIROMENT_H_*/
