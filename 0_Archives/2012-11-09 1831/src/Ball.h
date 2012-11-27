#ifndef BALL_H
#define BALL_H
#include "geo.h"
#include "Block.h"
#include "Platform.h"
#include "Border.h"
#include "Bonus.h"
//#include "main.h"
#include <vector>

class Ball {
	public:
		Ball(int x, int y, int r, vec v, double speed, std::vector<Border>::iterator it);
		void change_speed(double speed);
		int move(std::vector<Block> &blocks, std::vector<Border> &borders, std::vector<Bonus> &bonuses, Platform plat, int blocksuntouched[]); //zwraca 1 jeśli trzeba usunąć piłkę i nie było kolizji,
												// 2 jeśli była kolizja z platformą i trzeba usunąć,
												// 3 jeśli była kolizja z platformą i nie trzeba usunąć, 4 else
		void print();
		point middle();
		double r();
		void change_vec(double pl_speed, double dist);
		void change_pos(point p);
	protected:
		point collision(rectangle rec);
		void reflect(point w);
	private:
		circle cir;
		vec mv;
		double base_speed;
		double speed, time_remaining;
		bool moze_odbic_sie_od_platformy;
		std::vector<Border>::iterator lastBorderReflection;
};

#endif
