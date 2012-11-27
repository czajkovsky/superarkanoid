#ifndef BONUS_H
#define BONUS_H

#include "geo.h"
#include "Platform.h"

class Bonus {
	public:
		Bonus(const circle &cir, const int &type);
		circle return_circle();
		int move_bonus(const double &distance, Platform &plat);
		int get_type();
		double getSpeed();
	protected:
		point collision(const rectangle &rec);
	private:
		circle cir;
		int type;
		double speed;
};

#endif
