#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_gfxPrimitives.h>
#include "main.h"
//#include "bitmaps.h"
using namespace std;
#define INF 1e9

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

void init() {
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();	
}

const int menu_pos = 5;

int main() {	
	init();

	bool exit = false;
	int pos = 0;
	int level_to_play = 1;
	const int lvl_max = 11;
	const int lives_at_beginning = 4;
	
	int base_level = 1, base_lives = lives_at_beginning, base_points = 0;
	
	Screen screen;
	
	SDL_Event menu_event;

	while (!exit && SDL_WaitEvent(&menu_event)) {
		if (menu_event.type == SDL_QUIT) {
				exit = true;
		}
		else if (menu_event.type == SDL_KEYDOWN) {
			if (menu_event.key.keysym.sym == SDLK_ESCAPE) {
				exit = true;
			}
			else if (menu_event.key.keysym.sym == SDLK_RETURN) {
				switch (pos) {
				case 0: //continue campaign
					{
						rectangle rec;
						point pt;
	
						//platforma
						pt.x = 380;
						pt.y = 600;
						rec.upperleft = pt;
						pt.x = 580;
						pt.y = 590;
						rec.bottomright = pt;
		
						Player player(base_lives, base_level, rec);
						player.setPoints(base_points);
	
						bool died = false;
						
						for (int lvl_counter = base_level; lvl_counter<=lvl_max && !died; lvl_counter++) {
							base_level = lvl_counter;
							base_lives = player.getLives();
							base_points = player.getPoints();
							player.setLevel(lvl_counter);
							Level level(player, screen, lvl_counter, died, exit);
						}
					
					}
					break;
				case 1: //start campaign
					{
						rectangle rec;
						point pt;
	
						//platforma
						pt.x = 380;
						pt.y = 600;
						rec.upperleft = pt;
						pt.x = 580;
						pt.y = 590;
						rec.bottomright = pt;
		
						Player player(lives_at_beginning, 1, rec);
	
						bool died = false;
						
						for (int lvl_counter = 1; lvl_counter<=lvl_max && !died; lvl_counter++) {
							base_level = lvl_counter;
							base_lives = player.getLives();
							base_points = player.getPoints();
							player.setLevel(lvl_counter);
							Level level(player, screen, lvl_counter, died, exit);
						}
					}
					break;
				case 2: // select 1 level
					{
						rectangle rec;
						point pt;
	
						//platforma
						pt.x = 380;
						pt.y = 600;
						rec.upperleft = pt;
						pt.x = 580;
						pt.y = 590;
						rec.bottomright = pt;
		
						Player player(lives_at_beginning, level_to_play, rec);
	
						bool died = false;
						
						Level level(player, screen, level_to_play, died, exit);
					}
					break;
				case 3: // info
					{
					
					
					}
					break;
				case 4: // wyjście
						exit = true;
					break;
				}
			}
			else if (menu_event.key.keysym.sym == SDLK_DOWN) {
				pos = min(menu_pos-1, pos+1);
			}
			else if (menu_event.key.keysym.sym == SDLK_UP) {
				pos = max(0, pos-1);
			}
			else if (pos == 2 && menu_event.key.keysym.sym == SDLK_LEFT) {
				level_to_play = max(1, level_to_play-1);
			}
			else if (pos == 2 && menu_event.key.keysym.sym == SDLK_RIGHT) {
				level_to_play = min(lvl_max, level_to_play+1);
			}
		}

		screen.drawMenu(pos, level_to_play);
	}

	SDL_Quit();

	return 0;
}
