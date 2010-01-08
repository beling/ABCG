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

#ifndef UTILS_H_
#define UTILS_H_

template <typename T>
inline T& bound(T& value, T& min, T& max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

template <typename T>
inline const T& bound(const T& value, const T& min, const T& max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

/**
 * Conver object from one type to another using std::stringstream.
 * @param arg to convert
 * @return converted
 */
template <typename dst, typename src>
inline dst cast(const src& arg) {
	std::stringstream s;
	s << arg;
	dst result;
	s >> result;
	return result;
}

#endif /*UTILS_H_*/
