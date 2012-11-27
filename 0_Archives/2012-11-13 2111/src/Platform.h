#ifndef PLATFORM_H
#define PLATFORM_H

#include "geo.h"
#include "Border.h"

#include <vector>

class Platform {
	public:
		Platform(rectangle rec);
		void change_speed(int dir); // 1 - lewo   2 - prawo
		void move(std::vector<Border> &borders/*, std::vector<Ball> &balls*/);
		rectangle return_rectangle();
		double getSpeed();
		void change_size(int size);
		int get_size();
	protected:
		
	private:
		rectangle rec;
		double speed;
		double max_speed;
		double step;
		int size; //0 - normalna, 1 - duza, -1 - mala
};

#endif
