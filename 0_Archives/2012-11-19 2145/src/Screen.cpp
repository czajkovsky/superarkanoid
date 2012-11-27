#include "Screen.h"
#include <vector>
#include <cstdio>
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
	menufont = TTF_OpenFont("res/fonts/dinprolight.otf", 36);
	titlefont = TTF_OpenFont("res/fonts/dinprolight.otf", 50);
	gamefont = TTF_OpenFont("res/fonts/dinprolight.otf", 20);
	

	img_background = SDL_DisplayFormatAlpha(IMG_Load("res/img/background.png"));

	img_ball = SDL_DisplayFormatAlpha(IMG_Load("res/img/ball.png"));
	
	white.r=white.g=white.b=255;
	cyan.r=0;cyan.g=255;cyan.b=255;
	darkblue.r=34;darkblue.g=50;darkblue.b=65;

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
	img_bonus05 = SDL_DisplayFormatAlpha(IMG_Load("res/img/bn05.png"));
}

void Screen::drawLevel(vbn bonuses, vbl blocks, Player &player, int blocksuntouched[]) {
	SDL_Rect offset, clip;

	//tło
	SDL_BlitSurface(img_background, NULL, screen, NULL);		
		
	//wyswietlania bonusow
	for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
		offset.x = (*it).return_circle().center.x - (*it).return_circle().r;
		offset.y = (*it).return_circle().center.y - (*it).return_circle().r;

		if((*it).get_type()==1) SDL_BlitSurface(img_bonus01, NULL, screen, &offset);
		else if((*it).get_type()==2)  SDL_BlitSurface(img_bonus02, NULL, screen, &offset);
		else if((*it).get_type()==3)  SDL_BlitSurface(img_bonus03, NULL, screen, &offset);
		else if((*it).get_type()==4)  SDL_BlitSurface(img_bonus04, NULL, screen, &offset);
		else if((*it).get_type()==5)  SDL_BlitSurface(img_bonus05, NULL, screen, &offset);
	}
	
	//piłki
	for (vba::iterator it = player.balls.begin(); it != player.balls.end(); it++) {
		offset.x = (*it).middle().x - (*it).r();
		offset.y = (*it).middle().y - (*it).r();

		SDL_BlitSurface(img_ball, NULL, screen, &offset);
	}
			
	//wyświetlanie bloków	
	for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++) {
		Block bl = *it;
		//bl.print();
		clip.x = 0;
		clip.y = 0;
		clip.w = rec_width(bl.return_rectangle())+6;
		clip.h = rec_height(bl.return_rectangle())+6;

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
		clip.x = 0;
		clip.y = 0;
		clip.w = 960;
		clip.h = 10;

		offset.x = 0;
		offset.y = 590;
		SDL_BlitSurface(img_block0001, &clip, screen, &offset);
	}
			
			
	//wyświetlanie platformy
	clip.x = 0;
	clip.y = 0;
	clip.w = 3;
	clip.h = rec_height(player.return_rectangle());
	
	offset.x = player.return_rectangle().upperleft.x;
	offset.y = player.return_rectangle().bottomright.y;
	
	SDL_BlitSurface(img_pad_big, &clip, screen, &offset);
	
	clip.x=3;
	clip.y=0;
	clip.w = player.get_size();
	clip.h = rec_height(player.return_rectangle());

	offset.x += 3;
	SDL_BlitSurface(img_pad_big, &clip, screen, &offset);
	
	clip.x= 270-3;
	clip.w=3;
	offset.x += player.get_size();
	SDL_BlitSurface(img_pad_big, &clip, screen, &offset);

	/*if(player.get_size()==0) SDL_BlitSurface(img_pad_norm, NULL, screen, &offset);
	else if(player.get_size()==-1) SDL_BlitSurface(img_pad_small, NULL, screen, &offset);
	else if(player.get_size()==1) SDL_BlitSurface(img_pad_big, NULL, screen, &offset);
	*/
	
	int prscore=player.getPoints();
	char printscore[9];
	printscore[8]='\0';
	for(int i=7; i>=0; i--) {
		printscore[i]=char(prscore%10+'0');
		prscore/=10;
	}
	
	int prlevel=player.getLevel();
	char printlevel[3];
	printlevel[2]='\0';
	for(int i=1; i>=0; i--) {
		printlevel[i]=char(prlevel%10+'0');
		prlevel/=10;
	}
	
	int prlives=player.getLives();
	char printlives[3];
	printlives[2]='\0';
	for(int i=1; i>=0; i--) {
		printlives[i]=char(prlives%10+'0');
		prlives/=10;
	}
	
	
	offset.x = 10; offset.y = 0;
	SDL_Surface *font_surface = TTF_RenderUTF8_Blended(gamefont,"score", cyan );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.x = 63; offset.y = 0;
	font_surface = TTF_RenderUTF8_Blended(gamefont, printscore, white);
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.x = 146; offset.y = 0;
	font_surface = TTF_RenderUTF8_Blended(gamefont,"level", cyan);
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.x = 190; offset.y = 0;
	font_surface = TTF_RenderUTF8_Blended(gamefont, printlevel, white);
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.x = 212; offset.y = 0;
	font_surface = TTF_RenderUTF8_Blended(gamefont,"lives", cyan);
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.x = 254; offset.y = 0;
	font_surface = TTF_RenderUTF8_Blended(gamefont, printlives, white);
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);	
	
	SDL_Flip(screen);
}

