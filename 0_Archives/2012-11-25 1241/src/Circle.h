#ifndef CIRCLE_H
#define CIRCLE_H

#include "geo.h"
#include "Rectangle.h"

class Circle {
	public:
		Circle(const point &center, const double &r);
		Circle(const int &cx, const int &cy, const double &r);
		double getR();
		point getCenter();
		Circle getCircle();
		point intersects(const Rectangle &r);
		void move(const vec &v, const double &speed);
	protected:
		point center;
		double r;
};

#endif
