#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "main.h"
using namespace std;

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

bool isInteger(char *s);
bool isInteger(char *s);
void init(char *path, int &lvl_max, int &lives_at_beginning, int &ulx, int &uly, int &brx, int &bry, int &base_lives, int &base_level, int &base_points, int &unlocked);
void save_init(char *path, int lvl_max, int lives_at_beginning, int ulx, int uly, int brx, int bry, int base_lives, int base_level, int base_points, int unlocked);

const int menu_pos = 5;

int main(int argc, char *argv[]) {	
	int level_to_play = 1;
	int lvl_max;
	int lives_at_beginning;
	int ulx,uly,brx,bry;
	int base_lives, base_level, base_points, unlocked;
	
	char init_path[16] = "in/init";
	init(init_path, lvl_max, lives_at_beginning, ulx, uly, brx, bry, base_lives, base_level, base_points, unlocked);

	bool exit = false;
	int pos = 0;
	
	Screen *screen = new Screen();
	Level *level;
	Player *player;
	
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
						player = new Player(base_lives, base_level, ulx, uly, brx, bry);
						player->setPoints(base_points);
	
						bool died = false;
						
						for (int lvl_counter = base_level; lvl_counter<=lvl_max && !died; lvl_counter++) {
							unlocked = max(unlocked, lvl_counter);
							base_level = lvl_counter;
							base_lives = player->getLives();
							base_points = player->getPoints();
							player->setLevel(lvl_counter);
							level = new Level(player, screen, lvl_counter, died, exit);
							delete(level);
						}

						//wyświetlanie wyniku
						bool info = true;
						while (!exit && info) {
							while(SDL_PollEvent(&menu_event)) {
								if (menu_event.type == SDL_QUIT) {
									exit = true;
								}
								else if (menu_event.type == SDL_KEYDOWN) {
									if (menu_event.key.keysym.sym == SDLK_ESCAPE) {
										info = false;
									}
									else if (menu_event.key.keysym.sym == SDLK_RETURN) {
										info = false;
									}								
								}
							}
							screen->drawFinalScore(player->getPoints());
						}

						delete(player);
					}
					break;
				case 1: //start campaign
					{
						player = new Player(lives_at_beginning, 1, ulx, uly, brx, bry);
	
						bool died = false;
						
						for (int lvl_counter = 1; lvl_counter<=lvl_max && !died; lvl_counter++) {
							unlocked = max(unlocked, lvl_counter);
							base_level = lvl_counter;
							base_lives = player->getLives();
							base_points = player->getPoints();
							player->setLevel(lvl_counter);
							level = new Level(player, screen, lvl_counter, died, exit);
							delete(level);
						}
						//wyświetlanie wyniku
						bool info = true;
						while (!exit && info) {
							while(SDL_PollEvent(&menu_event)) {
								if (menu_event.type == SDL_QUIT) {
									exit = true;
								}
								else if (menu_event.type == SDL_KEYDOWN) {
									if (menu_event.key.keysym.sym == SDLK_ESCAPE) {
										info = false;
									}
									else if (menu_event.key.keysym.sym == SDLK_RETURN) {
										info = false;
									}								
								}
							}
							screen->drawFinalScore(player->getPoints());
						}
						delete(player);
					}
					break;
				case 2: // select 1 level
					{
						player = new Player(lives_at_beginning, level_to_play, ulx, uly, brx, bry);
	
						bool died = false;
						
						level = new Level(player, screen, level_to_play, died, exit);
						delete(level);
					}
					delete(player);
					break;
				case 3: // info
					{
						bool info = true;
						while (!exit && info) {
							while(SDL_PollEvent(&menu_event)) {
								if (menu_event.type == SDL_QUIT) {
									exit = true;
								}
								else if (menu_event.type == SDL_KEYDOWN) {
									if (menu_event.key.keysym.sym == SDLK_ESCAPE) {
										info = false;
									}
									else if (menu_event.key.keysym.sym == SDLK_RETURN) {
										info = false;
									}								
								}
							}
							screen->drawInfo();
						}					
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
				level_to_play = min(unlocked, level_to_play+1);
			}
		}

		screen->drawMenu(pos, level_to_play);
	}
	
	delete(screen);
	
	save_init(init_path, lvl_max, lives_at_beginning, ulx, uly, brx, bry, base_lives, base_level, base_points, unlocked);

	return 0;
}


bool isInteger(char *s) {
	for (int i=0; s[i] != '\0'; i++) {
		if (s[i] < '0' || s[i] > '9') return false;
	}
	return true;
}

int parseString(char *s) {
	int res = 0;
	for (int i=0; s[i] != '\0'; i++) {
		res  *= 10;
		res += int(s[i] - '0');
	}
	return res;
}

void save_init(char *path, int lvl_max=11, int lives_at_beginning=3, int ulx=380, int uly=600, int brx=580, int bry=590, int base_lives=3, int base_level=1, int base_points=0, int unlocked=1) {
	try {
		FILE *f;
		if ((f=fopen(path, "w")) == NULL) {
			throw 1;
		}
		
		fprintf(f, "%d\n", lvl_max);
		fprintf(f, "%d\n", lives_at_beginning);
		fprintf(f, "%d ", ulx);
		fprintf(f, "%d ", uly);
		fprintf(f, "%d ", brx);
		fprintf(f, "%d\n", bry);
		fprintf(f, "%d ", base_lives);
		fprintf(f, "%d ", base_level);
		fprintf(f, "%d\n", base_points);
		fprintf(f, "%d", unlocked);
		
		fclose(f);
		printf("Init file saved\n");
	}
	catch (int e) {
		if (e == 1) printf("No init file\n");
	}
}

void init(char *path, int &lvl_max, int &lives_at_beginning, int &ulx, int &uly, int &brx, int &bry, int &base_lives, int &base_level, int &base_points, int &unlocked) {
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_WM_SetCaption("Super Arkanoid", NULL);
	atexit(SDL_Quit);	
	
	try {
		FILE *f;
		if ((f=fopen(path, "r")) == NULL) {
			throw 1;
		}
		
		char t[10][16];
		for (int i=0; i<10; i++) {
			fscanf(f, "%s", t[i]);
			if (!isInteger(t[i])) {
				fclose(f);
				throw 2;
			}
		}
		
		lvl_max = parseString(t[0]);
		lives_at_beginning = parseString(t[1]);
		ulx = parseString(t[2]);
		uly = parseString(t[3]);
		brx = parseString(t[4]);
		bry = parseString(t[5]);
		base_lives = parseString(t[6]);
		base_level = parseString(t[7]);
		base_points = parseString(t[8]);
		unlocked = parseString(t[9]);
		
		fclose(f);
		printf("Init file loaded\n");
	}
	catch (int e) {
		if (e == 1) printf("No init file\n");
		else if (e == 2) printf("Bad init file\n");
		lvl_max = 11;
		lives_at_beginning = 3;
		ulx = 380;
		uly = 600;
		brx = 580;
		bry = 590;
		base_lives = 3;
		base_level = 1;
		base_points = 0;
		unlocked = 1;
		save_init(path);
	}	
}
