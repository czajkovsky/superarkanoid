#ifndef PLATFORM_H
#define PLATFORM_H

#include "geo.h"
#include "Border.h"
#include "Rectangle.h"

#include <vector>
using namespace std;

class Platform : public Rectangle {
	public:
		Platform(const int &ulx, const int &uly, const int &brx, const int &bry);
		void change_speed(const int &dir); // 1 - lewo   2 - prawo
		void move(vector<Border> &borders);
		double getSpeed();
		void change_size(const int &size);
		void set_size(const int &size); // na razie nie działa - trzeba poprawić !!!!
		int get_size();
	protected:
		double speed;
		double max_speed;
		double step;
		double size;
};

#endif
