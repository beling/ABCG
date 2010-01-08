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

#include "train.h" // class's header file

// class constructor
Train::Train(double max_delta_r, double interval, double left, double down, double width, double height, double upMass, double downMass)
: max_delta_r_sqr(max_delta_r * max_delta_r), interval(interval) {
	wagons.push_back(Vicicle(
      Node(left, down, downMass),
      Node(left + width, down, downMass),
      Node(left + 0.8 * width, down + height, upMass),
      Node(left, down + height, upMass)));
}

void Train::addWagon(double width, double height, double downMass, double upMass) {
    Node& prev = wagons.back().leftDown;
    wagons.push_back(Vicicle(prev.pos.y, prev.pos.y + height, prev.pos.x - width - interval,
                             prev.pos.x - interval, downMass, upMass));
    links.push_back(Link(prev, wagons.back().rightDown));
}

void Train::reset() {
    for (std::list<Vicicle>::iterator i = wagons.begin(); i != wagons.end(); i++) {
        i->reset();
        /*double max_delta_r = sqrt(max_delta_r_sqr);
        i->leftDown.prev_pos.x -= max_delta_r;
        i->rightDown.prev_pos.x -= max_delta_r;
        i->leftUp.prev_pos.x-= max_delta_r;
        i->rightUp.prev_pos.x -= max_delta_r; */       //nadaje ¿¹dan¹ prêdkoœæ
    }
    for (std::list<Link>::iterator i = links.begin(); i != links.end(); i++)
        i->reset();
}

void Train::draw() {
    for (std::list<Link>::iterator i = links.begin(); i != links.end(); i++)
        i->draw();
    for (std::list<Vicicle>::iterator i = wagons.begin(); i != wagons.end(); i++)
        i->draw();
}

void Train::go(const double time, const World& world) {
    for (std::list<Link>::iterator i = links.begin(); i != links.end(); i++)
        i->calcForces();
    for (std::list<Vicicle>::iterator i = wagons.begin(); i != wagons.end(); i++)
        i->goAndZeroForce(time, world);
}

void Train::speedUpIfNeed(Node& node, const double& dx, const double& dy, const double& len) {
	if (node.pos.distans_sqr(node.prev_pos) >= max_delta_r_sqr) return;
    //if (node.Vx >= speed) return;
    node.force.x += acc_force * dx / len;
    node.force.y += acc_force * dy / len;
}

void Train::satisfyLineConstraints(Link& link) {
    if (link.A.pos.x >= link.B.pos.x)   //belki odwrócone nas nie interesuj¹
        return;
    std::list<Vicicle>::iterator i = wagons.begin();
    //je¶li lokomotywa jedzie po szynach, mo¿na próbowaæ przyspieszaæ:
    if (i->satisfyLineConstraint(link, i->leftDown))
        speedUpIfNeed(i->leftDown, link.B.pos.x - link.A.pos.x, link.B.pos.y - link.A.pos.y, link.length);

    if (i->satisfyLineConstraint(link, i->rightDown))
        speedUpIfNeed(i->rightDown, link.B.pos.x - link.A.pos.x, link.B.pos.y - link.A.pos.y, link.length);

    for (i++; i != wagons.end(); i++)
        i->satisfyLineConstraints(link);
}

void Train::satisfyTerrainConstraints(Terrain& terrain) {
    std::list<Vicicle>::iterator i = wagons.begin();

    //ew. przyspieszanie lokomotywy na prostej
    if (terrain.correctPoint(i->leftDown.pos.x, i->leftDown.pos.y) &&
        (i->leftDown.pos.x < terrain.fromX() || terrain.toX() < i->leftDown.pos.x))
        speedUpIfNeed(i->leftDown, terrain.step, 0.0, terrain.step);

    if (terrain.correctPoint(i->rightDown.pos.x, i->rightDown.pos.y) &&
        (i->rightDown.pos.x < terrain.fromX() || terrain.toX() < i->rightDown.pos.x))
        speedUpIfNeed(i->rightDown, terrain.step, 0.0, terrain.step);

    terrain.correctPoint(i->leftUp.pos);
    terrain.correctPoint(i->rightUp.pos);

    for (++i; i != wagons.end(); ++i)
        i->satisfyTerrainConstraints(terrain);
}

void Train::satisfyConstraints() {
    for (std::list<Vicicle>::iterator i = wagons.begin(); i != wagons.end(); i++)
        i->satisfyConstraints();
}
