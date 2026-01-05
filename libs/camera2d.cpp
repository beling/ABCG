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

#include "camera2d.h" // class's header file

void Camera2d::set() const {
    double sx = (double)w / zoom / 2.0;
    double sy = (double)h / zoom / 2.0;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
    glLoadIdentity();             // Reset The Projection Matrix
    gluOrtho2D(x - sx, x + sx, y - sy, y + sy);
}

void Camera2d::resize(int width, int height) {
    w = width;
    h = height;
    set();
}

// class destructor
Camera2d::~Camera2d()
{
	// insert your code here
}

