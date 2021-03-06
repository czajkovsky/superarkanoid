#include "Level.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>
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

Level::Level(Player &player, Screen &screen) {
	createLevel();
	playLevel(player, screen);
}

void Level::createLevel() {
	rectangle rec;
	point pt;
	
	//wczytywanie obramowania
	int k;
	cin>>k;
	for (int i=0; i<k; i++) {
		cin>>pt.x>>pt.y;		
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;		
		rec.bottomright = pt;
		int mur;
		cin>>mur;
		Border border(rec, (bool)mur);
		(this->borders).push_back(border);		
	}
	
	
	//inicjalizacja bloków
	int strength,type;
	int n;
	for(int i=0; i<7; i++) blocksuntouched[i]=0;
	cin>>n;
	for (int i=0; i<n; i++) {
		cin>>pt.x>>pt.y;
		pt.x += 3;
		pt.y -= 3;
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;
		pt.x -= 3;
		pt.y += 3;
		rec.bottomright = pt;
		cin>>type;

		if(type==1) strength=1;
		else if(type==2) strength=2;
		else if (type==3) strength=INF;
		else strength=0;
		blocksuntouched[type]++;
		Block block(rec, strength, type); 
		(this->blocks).push_back(block);
	}
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
 
	/* ... initialisiere damit einen Event */
	event.type = SDL_USEREVENT;
	event.user = userevent;
 
	/* neuen Event erzeugen */
	SDL_PushEvent (&event);
	return intervall;
}

void Level::playLevel(Player &player, Screen &screen) {
	bool quit = false;
	SDL_Event event;

	volatile SDL_TimerID timer = SDL_AddTimer(20, my_function, NULL);
	
	while (!quit && SDL_WaitEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}
		if (event.type == SDL_USEREVENT) {
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
						int usun = (*it).move(blocks, borders, bonuses, player, blocksuntouched);
						//(*it).print();
	
						if (usun == 1 || usun == 2) {
							it--;
							player.balls.erase(it+1);
							if (player.reduceBalls()) {
								player.reduceLives();
								//cout<<"Lives: "<<player.getLives()<<"\n";
							}
						}
						else if (usun == 3) ruch_mozliwy = false;
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
							else if(movestatus==3) if(blocksuntouched[0]==0) blocksuntouched[0]=1;
			
						
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
}		
