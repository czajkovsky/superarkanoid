#include "Circle.h"
#include <cmath>

Circle::Circle(const point &center, const double &r) {
	this->center = center;
	this->r = r;
}

Circle::Circle(const int &cx, const int &cy, const double &r) {
	(this->center).x = cx;
	(this->center).y = cy;
	this->r = r;
}

double Circle::getR() {
	return this->r;
}

point Circle::getCenter() {
	return this->center;
}

point Circle::intersects(const Rectangle &rectangle) {
	point res;
	point ul = rectangle.upperleft;
	point br = rectangle.bottomright;
	double tmp;
	if ((tmp = center.y+r) >= br.y-eps && tmp <= br.y+eps && center.x <= br.x+eps && center.x >= ul.x-eps) {res.x = center.x; res.y = br.y;}
	else if ((tmp = center.x+r) >= ul.x-eps && tmp <= ul.x+eps && center.y <= ul.y+eps && center.y >= br.y-eps) {res.x = ul.x; res.y = center.y;}
	else if ((tmp = center.y-r) <= ul.y+eps && tmp >= ul.y-eps && center.x <= br.x+eps && center.x >= ul.x-eps) {res.x = center.x; res.y = ul.y;}
	else if ((tmp = center.x-r) <= br.x+eps && tmp >= br.x-eps && center.y <= ul.y+eps && center.y >= br.y-eps) {res.x = br.x; res.y = center.y;}

	else if ((tmp = sqrt((center.x-br.x)*(center.x-br.x) + (center.y-ul.y)*(center.y-ul.y))) <= r+eps) {res.x = br.x; res.y = ul.y;}
	else if ((tmp = sqrt((center.x-br.x)*(center.x-br.x) + (br.y-center.y)*(br.y-center.y))) <= r+eps) {res = br;}
	else if ((tmp = sqrt((ul.x-center.x)*(ul.x-center.x) + (center.y-ul.y)*(center.y-ul.y))) <= r+eps) {res = ul;}
	else if ((tmp = sqrt((ul.x-center.x)*(ul.x-center.x) + (br.y-center.y)*(br.y-center.y))) <= r+eps) {res.x = ul.x; res.y = br.y;}
	
	else res.x = res.y = INF;

	return res;
}

Circle Circle::getCircle() {
	return *this;
}

void Circle::move(const vec &v, const double &speed) {
	point p = this->center;
	p.x += speed*v.x;
	p.y += speed*v.y;
	this->center = p;
}

