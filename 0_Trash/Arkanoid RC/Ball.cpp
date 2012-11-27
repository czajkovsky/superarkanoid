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

const double dif = 2;

void Ball::change_vec(double pl_speed) {
	vec nowy;
	double d = sqrt(mv.x*mv.x + mv.y*mv.y);
	nowy.x = mv.x - pl_speed * wsp;
	if (nowy.x >= 0) {
		if (nowy.x >= d-dif) nowy.x = min(mv.x, d-dif);
		else if (nowy.x <= dif-d) nowy.x = max(mv.x, dif-d);
	}
	else {
		if (nowy.x >= d-dif) nowy.x = max(mv.x, d-dif);
		else if (nowy.x <= dif-d) nowy.x = min(mv.x, dif-d);
	}
	nowy.y = sqrt(d*d - nowy.x*nowy.x);
	mv = nowy;
}

//co zwraca opisane jest w Ball.h
int Ball::move(vbl &blocks, vbo &borders, Platform plat) {
	bool col = false;
	bool usun = false;

	rectangle rr;
	point w;

	int cc = 0;

	//zderzenie z obramowaniem
	for (vbo::iterator it = borders.begin(); it != borders.end()/* && !col*/ && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			change_vec(0);
			if (!(*it).isMurderous()) {
				col = true;
				cc++;
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
	for (vbl::iterator it = blocks.begin(); it != blocks.end()/* && !col*/ && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			col = true;
			cc++;
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
			cc++;
			if (this->moze_odbic_sie_od_platformy) {
				change_vec(plat.getSpeed());
				reflect(w);
			}
			this->moze_odbic_sie_od_platformy = false;
		}
//	}
	
	if (cc > 1) cout<<"errrrr\n";
	if (cc > 1 && plat_col) usun = true;
	
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
