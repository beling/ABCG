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

#include "link.h" // class's header file

Link::Link(Node &A, Node &B, const double massden, const double resilence, const double maxDeltaLength)
: A(A), B(B), maxDeltaLength(maxDeltaLength) {
    length = originalLength = A.pos.distans(B.pos); //d³ugo¶æ
    deltaLength = 0.0;
    mass = massden * originalLength;           //masa
    alfa = resilence / originalLength;        //wsp. alfa
}

/*Link::Link(const Link& toCopy)
: mass(toCopy.mass), alfa(toCopy.alfa), length(toCopy.length),
  originalLength(toCopy.originalLength), deltaLength(toCopy.deltaLength),
  maxDeltaLength(toCopy.maxDeltaLength), A(toCopy.A), B(toCopy.B) {};*/


//nie zmieniaæ masy node w destruktorze i konstruktorze bo s¹ one wywo³ywane w doœæ przypadkowych momentach w programie!!!
