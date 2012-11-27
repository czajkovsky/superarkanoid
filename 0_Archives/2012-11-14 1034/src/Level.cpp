#include "Level.h"
//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
using namespace std;

#define INF 1e9

#define WIDTH 960
#define HEIGHT 640

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

vec v;
double speed = 0.0151;
const double ball_r = 12;

bool quit;

Level::Level(Player &player, Screen &screen, int nr, bool &died) {
	this->nr = nr;
	(this->borders).reserve(4);
	(this->blocks).reserve(128);
	(this->bonuses).reserve(64);
	createLevel();
	playLevel(player, screen, died);
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
	rectangle rec;
	point pt;
	
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
		fscanf(f, "%lf%lf", &pt.x, &pt.y);	
		rec.upperleft = pt;
		fscanf(f, "%lf%lf", &pt.x, &pt.y);	
		rec.bottomright = pt;
		int mur;
		fscanf(f, "%d", &mur);
		Border border(rec, (bool)mur);
		(this->borders).push_back(border);		
	}
	
	
	//inicjalizacja bloków
	int strength,type;
	memset(blocksuntouched, 0, 7*sizeof(int));
//	for(int i=0; i<7; i++) printf("%d ", blocksuntouched[i]);
	fscanf(f, "%d", &(this->blocks_left));
	for (int i=0; i<(this->blocks_left); i++) {
		fscanf(f, "%lf%lf", &pt.x, &pt.y);
		pt.x += 3;
		pt.y -= 3;
		rec.upperleft = pt;
		fscanf(f, "%lf%lf", &pt.x, &pt.y);
		pt.x -= 3;
		pt.y += 3;
		rec.bottomright = pt;
		fscanf(f, "%d", &type);

		if(type==1) strength=1;
		else if(type==2) strength=2;
		else if (type==3) strength=INF;
		else strength=0;
		blocksuntouched[type]++;
		Block block(rec, strength, type); 
		(this->blocks).push_back(block);
	}
	
	fclose(f);
}

Uint8 * keystate = SDL_GetKeyState(NULL);

Uint32 my_function(Uint32 intervall, void *parameter) {
	SDL_Event event;
	SDL_UserEvent userevent;
 
	/* inicjalizacja UserEvent */
	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL; 
 
	event.type = SDL_USEREVENT;
	event.user = userevent;
 
	SDL_PushEvent (&event);
	return intervall;
}

void Level::playLevel(Player &player, Screen &screen, bool &died) {
	SDL_Event event;

	player.removeBalls();

	volatile SDL_TimerID timer = SDL_AddTimer(20, my_function, NULL);
	
	while (!quit && SDL_WaitEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quit = true;
			died = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
				died = true;
			}
		}
		else if (event.type == SDL_USEREVENT) {
			int running = 50;
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
						int usun = (*it).move(blocks, borders, bonuses, player, blocksuntouched, blocks_left);
						//(*it).print();
	
						if (usun == 1 || usun == 2) {
							it--;
							player.balls.erase(it+1);
							if (player.reduceBalls()) {
								if (player.reduceLives()) {
									quit = true;
									died = true;
								}
								bonuses.clear();
								printf("Lives: %d\n", player.getLives());							
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
						int movestatus=(*it).move_bonus(speed,player);
						if(movestatus!=0) {
							/*
							tutaj wleci kod wywołujący możliwości bonusa, wyjątek dla movestatus=-1, inaczej movestatus zwraca typ
							statystycznie co 3. zbity klocek do bonus (do ustawienia w ball.cpp)
	
							1 - zmniejsz platformę
							2 - zwiększ platformę
							3 - dodaj border na dole
	
							*/
							//cout << "bonus typu (" << movestatus << ")\n";
	
						
							if(movestatus==1) player.change_size(-1);
							else if(movestatus==2) player.change_size(1);
							else if(movestatus==3) { if(blocksuntouched[0]==0) blocksuntouched[0]=1; }
							else if(movestatus==4) {
								rectangle rr = player.return_rectangle();
								point p;
								p.x = (rr.upperleft.x+rr.bottomright.x)/2;
								p.y = rr.bottomright.y-ball_r;
								double vec_length = 10;
								
								v.x = -4+(double)rand()/RAND_MAX*8;
								v.y = -sqrt(vec_length*vec_length-v.x*v.x);
								Ball ball(p.x,p.y-10,ball_r,v,speed,borders.end(),blocks.end());
								player.balls.push_back(ball);
								player.moreBalls(1);
								
								v.x = -4+(double)rand()/RAND_MAX*8;
								v.y = -sqrt(vec_length*vec_length-v.x*v.x);
								Ball ball2(p.x,p.y-10,ball_r,v,speed,borders.end(),blocks.end());
								player.balls.push_back(ball2);
								
								player.moreBalls(1);
							}
							else if(movestatus==5) player.addLives(1);
			
						
							it--;
							bonuses.erase(it+1);			
						}
					}	
	
					//przesuwanie platrofmy
					if (ruch_mozliwy) player.move(borders/*, balls*/);
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
				
					rectangle rr = player.return_rectangle();
					point p;
					p.x = (rr.upperleft.x+rr.bottomright.x)/2;
					p.y = rr.bottomright.y-ball_r;
				
					if (player.balls.begin() == player.balls.end()) {
						double vec_length = 10;
						v.x = -4+(double)rand()/RAND_MAX*8;
						v.y = -sqrt(vec_length*vec_length-v.x*v.x);
						//cout<<v.x<<" "<<v.y<<"\n";
						//v.x=vec_length;
						//v.y=0;
						Ball ball(p.x,p.y,ball_r,v,0,borders.end(),blocks.end());
						player.balls.push_back(ball);
					}
					if (keystate[SDLK_SPACE]) {
						player.balls[0].change_speed(speed);
						player.addBalls(1);
					}
				
					player.balls[0].change_pos(p);
				}
			}
		}
		//cout<<running<<"\n";

		//rysowanie tutaj ↓
		screen.drawLevel(bonuses, blocks, player, blocksuntouched);

	}
	//if (!died) SDL_Delay(1000);
	SDL_RemoveTimer(timer);
	
}		
