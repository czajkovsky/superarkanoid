#ifndef BALL_H
#define BALL_H

#include "geo.h"
#include "Block.h"
#include "Platform.h"
#include "Border.h"
#include "Bonus.h"
#include "Circle.h"
#include "Rectangle.h"
#include <vector>
using namespace std;

class Ball : public Circle {
	public:
		Ball(const int &x, const int &y, const int &r, const vec &v, const double &speed);
		void change_speed(const double &speed);
		int move(vector<Block> &blocks, vector<Border> &borders, vector<Bonus> &bonuses, Platform *plat, int blocksuntouched[], int &blocks_left, int &new_points);
								//zwraca 1 jeśli trzeba usunąć piłkę i nie było kolizji,
								// 2 jeśli była kolizja z platformą i trzeba usunąć,
								// 3 jeśli była kolizja z platformą i nie trzeba usunąć, 4 else
		void print();
		void change_vec(const double &pl_speed, const double &dist);
		void change_pos(const point &p);
	protected:
		point collision(const Rectangle &rec);
		void reflect(const point &w);
	private:
		vec mv;
		double base_speed;
		double speed, time_remaining;
		bool moze_odbic_sie_od_platformy;
		Rectangle bon_bl;
		vector<Block> lBR, lBR_tmp;
		vector<Border> lBoR, lBoR_tmp;
		bool find(Block &bl);
		bool find(Border &bo);		
};

#endif
