#include "Level.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
using namespace std;

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

vec v;
double speed = 0.015;
const double ball_r = 12;

bool quit;

Level::Level(Player &player, Screen &screen, const int &nr, bool &died, bool &exit) {
	this->nr = nr;
	(this->borders).reserve(4);
	(this->blocks).reserve(128);
	(this->bonuses).reserve(64);
	createLevel();
	playLevel(player, screen, died, exit);
}

char path[16] = "in/blocks";

void parse_nr(int nr) {
	int count = 0;
	char tmp[8];
	while (nr) {
		tmp[count++] = (char)(nr%10 + '0');
		nr /= 10;
	}
	for (int i=0; i<count; i++) {
		path[8+count-i] = tmp[i];
	}
	path[9+count] = '\0';
	printf("%s\n", path);
}

void Level::createLevel() {
	quit = false;
	point ul, br;
	
	FILE *f;
	parse_nr(this->nr);
	if ((f=fopen(path, "r")) == NULL) {
		printf("file error\n");
		quit = true;
		return;
	}
	
	//wczytywanie obramowania
	int k;
	fscanf(f, "%d", &k);
	for (int i=0; i<k; i++) {
		fscanf(f, "%lf%lf", &ul.x, &ul.y);	
		fscanf(f, "%lf%lf", &br.x, &br.y);	
		int mur;
		fscanf(f, "%d", &mur);
		Border border(ul, br, (bool)mur);
		(this->borders).push_back(border);		
	}
	
	
	//inicjalizacja bloków
	int strength,type;
	memset(blocksuntouched, 0, 7*sizeof(int));
	fscanf(f, "%d", &(this->blocks_left));
	for (int i=0; i<(this->blocks_left); i++) {
		fscanf(f, "%lf%lf", &ul.x, &ul.y);
		ul.x += 3;
		ul.y -= 3;
		fscanf(f, "%lf%lf", &br.x, &br.y);
		br.x -= 3;
		br.y += 3;
		fscanf(f, "%d", &type);

		if(type==1) strength=1;
		else if(type==2) strength=2;
		else if (type==3) strength=INF;
		else strength=0;
		blocksuntouched[type]++;
		Block block(ul, br, strength, type); 
		(this->blocks).push_back(block);
	}
	
	fclose(f);
}

Uint8 * keystate = SDL_GetKeyState(NULL);

const int FRAMES_PER_SECOND = 200;

