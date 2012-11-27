#ifndef BONUS_H
#define BONUS_H

#include "geo.h"
#include "Platform.h"
#include "Circle.h"

class Bonus : public Circle {
	public:
		Bonus(const point &center, const double &r, const int &type);
		int move_bonus(const double &distance, Platform &plat);
		int get_type();
		double getSpeed();
	private:
		int type;
		double speed;
};

#endif
