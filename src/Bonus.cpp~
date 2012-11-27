#include "Bonus.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

const double bonus_speed = 0.045;

Bonus::Bonus(const point &center, const double &r, const int &type) : Circle(center, r) {
	this->type = type;
	this->speed = (((double)rand()/RAND_MAX)*0.7 + 0.3) * bonus_speed;
}

int Bonus::move_bonus(const double &distance, Platform *plat) {
	this->center.y+=distance;
	point P=intersects(plat->getRectangle());
	if(center.y>640+r) return -1;
	else {
		if(P.x!=INF&&P.y!=INF) return this->type;
		else return 0;
	}
}

int Bonus::get_type() {
	return this->type;
}

double Bonus::getSpeed() {
	return this->speed;
}


