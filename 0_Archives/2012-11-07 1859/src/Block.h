#ifndef BLOCK_H
#define BLOCK_H

#include "geo.h"

class Block {
	public:
		Block(rectangle rec, int lives, int type);
		bool hit(int power);
		void print();
		rectangle return_rectangle();
		int get_type();
		int get_lives();
	private:
		rectangle rec;
		int lives;
		int type;
};

#endif
