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

#ifndef GEOMETRY_H
#define GEOMETRY_H

/**
    Zwraca liczbe q tak� �e odleg�o�� mi�dzy punktami:
    (x0 + q*dx, y0 + q*dy) oraz (x, y)
    jest minimalna.
*/

template <class T>
inline T getProp(const T& x, const T& y, const T& x0, const T& y0, const T& dx, const T& dy) {
    return ((x - x0) * dx + (y - y0) * dy) / (sqr(dx) + sqr(dy));
}

/**
    Zwraca liczbe 0.0 <= q <= 1.0 tak� �e odleg�o�� mi�dzy punktami:
    (x0 + q*dx, y0 + q*dy) oraz (x, y)
    jest minimalna.
*/

template <class T>
inline T getPropNorm(const T& x, const T& y, const T& x0, const T& y0, const T& dx, const T& dy) {
    T q = getProp(x, y, x0, y0, dx, dy);
    if (q < 0.0) q = 0.0; else
    if (q > 1.0) q = 1.0;
    return q;
}

//template <class T>
//inline T distance();

/**
    Sprawdza czy punkt (x, y) jest nad prost� wyznaczon� przez (x0, y0), [dx, dy].
    Zwraca false je�li punkt jest na prostej.
*/
template <class T>
inline bool isOver(const T& x, const T& y, const T& x0, const T& y0, const T& dx, const T& dy) {
    return dx * (y - y0) > (x - x0) * dy;
}

/**
    Sprawdza czy punkt (x, y) jest nad prost� wyznaczon� przez (x0, y0), [dx, dy].
    Zwraca true tak�e je�li punkt jest na prostej.
*/
template <class T>
inline bool isOverOn(const T& x, const T& y, const T& x0, const T& y0, const T& dx, const T& dy) {
    return dx * (y - y0) >= (x - x0) * dy;
}

/**
    @return warto�� iloczynu wektorowego [x1, y1] * [x2, y2]
*/

template <class T>
inline T dotProdukt(const T& x1, const T& y1, const T& x2, const T& y2) {
    return x1 * y2 - x2 * y1;
}

///@return a*a
template <class T>
inline T sqr(const T& a) {
    return a * a;
}

#endif // GEOMETRY_H
