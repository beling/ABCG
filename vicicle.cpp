#include "vicicle.h" // class's header file

#include "world.h"

void Vicicle::initLengths() {
  //zapamiêtuje oryginalne odleg³oœci pomiêdzy punktami:
  //d = hypot(rightDown.x0 - leftDown.x0, rightDown.y0 - leftDown.y0);
  //u = hypot(rightUp.x0 - leftUp.x0, rightUp.y0 - leftUp.y0);
  //l = hypot(leftUp.x0 - leftDown.x0, leftUp.y0 - leftDown.y0);
  //r = hypot(rightUp.x0 - rightDown.x0, rightUp.y0 - rightDown.y0);
  //dl = hypot(rightUp.x0 - leftDown.x0, rightUp.y0 - leftDown.y0);
  //ul = hypot(leftUp.x0 - rightDown.x0, leftUp.y0 - rightDown.y0);
  
  d = rightDown.pos_0.distans(leftDown.pos_0);
  u = rightUp.pos_0.distans(leftUp.pos_0);
  l = leftUp.pos_0.distans(leftDown.pos_0);
  r = rightUp.pos_0.distans(rightDown.pos_0);
  dl = rightUp.pos_0.distans(leftDown.pos_0);
  ul = leftUp.pos_0.distans(rightDown.pos_0);
}

Vicicle::Vicicle(const Node& leftDown, const Node& rightDown, const Node& rightUp, const Node& leftUp)
: leftDown(leftDown), rightDown(rightDown), leftUp(leftUp), rightUp(rightUp) { initLengths(); }

Vicicle::Vicicle(const double down, const double up, const double left, const double right,
                 const double downMass, const double upMass)
: leftDown(left, down, downMass), rightDown(right, down, downMass),
  leftUp(left, up, upMass), rightUp(right, up, upMass)
{ initLengths(); }

void Vicicle::reset() {
    leftDown.reset();
    rightDown.reset();
    leftUp.reset();
    rightUp.reset();
}

void Vicicle::satisfyDistanceConstraint(Node& A, Node& B, double& distance) {
	double dx(B.pos.x - A.pos.x), dy(B.pos.y - A.pos.y);
	double dlen(hypot(dx, dy));
	double diff((dlen - distance)/dlen/(A.mass+B.mass));
	double diffMass(diff * B.mass);
	A.pos.x += diffMass * dx;
	A.pos.y += diffMass * dy;
	diffMass = diff * A.mass;
	B.pos.x -= diffMass * dx;
	B.pos.y -= diffMass * dy;
}

void Vicicle::satisfyConstraints() {
    satisfyDistanceConstraint(rightDown, leftDown, d);
    satisfyDistanceConstraint(rightUp, leftUp, u);
    satisfyDistanceConstraint(leftUp, leftDown, l);
    satisfyDistanceConstraint(rightUp, rightDown, r);
    satisfyDistanceConstraint(rightUp, leftDown, dl);
    satisfyDistanceConstraint(leftUp, rightDown, ul);
}

void Vicicle::satisfyTerrainConstraints(Terrain& terrain) {
    terrain.correctPoint(leftDown.pos);
    terrain.correctPoint(rightDown.pos); 
    terrain.correctPoint(leftUp.pos);
    terrain.correctPoint(rightUp.pos);
}

bool Vicicle::satisfyLineConstraint(Link& link, Node& a) {
   if (a.pos.x > link.B.pos.x || link.A.pos.x > a.pos.x)
      return false;      //szybka eliminacja odleg³ych belek
   double p = dotProdukt(a.pos.x - link.A.pos.x, a.pos.y - link.A.pos.y, link.B.pos.x - link.A.pos.x, link.B.pos.y - link.A.pos.y);
   if (p <= 0 || p/link.length >= l/1000.0) return false;   //belka zbyt daleko
   link.correctColision(a.pos, 1.0 / a.mass);         //OK, poprawiamy
   return true;   //TODO: ten kod mo¿e ¼le dzia³aæ! wytestowaæ!
}

void Vicicle::satisfyLineConstraints(Link& link) {
    if (leftDown.pos.x >= rightDown.pos.x) return;
    satisfyLineConstraint(link, rightDown);
    satisfyLineConstraint(link, leftDown);
}

void Vicicle::draw() {
    glColor3d(0.7, 0.9, 0.7);
    glBegin(GL_QUADS);
        glVertex2d(leftDown.pos.x, leftDown.pos.y); 
        glVertex2d(leftUp.pos.x, leftUp.pos.y);
        glVertex2d(rightUp.pos.x, rightUp.pos.y);
        glVertex2d(rightDown.pos.x, rightDown.pos.y);  
    glEnd();
}

void Vicicle::go(const double time, const World& world) {
    leftDown.go(time, world.entropy(leftDown.pos.y), world.gravity_force(leftDown.pos.y));
    rightDown.go(time, world.entropy(rightDown.pos.y), world.gravity_force(rightDown.pos.y));
    leftUp.go(time, world.entropy(leftUp.pos.y), world.gravity_force(leftUp.pos.y));
    rightUp.go(time, world.entropy(rightUp.pos.y), world.gravity_force(rightUp.pos.y));
}

void Vicicle::goAndZeroForce(const double timeXtime, const World& world) {
    leftDown.goAndZeroForce(timeXtime, world.entropy(leftDown.pos.y), world.gravity_force(leftDown.pos.y));
    rightDown.goAndZeroForce(timeXtime, world.entropy(rightDown.pos.y), world.gravity_force(rightDown.pos.y));
    leftUp.goAndZeroForce(timeXtime, world.entropy(leftUp.pos.y), world.gravity_force(leftUp.pos.y));
    rightUp.goAndZeroForce(timeXtime, world.entropy(rightUp.pos.y), world.gravity_force(rightUp.pos.y));
}
