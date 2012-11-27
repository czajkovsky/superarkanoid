#ifndef BLOCK_H
#define BLOCK_H

#include "geo.h"

class Block {
	public:
		Block(const rectangle &rec, const int &lives, const int &type);
		bool hit(const int &power);
		void print();
		rectangle return_rectangle();
		int get_type();
		int get_lives();
		int get_maxlives();
	private:
		rectangle rec;
		int lives;
		int type;
		int maxlives;
};

#endif
