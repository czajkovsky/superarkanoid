#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//#include <SDL/SDL_gfxPrimitives.h>
#include "main.h"
using namespace std;
#define INF 1e9

//typedef vector<Ball> vba;
//typedef vector<Block> vbl;

Uint8 * keystate = SDL_GetKeyState( NULL );

int main() {
	ios_base::sync_with_stdio(0);

	vbo borders;
	vbl blocks;

	rectangle rec;
	point pt;
	int strength,type;

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
		borders.push_back(border);		
	}

	/* 
		block types
		1 - red: 1shot
		2 - green: 2shots
		3 - yelllow: all in colour

	*/

	int n,blocksuntouched[7];
	for(int i=0; i<7; i++) blocksuntouched[i]=0;
	cin>>n;
	for (int i=0; i<n; i++) {
		cin>>pt.x>>pt.y;
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;
		rec.bottomright = pt;
		cin>>type;

		if(type==1) strength=1;
		else if(type==2) strength=2;
		else if (type==3) strength=INF;
		else strength=0;
		blocksuntouched[type]++;
		Block block(rec, strength, type); 
		blocks.push_back(block);
	}

	vba balls;
	int balls_alive = 0;
	vbn bonuses;	

	vec v;
	
	//pierwsza piłka
	v.x = 4;
	v.y = -9;
	double speed = 0.0054;
	double ball_r = 12;
	//Ball ball(450,490,12,v,speed);

	//balls.push_back(ball);
	//balls_alive++;

	//platforma
	pt.x = 380;
	pt.y = 600;
	rec.upperleft = pt;
	pt.x = 580;
	pt.y = 590;
	rec.bottomright = pt;
	Platform platform(rec);


	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));


	bool quit = false;
	SDL_Event event;

	SDL_Surface *img_background = SDL_DisplayFormatAlpha(IMG_Load("res/img/background.png"));

	SDL_Surface *img_ball = SDL_DisplayFormatAlpha(IMG_Load("res/img/ball.png"));

	SDL_Surface *img_pad_norm = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_norm.png"));
	SDL_Surface *img_pad_big = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_big.png"));
	SDL_Surface *img_pad_small = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_small.png"));

	SDL_Surface *img_block0202 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green02.png"));
	SDL_Surface *img_block0201 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green01.png"));
	SDL_Surface *img_block0101 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_red01.png"));
	SDL_Surface *img_block0302 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_yellow02.png"));
	SDL_Surface *img_block0301 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_yellow01.png"));

	SDL_Surface *img_bonus01 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn01.png"));
	SDL_Surface *img_bonus02 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn02.png"));

	//SDL_FreeSurface(source);

	//SDL_EnableKeyRepeat(1,0);

	Uint32 deadline = SDL_GetTicks();

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
			         quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
			}
		}
		deadline = SDL_GetTicks() + 8;
		int running = 170;
		while (deadline > SDL_GetTicks()) {
			if (running > 0) {
				running--;			
				if (balls_alive>0) {
				if (keystate[SDLK_LEFT]) {
					platform.change_speed(1);
				}
				else if (keystate[SDLK_RIGHT]) {
					platform.change_speed(2);
				}
				else {
					platform.change_speed(0);
				}		
	
				//obsługa piłek
				bool ruch_mozliwy = true;
				for (vba::iterator it = balls.begin(); it != balls.end(); it++) {
					int usun = (*it).move(blocks, borders, bonuses, platform, blocksuntouched);
					//(*it).print();
	
					if (usun == 1 || usun == 2) {
						it--;
						balls.erase(it+1);
						balls_alive--;
					}
					else if (usun == 3) ruch_mozliwy = false;
				}

				//kolizja bonusu z platformą

				for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
					int movestatus=(*it).move_bonus(speed,platform);
					if(movestatus!=0) {
						/*
						tutaj wleci kod wywołujący możliwości bonusa, wyjątek dla movestatus=-1, inaczej movestatus zwraca typ
						statystycznie co 3. zbity klocek do bonus (do ustawienia w ball.cpp)

						1 - zmniejsz platformę
						2 - zwiększ platformę
						3 - dodaj border na dole

						*/
						//cout << "bonus typu (" << movestatus << ")\n";


						if(movestatus==1) platform.change_size(-1);
						else if(movestatus==2) platform.change_size(1);
	
						it--;
						bonuses.erase(it+1);			
					}
				}	

				//przesuwanie platrofmy
				if (ruch_mozliwy) platform.move(borders/*, balls*/);
				}
				else {
					if (keystate[SDLK_LEFT]) {
						platform.change_speed(1);
					}
					else if (keystate[SDLK_RIGHT]) {
						platform.change_speed(2);
					}
					else {
						platform.change_speed(0);
					}
					platform.move(borders);
				
					rectangle rr = platform.return_rectangle();
					point p;
					p.x = (rr.upperleft.x+rr.bottomright.x)/2;
					p.y = rr.bottomright.y-ball_r;
				
					if (balls.begin() == balls.end()) {
					
						Ball ball(p.x,p.y,ball_r,v,0,borders.end());
						balls.push_back(ball);
					}
					if (keystate[SDLK_SPACE]) {
						balls[0].change_speed(speed);
						balls_alive++;
					}
				
					balls[0].change_pos(p);
				}
			}
		}
		//cout<<running<<"\n";

		//rysowanie tutaj ↓
		//tło
		SDL_BlitSurface(img_background, NULL, screen, NULL);		

		//obramowanie

		/*

		for (vbo::iterator it = borders.begin(); it != borders.end(); it++) {
			Border bo = *it;

			SDL_Rect clip;
			clip.x = 0;
			clip.y = 0;
			clip.w = rec_width(bo.return_rectangle());
			clip.h = rec_height(bo.return_rectangle());

			SDL_Rect offset;
			offset.x = bo.return_rectangle().upperleft.x;
			offset.y = bo.return_rectangle().bottomright.y;

			
			//
			//SDL_BlitSurface(img_block, &clip, screen, &offset);
		}

		*/

		//piłki
		for (vba::iterator it = balls.begin(); it != balls.end(); it++) {
				SDL_Rect offset;
				offset.x = (*it).middle().x - (*it).r();
				offset.y = (*it).middle().y - (*it).r();

				SDL_BlitSurface(img_ball, NULL, screen, &offset);
		}
		
		//wyświetlanie bloków
		
		int yellowblock=3;

		if(blocksuntouched[yellowblock]==0) {
			for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++)
				if((*it).get_type()==yellowblock) {
					it--;
					blocks.erase(it+1);
				}
			blocksuntouched[yellowblock]=-1;
		}
	
		for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++) {
			Block bl = *it;
			//bl.print();

			SDL_Rect clip;

			clip.x = 0;
			clip.y = 0;
			clip.w = rec_width(bl.return_rectangle());
			clip.h = rec_height(bl.return_rectangle());

			SDL_Rect offset;
			offset.x = bl.return_rectangle().upperleft.x;
			offset.y = bl.return_rectangle().bottomright.y;

			if(bl.get_type()==1) {
				SDL_BlitSurface(img_block0101, &clip, screen, &offset);
			}
			else if(bl.get_type()==2) {
				if(bl.get_lives()==1) SDL_BlitSurface(img_block0201, &clip, screen, &offset);
				else if(bl.get_lives()==2) SDL_BlitSurface(img_block0202, &clip, screen, &offset);
			}
			else if(bl.get_type()==3) {
				if(bl.get_lives()==bl.get_maxlives()) SDL_BlitSurface(img_block0302, &clip, screen, &offset);
				else SDL_BlitSurface(img_block0301, &clip, screen, &offset);
			}	
		}
		
		
		//wyświetlanie platformy
		SDL_Rect clip;
		clip.x = 0;
		clip.y = 0;
		clip.w = rec_width(platform.return_rectangle())+2; // to działa ale trzeba ujednolicić dokładność przy wyświetlaniu!
		clip.h = rec_height(platform.return_rectangle())+1;

		SDL_Rect offset;
		offset.x = platform.return_rectangle().upperleft.x;
		offset.y = platform.return_rectangle().bottomright.y;

		if(platform.get_size()==0) SDL_BlitSurface(img_pad_norm, &clip, screen, &offset);
		else if(platform.get_size()==-1) SDL_BlitSurface(img_pad_small, &clip, screen, &offset);
		else if(platform.get_size()==1) SDL_BlitSurface(img_pad_big, &clip, screen, &offset);


		//wyswietlania bonusow
		for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
				SDL_Rect offset;
				offset.x = (*it).return_circle().center.x - (*it).return_circle().r;
				offset.y = (*it).return_circle().center.y - (*it).return_circle().r;

				if((*it).get_type()==1) SDL_BlitSurface(img_bonus01, NULL, screen, &offset);
				else  SDL_BlitSurface(img_bonus02, NULL, screen, &offset);				
		}

//		if (czas<SDL_GetTicks()) {
			SDL_Flip(screen);
//			czas = SDL_GetTicks() + 35;
		}
	//	}

 //       }


        SDL_Quit();


/*	vec sp;
	sp.x = 5;
	sp.y = 0;

	vec r;
	r.x = 10;
	r.y = 10;

	vec epic = reflection(sp, r);
	cout<<"odbity: "<<epic.x<<" "<<epic.y<<"\n";

	rectangle rrr;
	point p;
	p.x = 50;
	p.y = 100;
	rrr.upperleft = p;
	p.x = 150;
	p.y = 50;
	rrr.bottomright = p;

	p.x = 52.5;
	p.y = 35;
	circle c;
	c.center = p;
	c.r = 15;

	point w = intersects(c,rrr);
	cout<<"point/circle: "<<w.x<<" "<<w.y<<"\n";;
*/
	return 0;
}