void Screen::drawMenu(const int &pos, int level_to_play) {
	SDL_Rect offset, clip;
	
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 97,192,231/*0,255,211*/));
	
	clip.x = 0;
	clip.y = 250 + pos*65;
	clip.w = 960;
	clip.h = 65;
	SDL_FillRect(screen, &clip, SDL_MapRGB(screen->format, 255, 255, 255));
	
	offset.x = 65; offset.y = 65;
	SDL_Surface *font_surface = TTF_RenderUTF8_Blended(titlefont,"super", white );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);	
	SDL_FreeSurface(font_surface);
	
	offset.x += 130;
	font_surface = TTF_RenderUTF8_Blended(titlefont,"arkanoid", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);	
	SDL_FreeSurface(font_surface);
	offset.x -= 130;
	
	offset.y = 256;

	font_surface = TTF_RenderUTF8_Blended(menufont,"continue campaign", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);

	font_surface = TTF_RenderUTF8_Blended(menufont,"new campaign", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);

	font_surface = TTF_RenderUTF8_Blended(menufont,"select level", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	char printlevel[7] = "< xx >";
	for(int i=3; i>=2; i--) {
		printlevel[i]=char(level_to_play%10+'0');
		level_to_play/=10;
	}
	
	offset.x += 740;
	font_surface = TTF_RenderUTF8_Blended(menufont, printlevel, darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);
	offset.x -= 740;

	font_surface = TTF_RenderUTF8_Blended(menufont,"authors and help", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);
	
	font_surface = TTF_RenderUTF8_Blended(menufont,"exit", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	SDL_Flip(screen);
}

void Screen::drawPauseMenu(const int &pause_pos) {
	SDL_Rect clip;
	clip.x = 560;
	clip.y = 0;
	clip.w = 400;
	clip.h = 640;

	SDL_FillRect(screen, &clip, SDL_MapRGB(screen->format, 97,192,231/*0, 255, 255*/));
	
	clip.y = 250 + pause_pos*65;
	clip.h = 65;
	
	SDL_FillRect(screen, &clip, SDL_MapRGB(screen->format, 255, 255, 255));
	
	SDL_Rect offset;
	offset.x = 605; offset.y = 40;
	SDL_Surface *font_surface = TTF_RenderUTF8_Blended(titlefont,"super arkanoid", white );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=45;
	SDL_FreeSurface(font_surface);
	
	font_surface = TTF_RenderUTF8_Blended(titlefont,"game paused", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	offset.y = 321;
	font_surface = TTF_RenderUTF8_Blended(menufont,"continue", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);
	
	font_surface = TTF_RenderUTF8_Blended(menufont,"controls", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);
	
	font_surface = TTF_RenderUTF8_Blended(menufont,"main menu", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset); offset.y+=65;
	SDL_FreeSurface(font_surface);
	
	font_surface = TTF_RenderUTF8_Blended(menufont,"exit", darkblue );
	SDL_BlitSurface(font_surface, NULL, screen, &offset);
	SDL_FreeSurface(font_surface);
	
	SDL_Flip(screen);
}
