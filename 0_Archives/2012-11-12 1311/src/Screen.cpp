#include "Screen.h"
#include <vector>
using namespace std;

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

#define WIDTH 960
#define HEIGHT 640

Screen::Screen() {
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	img_background = SDL_DisplayFormatAlpha(IMG_Load("res/img/background.png"));

	img_ball = SDL_DisplayFormatAlpha(IMG_Load("res/img/ball.png"));

	img_pad_norm = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_norm.png"));
	img_pad_big = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_big.png"));
	img_pad_small = SDL_DisplayFormatAlpha(IMG_Load("res/img/whitepad_small.png"));

	img_block0202 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green02.png"));
	img_block0201 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_green01.png"));
	img_block0101 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_red01.png"));
	img_block0302 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_yellow02.png"));
	img_block0301 = SDL_DisplayFormatAlpha(IMG_Load("res/img/b_yellow01.png"));
	img_block0001 = SDL_DisplayFormatAlpha(IMG_Load("res/img/cyanpad.png"));

	img_bonus01 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn01.png"));
	img_bonus02 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn02.png"));
	img_bonus03 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn03.png"));
	img_bonus04 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn04.png"));
}

void Screen::drawLevel(vbn bonuses, vbl blocks, Player player, int blocksuntouched[]) {
	//tło
	SDL_BlitSurface(img_background, NULL, screen, NULL);		
		
	//wyswietlania bonusow
	for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
		SDL_Rect offset;
		offset.x = (*it).return_circle().center.x - (*it).return_circle().r;
		offset.y = (*it).return_circle().center.y - (*it).return_circle().r;

		if((*it).get_type()==1) SDL_BlitSurface(img_bonus01, NULL, screen, &offset);
		else if((*it).get_type()==2)  SDL_BlitSurface(img_bonus02, NULL, screen, &offset);
		else if((*it).get_type()==3)  SDL_BlitSurface(img_bonus03, NULL, screen, &offset);
		else if((*it).get_type()==4)  SDL_BlitSurface(img_bonus04, NULL, screen, &offset);				
	}
	
	//piłki
	for (vba::iterator it = player.balls.begin(); it != player.balls.end(); it++) {
		SDL_Rect offset;
		offset.x = (*it).middle().x - (*it).r();
		offset.y = (*it).middle().y - (*it).r();

		SDL_BlitSurface(img_ball, NULL, screen, &offset);
	}
			
	//wyświetlanie bloków
	
	for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++) {
		Block bl = *it;
		//bl.print();
			SDL_Rect clip;
			clip.x = 0;
		clip.y = 0;
		clip.w = rec_width(bl.return_rectangle())+6;
		clip.h = rec_height(bl.return_rectangle())+6;

		SDL_Rect offset;
		offset.x = bl.return_rectangle().upperleft.x-3;
		offset.y = bl.return_rectangle().bottomright.y-3;

		if(bl.get_type()==1) SDL_BlitSurface(img_block0101, &clip, screen, &offset);
		else if(bl.get_type()==2) {
			if(bl.get_lives()==1) SDL_BlitSurface(img_block0201, &clip, screen, &offset);
			else if(bl.get_lives()==2) SDL_BlitSurface(img_block0202, &clip, screen, &offset);
		}
		else if(bl.get_type()==3) {
			if(bl.get_lives()==bl.get_maxlives()) SDL_BlitSurface(img_block0302, &clip, screen, &offset);
			else SDL_BlitSurface(img_block0301, &clip, screen, &offset);
		}	
	}
	
	//wyświetlanie dolnego blocka bonusowego
	if(blocksuntouched[0]) {
		SDL_Rect clip;
		clip.x = 0;
		clip.y = 0;
		clip.w = 960;
		clip.h = 10;

		SDL_Rect offset;
		offset.x = 0;
		offset.y = 590;
		SDL_BlitSurface(img_block0001, &clip, screen, &offset);
	}
			
			
	//wyświetlanie platformy
	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	clip.w = rec_width(player.return_rectangle())+2; // to działa ale trzeba ujednolicić dokładność przy wyświetlaniu!
	clip.h = rec_height(player.return_rectangle())+1;

	SDL_Rect offset;
	offset.x = player.return_rectangle().upperleft.x;
	offset.y = player.return_rectangle().bottomright.y;

	if(player.get_size()==0) SDL_BlitSurface(img_pad_norm, &clip, screen, &offset);
	else if(player.get_size()==-1) SDL_BlitSurface(img_pad_small, &clip, screen, &offset);
	else if(player.get_size()==1) SDL_BlitSurface(img_pad_big, &clip, screen, &offset);
	
	
	
	SDL_Flip(screen);
}

void Screen::drawMenu(int pos) {
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 255, 255));
	
	SDL_Flip(screen);
}
