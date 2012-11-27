#include "Bonus.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#define INF 1e9
#define eps 0.3

const double bonus_speed = 0.045;

Bonus::Bonus(const circle &cir, const int &type) {
	this->cir = cir;
	this->cir.r=18;
	this->type = type;
	this->speed = (((double)rand()/RAND_MAX)*0.7 + 0.3) * bonus_speed;
}
int Bonus::move_bonus(const double &distance, Platform &plat) {
	this->cir.center.y+=distance;
	point P=intersects(this->cir,plat.return_rectangle());
	if(cir.center.y>640+cir.r) return -1;
	else {
		if(P.x!=INF&&P.y!=INF) return this->type;
		else return 0;
	}
}
circle Bonus::return_circle() {
	return this->cir;
}
int Bonus::get_type() {
	return this->type;
}

double Bonus::getSpeed() {
	return this->speed;
}


