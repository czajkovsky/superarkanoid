#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//#include <SDL/SDL_gfxPrimitives.h>
#include "main.h"
using namespace std;

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
		cout<<mur<<"\n";
		Border border(rec, (bool)mur);
		borders.push_back(border);		
	}

	int n;
	cin>>n;
	for (int i=0; i<n; i++) {
		cin>>pt.x>>pt.y;
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;
		rec.bottomright = pt;
		cin>>strength;
		// cos mi się rand() pierdoli
		type=i%2+1;

		//dane w pliku (temp/blockgenerator) będzie trzeba zmienić
		Block block(rec, type, type); 
		blocks.push_back(block);
	}

	vba balls;	

	vec v;
	
	//pierwsza piłka
	v.x = 0;
	v.y = -9;
	double speed = 0.0054;
	Ball ball(450,490,16,v,speed);

	balls.push_back(ball);

	//druga piłka
/*	v.x = -3;
	v.y = -8;
	Ball ball2(550,490,16,v,speed);
	balls.push_back(ball2);
*/
	//platforma

	//to jak to jest z tym układem współrzędnych? lewy górny to 0 0? bo patrząc na tą tackę mam wątpliwości ;p

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

	SDL_Surface *source = IMG_Load("res/img/background.png");
	SDL_Surface *img_background = SDL_DisplayFormatAlpha(source);

	source = IMG_Load("kolo.png");
	SDL_Surface *img_kolo = SDL_DisplayFormatAlpha(source);

	source = IMG_Load("res/img/whitepad_norm.png");
	SDL_Surface *img_pad = SDL_DisplayFormatAlpha(source);

	SDL_Surface *img_block0202 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green02.png"));
	SDL_Surface *img_block0201 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green01.png"));
	SDL_Surface *img_block0101 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_red01.png"));

	source = IMG_Load("czarny.jpg");
	SDL_Surface *img_black = SDL_DisplayFormat(source);
	SDL_FreeSurface(source);

	SDL_EnableKeyRepeat(1,0);

	Uint32 czas = SDL_GetTicks();
	Uint32 deadline = SDL_GetTicks();

	while (!quit) {
		deadline = SDL_GetTicks() + 8;
		int running = 80;
		while (deadline > SDL_GetTicks()) {
			if (running > 0) {
				running--;
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
					int usun = (*it).move(blocks, borders, platform);
//					(*it).print();
	
					if (usun == 1 || usun == 2) {
						it--;
						balls.erase(it+1);
					}
					else if (usun == 3) ruch_mozliwy = false;
				}		

				//przesuwanie platrofmy
				if (ruch_mozliwy) platform.move(borders/*, balls*/);		
			}
		}
//		cout<<running<<"\n";

		//rysowanie tutaj ↓
		//tło
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

		SDL_Rect bgpos;
		bgpos.x = 0;
		bgpos.y = 0;
				
		SDL_BlitSurface(img_background, NULL, screen, &bgpos);		
		

		//obramowanie
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

			
			// obramowanie dla testów, c'nie?
			//SDL_BlitSurface(img_block, &clip, screen, &offset);
		}

		//piłki
		for (vba::iterator it = balls.begin(); it != balls.end(); it++) {
				SDL_Rect offset;
				offset.x = (*it).middle().x - (*it).r();
				offset.y = (*it).middle().y - (*it).r();

				SDL_BlitSurface(img_kolo, NULL, screen, &offset);
		}
		
		//wyświetlanie bloków
		for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++) {
			Block bl = *it;
//			bl.print();

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
			
		}
		
		//wyświetlanie platformy
		SDL_Rect clip;
		clip.x = 0;
		clip.y = 0;
		clip.w = rec_width(platform.return_rectangle())+2; // to działa ale trzeba ujednolicić dokładność przy wyświetlaniu !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		clip.h = rec_height(platform.return_rectangle())+1;

		SDL_Rect offset;
		offset.x = platform.return_rectangle().upperleft.x;
		offset.y = platform.return_rectangle().bottomright.y;

		SDL_BlitSurface(img_pad, &clip, screen, &offset);


//		if (czas<SDL_GetTicks()) {
			SDL_Flip(screen);
//			czas = SDL_GetTicks() + 35;
		}
	//	}

//		SDL_Delay(1);
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