void Level::playLevel(Player &player, Screen &screen, bool &died, bool &exit) {
	SDL_Event event;

	player.removeBalls();
	//player.set_size(194);

	int time_start, time_left;
	
	double def_size = (double)player.get_size();
	
	while (!quit) {
		time_start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
				died = true;
				exit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) { //menu pauzy
					bool pause = true;
					int pause_pos = 1, pause_max = 4;
					SDL_Event pause_menu_event;
					screen.drawLevel(bonuses, blocks, player, blocksuntouched); // zapamiętanie stanu poziomu w buforze 1
					screen.drawLevel(bonuses, blocks, player, blocksuntouched); // zapamiętanie stanu poziomu w buforze 2
					screen.drawPauseMenu(pause_pos);
					while (pause && SDL_WaitEvent(&pause_menu_event)) {
						if (pause_menu_event.type == SDL_QUIT) {
							quit = true;
							died = true;
							exit = true;
							pause = false;
						}
						else if (pause_menu_event.type == SDL_KEYDOWN) {
							if (pause_menu_event.key.keysym.sym == SDLK_ESCAPE) {
								pause = false;
							}
							else if (pause_menu_event.key.keysym.sym == SDLK_DOWN) {
								pause_pos = min(pause_max, pause_pos+1);
							}
							else if (pause_menu_event.key.keysym.sym == SDLK_UP) {
								pause_pos = max(1, pause_pos-1);
							}
							else if (pause_menu_event.key.keysym.sym == SDLK_RETURN) {
								switch(pause_pos) {
									case 1: // continue
										pause = false;
									break;
									case 2: // controls
									
									break;
									case 3: // main menu
										pause = false;
										quit = true;
										died = true;
									break;
									case 4: // exit
										pause = false;
										quit = true;
										died = true;
										exit = true;
									break;						
								}
							}
							
						}
						screen.drawPauseMenu(pause_pos);					
					}
				}
			}
		}
		int running = 13;
		while (running > 0) {
			running--;			
			if (player.getBallsAlive()>0) {
				if (keystate[SDLK_LEFT]) {
					player.change_speed(1);
				}
				else if (keystate[SDLK_RIGHT]) {
					player.change_speed(2);
				}
				else {
					player.change_speed(0);
				}		
	
				//obsługa piłek
				bool ruch_mozliwy = true;
				for (vba::iterator it = player.balls.begin(); it != player.balls.end(); it++) {
					int new_points;
					int usun = (*it).move(blocks, borders, bonuses, player, blocksuntouched, blocks_left, new_points);
					player.addPoints(new_points*nr);
	
					if (usun == 1 || usun == 2) {
						it--;
						player.balls.erase(it+1);
						if (player.reduceBalls()) {
							if (player.reduceLives()) {
								quit = true;
								died = true;
							}
							bonuses.clear();
							blocksuntouched[0] = 0;				
							//tu jeszcze trzeba dać czyszczenie efektów bonusów - przywrócenie stanu platformy
						}
					}
					else if (usun == 3) ruch_mozliwy = false;
					
					if (blocks_left < 1) {
						quit = true;
					}
					
				}

				//kolizja bonusu z platformą
				for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
					int movestatus=(*it).move_bonus((*it).getSpeed(),player);
					if(movestatus!=0) {
						/*
						tutaj wleci kod wywołujący możliwości bonusa, wyjątek dla movestatus=-1, inaczej movestatus zwraca typ
						statystycznie co 3. zbity klocek do bonus (do ustawienia w ball.cpp)
							1 - zmniejsz platformę
							2 - zwiększ platformę
							3 - dodaj border na dole
							4 - dodaj 2 nowe piłki
							5 - dodaj życie
						*/
						//cout << "bonus typu (" << movestatus << ")\n";	
						
						if(movestatus==1) {
							def_size = max(player.get_size()-38, 118);
						}
						else if(movestatus==2) {
							def_size = min(player.get_size()+38, 270);
						}
						else if(movestatus==3) {
							if(blocksuntouched[0]==0)
								blocksuntouched[0]=1;
						}
						else if(movestatus==4) {
							//rectangle rr = player.return_rectangle();
							point p;
							p.x = (player.getUpperLeft().x+player.getBottomRight().x)/2;
							p.y = player.getBottomRight().y-ball_r;
							double vec_length = 10;
							
							v.x = -4+(double)rand()/RAND_MAX*8;
							v.y = -sqrt(vec_length*vec_length-v.x*v.x);
							Ball ball(p.x,p.y-10,ball_r,v,speed);
							player.balls.push_back(ball);
							player.moreBalls(1);
							
							v.x = -4+(double)rand()/RAND_MAX*8;
							v.y = -sqrt(vec_length*vec_length-v.x*v.x);
							Ball ball2(p.x,p.y-10,ball_r,v,speed);
							player.balls.push_back(ball2);
							
							player.moreBalls(1);
						}
						else if(movestatus==5) player.addLives(1);		
					
						it--;
						bonuses.erase(it+1);			
					}
				}
	
				//zmiana dlugości platformy
				player.change_size(def_size);
					
				//przesuwanie platrofmy
				if (ruch_mozliwy) player.move(borders);					
					
			}
			else {
				if (keystate[SDLK_LEFT]) {
					player.change_speed(1);
				}
				else if (keystate[SDLK_RIGHT]) {
					player.change_speed(2);
				}
				else {
					player.change_speed(0);
				}
				player.move(borders);
				
				point p;
				p.x = (player.getUpperLeft().x+player.getBottomRight().x)/2;
				p.y = player.getBottomRight().y-ball_r;
				//p.x=946.9;
			
				if (player.balls.begin() == player.balls.end()) {
					double vec_length = 10;
					//v.x = 0.0;
					//v.y = -10.0;
					v.x = -4+(double)rand()/RAND_MAX*8;
					v.y = -sqrt(vec_length*vec_length-v.x*v.x);
					//cout<<v.x<<" "<<v.y<<"\n";
					//v.x=vec_length;
					//v.y=0;
					Ball ball(p.x,p.y,ball_r,v,0);
					player.balls.push_back(ball);
				}
				if (keystate[SDLK_SPACE]) {
					player.balls[0].change_speed(speed);
					player.addBalls(1);
				}
			
				player.balls[0].change_pos(p);
			}
		}

		//rysowanie levelu ↓
		screen.drawLevel(bonuses, blocks, player, blocksuntouched);

		//wyrównywanie fps
		time_left = SDL_GetTicks() - time_start;
		if(time_left < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000 / FRAMES_PER_SECOND) - time_left);
		}
	}
}		
