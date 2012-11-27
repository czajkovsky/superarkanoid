#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include "Ball.h"
#include "Block.h"
#include "Player.h"
using namespace std;

class Screen{
	public:
		Screen();
		void drawLevel(vector<Bonus> bonuses, vector<Block> blocks, Player &player, int blocksuntouched[]);
		void drawMenu(const int &pos);
		void drawPauseMenu(const int &pause_pos);
	private:
	
		TTF_Font* menufont;
		TTF_Font* titlefont;
		TTF_Font* gamefont;
		SDL_Surface *screen;

		SDL_Surface *img_background;
		
		SDL_Surface *img_ball;
		
		SDL_Color white;
		SDL_Color cyan;
		SDL_Color darkblue;

		SDL_Surface *img_pad_norm;
		SDL_Surface *img_pad_big;
		SDL_Surface *img_pad_small;
	
		
		SDL_Surface *img_block0202;
		SDL_Surface *img_block0201;
		SDL_Surface *img_block0101;
		SDL_Surface *img_block0001;
		SDL_Surface *img_block0302;
		SDL_Surface *img_block0301;
		
		SDL_Surface *img_bonus01;
		SDL_Surface *img_bonus02;
		SDL_Surface *img_bonus03;
		SDL_Surface *img_bonus04;
		SDL_Surface *img_bonus05;
};

#endif
