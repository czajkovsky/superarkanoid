#ifndef LEVEL_H
#define LEVEL_H

#include "Block.h"
#include "Border.h"
#include "Bonus.h"
#include "Player.h"
#include "Ball.h"
#include "geo.h"
#include "Screen.h"
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using namespace std;

class Level{
	public:
		Level(Player *player, Screen *screen, const int &nr, bool &died, bool &exit);
		void createLevel();
		void playLevel(Player *player, Screen *screen, bool &died, bool &exit);		
	protected:
		vector<Block> blocks;
		vector<Border> borders;
		vector<Bonus> bonuses;
		int blocksuntouched[7];		
	private:
		int nr, blocks_left;
};

#endif
