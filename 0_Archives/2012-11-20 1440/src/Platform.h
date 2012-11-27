#ifndef PLATFORM_H
#define PLATFORM_H

#include "geo.h"
#include "Border.h"

#include <vector>
using namespace std;

class Platform {
	public:
		Platform(const rectangle &rec);
		void change_speed(const int &dir); // 1 - lewo   2 - prawo
		void move(vector<Border> &borders);
		rectangle return_rectangle();
		double getSpeed();
		void change_size(const int &size);
		void set_size(const int &size); // na razie nie działa - trzeba poprawić !!!!
		int get_size();
	protected:
		
	private:
		rectangle rec;
		double speed;
		double max_speed;
		double step;
		double size; //0 - normalna, 1 - duza, -1 - mala
};

#endif
