#ifndef LEVEL_H
#define LEVEL_H

#include "Block.h"
#include "Border.h"
#include "Bonus.h"
#include "Player.h"
#include "geo.h"
#include <vector>

class Level{
	public:
		Level();
		void createLevel();
		void playLevel();
		int blocksuntouched[7];
		std::vector<Block> blocks;
		std::vector<Border> borders;
		std::vector<Bonus> bonuses;
	private:
		int nr;
};

#endif
