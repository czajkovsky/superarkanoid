#include "Platform.h"
#include "Ball.h"
#include "variables.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

typedef vector<Border> vbo;
//typedef vector<Ball> vba;


Platform::Platform(rectangle rec) {
	this->rec = rec;
	this->max_speed = 0.05;
	this->speed = 0;
	this->step = 0.00007;
}

rectangle Platform::return_rectangle() {
	return this->rec;
}

void Platform::change_speed(int dir) {
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

void Platform::move(vbo &borders/*, vba &balls*/) {
	rec.upperleft.x += speed;
	rec.bottomright.x += speed;

	bool col = false;

	for (vbo::iterator it = borders.begin(); it != borders.end() && !col; it++) {
		if (intersects(rec, (*it).return_rectangle())) col = true;
	}

/*	for (vba::iterator it = balls.begin(); it != balls.end() && !col; it++) {

	}
*/
	if (col) {
		rec.upperleft.x = rec.upperleft.x - speed;
		rec.bottomright.x = rec.bottomright.x - speed;
		speed = 0;
//		cout<<rec.upperleft.x<<" "<<rec.bottomright.x<<endl;
	}
	
}
