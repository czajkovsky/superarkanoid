#include "Ball.h"
#include "Block.h"
#include "Platform.h"
#include "Border.h"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#define INF 1e9

typedef vector<Block> vbl;
typedef vector<Border> vbo;

const double wsp = 55;

Ball::Ball(int x, int y, int r, vec v, double speed) {
	point p;
	p.x = x;
	p.y = y;
	circle c;
	c.center = p;
	c.r = r;
	this->cir = c;
	this->base_speed = speed;
	this->speed = speed;
	this->time_remaining = 0;
	this->mv = v;
	this->moze_odbic_sie_od_platformy = true;
}

void Ball::change_speed(double speed) {
	this->speed = speed;
}

void Ball::print() {
	cout<<"Ball: "<<this->cir.center.x<<" "<<this->cir.center.y<<"\n";
}

point Ball::collision(rectangle rec) {
	return intersects(cir, rec);
}

void Ball::reflect(point w) {
	vec rrr;
	rrr.x = w.x - cir.center.x;
	rrr.y = w.y - cir.center.y;
	
	this->mv = reflection(mv, rrr);
	
}

void Ball::change_vec(double pl_speed) {
	mv.x -= pl_speed * wsp;
}

//co zwraca opisane jest w Ball.h
int Ball::move(vbl &blocks, vbo &borders, Platform plat) {
	bool col = false;
	bool usun = false;

	rectangle rr;
	point w;

	//zderzenie z obramowaniem
	for (vbo::iterator it = borders.begin(); it != borders.end() && !col && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			if (!(*it).isMurderous()) {
				col = true;
				reflect(w);
				this->moze_odbic_sie_od_platformy = true;
			}
			else {
				usun = true;
				break;
			}
		}
	}


	//zderzenie z blokiem
	for (vbl::iterator it = blocks.begin(); it != blocks.end() && !col && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			col = true;
			reflect(w);
			if ((*it).hit(1)) { // uderzenie z siłą 1 - do zmiany !!!
				cout<<"usuwam...\n";
				blocks.erase(it);
			}
			this->moze_odbic_sie_od_platformy = true;
			break;
		}
	}

	bool plat_col = false;
	//zderzenie z platformą będzie tu ↓
//	if (this->moze_odbic_sie_od_platformy == true) {
		rr = plat.return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			plat_col = true;
			if (this->moze_odbic_sie_od_platformy) {
				change_vec(plat.getSpeed());
				reflect(w);
			}
			this->moze_odbic_sie_od_platformy = false;
		}
//	}
	
	(this->cir).center.x += speed*mv.x;
	(this->cir).center.y += speed*mv.y;	

	if (usun && !plat_col) return 1;
	else if (usun && plat_col) return 2;
	else if (plat_col) return 3;
	else return 4;
}

point Ball::middle() {
	return this->cir.center;
}

double Ball::r() {
	return this->cir.r;
}
