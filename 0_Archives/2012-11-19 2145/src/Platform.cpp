#include "Platform.h"
#include "Ball.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

typedef vector<Border> vbo;
//typedef vector<Ball> vba;


Platform::Platform(const rectangle &rec) {
	this->rec = rec;
	this->max_speed = 0.117;
	this->speed = 0;
	this->step = 0.00085;
	this->size = rec_width((this->rec))-6;
}

rectangle Platform::return_rectangle() {
	return this->rec;
}
int Platform::get_size() {
	return this->size;
}
void Platform::change_size(const int &newsize) {
	int cursize=this->size;
	if (newsize < (int)cursize) {
		this->size -= 0.05;
		rec.upperleft.x += 0.025;
		rec.bottomright.x -= 0.025;
	}
	else if (newsize > cursize) {
		this->size += 0.05;
		rec.upperleft.x -= 0.025;
		rec.bottomright.x += 0.025;
		if (rec.upperleft.x <= 2) {
			rec.upperleft.x += 1;
			rec.bottomright.x += 1;
		}
		else if (rec.bottomright.x >= 958) {
			rec.upperleft.x -= 1;
			rec.bottomright.x -= 1;
		}
	}
	//printf("size: %lf\n", this->size);
}

void Platform::set_size(const int &size) {
	this->size = (double)size;
}

void Platform::change_speed(const int &dir) {
	if (dir == 1) { // w lewo
		speed -= step;
		speed = max(speed, -max_speed);
	}
	else if (dir == 2) { // w prawo
		speed += step;
		speed = min(speed, max_speed);
	}
	else  { //hamowanie swobodne
		if (speed > 0) {
			speed -= step;
			speed = max(0.0, speed);
		}
		else {
			speed += step;
			speed = min(0.0, speed);
		}
	}

}

double Platform::getSpeed() {
	return speed;
}

void Platform::move(vbo &borders) {
	rec.upperleft.x += speed;
	rec.bottomright.x += speed;

	bool col = false;

	for (vbo::iterator it = borders.begin(); it != borders.end() && !col; it++) {
		if (!(*it).isMurderous() && intersects(rec, (*it).return_rectangle())) col = true;
	}

	if (col) {
		rec.upperleft.x = rec.upperleft.x - speed;
		rec.bottomright.x = rec.bottomright.x - speed;
		speed = 0;
	}	
}
