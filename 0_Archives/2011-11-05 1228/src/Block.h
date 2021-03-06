#ifndef BLOCK_H
#define BLOCK_H

#include "geo.h"

class Block {
	public:
		Block(rectangle rec, int lives);
		bool hit(int power);
		void print();
		rectangle return_rectangle();
		int get_type();
	private:
		rectangle rec;
		int lives;
};

#endif
