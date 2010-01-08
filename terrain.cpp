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

#include "terrain.h" // class's header file
#include <cmath>

#include <algorithm>

// class constructor
//Terrain::Terrain()
//{
	// insert your code here
//}

// class destructor
Terrain::~Terrain()
{
	// insert your code here
}

/*void Terrain::createHole(double height, unsigned start, unsigned steps) {
    if (!steps) return;
    unsigned last = start + steps;
    reserve(last);
    double a = (height * 4.0) / (steps * steps);    //wsp. (-a) paraboli * step^2
    for (unsigned i = (steps - 1) / 2; i; --i) {
        double delta = a * (double)i * ((double)i + (double)steps);
        heights[start + i] += delta;
        heights[last - i] += delta;
    }
    if (!steps & 1) //nieparzysta iloœæ punktów do przesuniêcia
        heights[start + steps/2] += height;
};*/ //BUG??

//void Terrain::createHole(double height, double weight, unsigned start, unsigned steps) {
//}

bool Terrain::touch(const double& x, const double& y) const {
    int i = (int)floor(x / step);
    double y0((*this)[i]);
    return !isOver(x, y, step * i, y0, step, (*this)[i+1] - y0);
    //return (step * (y - y0) <= (x - (step * i)) * ((*this)[i+1] - y0));
}

//void drawTerrainPoint(double x, double y) {
//}

inline void terrainVertex(double x, double y) {
	glColor4f(0.1f, std::max(y/200.0+0.9, 0.2), 0.1f, 0.9f);
	glVertex2d(x, y);
}

//TODO: skompilowaæ i u¿ywaæ list openGL
void Terrain::draw(const Camera2d& c) const {
    double b = c.realBottom();
    if (b > 0.0) return; //kamera patrzy za wysoko
    if (b > _lowest) b = _lowest - 1.0;
    double l = fmin(c.realLeft(), -step);
    double r = fmax(c.realRight(), toX()+step);
    //glColor4f(0.0f, 0.7, 0.0f, 1.0f);//<-TODO: Color calc.
    glBegin(GL_QUAD_STRIP);
    	terrainVertex(l, b);
    	terrainVertex(l, 0.0);
    	terrainVertex(-step, b);
    	terrainVertex(-step, 0.0);
        double x = 0.0;
        for (unsigned i = 0; i < heights.size(); i++) {
        	terrainVertex(x, b);
        	terrainVertex(x, heights[i]);
            x += step;
        }
        terrainVertex(x, b);
        terrainVertex(x, 0.0);
        terrainVertex(r, b);
        terrainVertex(r, 0.0);
    glEnd();
}

std::istream& operator>>(std::istream& in, Terrain &t) {
    int len;
    in >> t.step;
    in >> len; //d³ugoœæ
    t.heights.clear();
    t.reserve(len);
    t._lowest = 0.0;
    for (std::vector<double>::iterator i = t.heights.begin(); i != t.heights.end(); i++) {
        in >> *i;
        if (*i < t._lowest) t._lowest = *i;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, Terrain &t) {
    out << t.step << ' ' << t.heights.size() << std::endl;
    for (std::vector<double>::iterator i = t.heights.begin(); i != t.heights.end(); i++)
        out << ' ' << *i;
    out << std::endl;
    return out;
}
