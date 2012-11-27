#ifndef BLOCK_H
#define BLOCK_H

#include "geo.h"
#include "Rectangle.h"

class Block : public Rectangle {
	public:
		Block(const point &upperleft, const point &bottomright, const int &lives, const int &type);
		bool hit(const int &power);
		void print();
		int get_type();
		int get_lives();
		int get_maxlives();
	private:
		int lives;
		int type;
		int maxlives;
};

#endif
