#ifndef BONUS_H
#define BONUS_H

#include "geo.h"
#include "Platform.h"

class Bonus {
	public:
		Bonus(circle cir, int type);
		circle return_circle();
		int move_bonus(double distance, Platform plat);
		int get_type();
	protected:
		point collision(rectangle rec);
	private:
		circle cir;
		int type;
};

#endif
