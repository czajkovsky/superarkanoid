#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <vector>
#include "Ball.h"
#include "Block.h"
#include "Player.h"
using namespace std;

class Screen{
	public:
		Screen();
		void drawLevel(vector<Bonus> bonuses, vector<Block> blocks, Player &player, int blocksuntouched[]);
		void drawMenu(const int &pos, int level_to_play);
		void drawPauseMenu(const int &pause_pos);
	private:
	
		TTF_Font* menufont;
		TTF_Font* titlefont;
		TTF_Font* gamefont;
		SDL_Surface *screen;

		GLuint img_background;
		
		GLuint img_ball;
		
		SDL_Color white;
		SDL_Color cyan;
		SDL_Color darkblue;

		GLuint img_pad_left;
		GLuint img_pad_center;
		GLuint img_pad_right;	
		
		GLuint img_block0202;
		GLuint img_block0201;
		GLuint img_block0101;
		GLuint img_block0001;
		GLuint img_block0302;
		GLuint img_block0301;
		
		GLuint img_bonus01;
		GLuint img_bonus02;
		GLuint img_bonus03;
		GLuint img_bonus04;
		GLuint img_bonus05;
		
		GLuint menu_texture[7];
		SDL_Rect menu_texture_offset[7];
		GLuint pause_menu_texture[6];
		SDL_Rect pause_menu_texture_offset[6];
		GLuint level_texture[3];
		SDL_Rect level_texture_offset[3];
		GLuint number_texture[10];
		SDL_Rect number_texture_offset[10];
		GLuint menu_number_texture[10];
		SDL_Rect menu_number_texture_offset[10];		
};

#endif
