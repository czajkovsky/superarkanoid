#include "Screen.h"
#include "sdl_gl.h"
#include <vector>
#include <cstdio>
#include <cmath>
using namespace std;

typedef vector<Block> vbl;
typedef vector<Ball> vba;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

void init_gl() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//kolor menu
	glClearColor(95.0/255.0,192.0/255.0,231.0/255.0,0.0);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

Screen::Screen() {	
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWSURFACE);
	
	init_gl();
	
	//wczytywanie czcionek
	menufont = TTF_OpenFont("res/fonts/dinprolight.otf", 36);
	titlefont = TTF_OpenFont("res/fonts/dinprolight.otf", 50);
	gamefont = TTF_OpenFont("res/fonts/dinprolight.otf", 20);
	
	white.r=white.g=white.b=255;
	cyan.r=0;cyan.g=255;cyan.b=255;
	darkblue.r=34;darkblue.g=50;darkblue.b=65;
	
	SDL_Surface *surface;

	//wczytywanie tła
	surface = IMG_Load("res/img2/background.png");
	img_background = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	
	//wczytywanie piłki
	surface = IMG_Load("res/img2/ball.png");
	img_ball = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	
	//wczytywanie platformy
	surface = IMG_Load("res/img2/whitepad_left.png");
	img_pad_left = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/whitepad_center.png");
	img_pad_center = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/whitepad_right.png");
	img_pad_right = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);

	//wczytywanie bloków
	surface = IMG_Load("res/img2/b_green02.png");
	img_block0202 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/b_green01.png");
	img_block0201 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/b_red01.png");
	img_block0101 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/b_yellow02.png");
	img_block0302 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/b_yellow01.png");
	img_block0301 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/cyanpad.png");
	img_block0001 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);

	//wczytywanie bonusów
	surface = IMG_Load("res/img2/bn01.png");
	img_bonus01 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/bn02.png");
	img_bonus02 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/bn03.png");
	img_bonus03 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/bn04.png");
	img_bonus04 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("res/img2/bn05.png");
	img_bonus05 = SDL_GL_MakeTexture(surface);
	SDL_FreeSurface(surface);

	//wczytywanie napisów	
	menu_texture[0] = SDL_GL_MakeTextTexture("super", titlefont, white, menu_texture_offset[0]);
	menu_texture[1] = SDL_GL_MakeTextTexture("arkanoid", titlefont, darkblue, menu_texture_offset[1]);
	menu_texture[2] = SDL_GL_MakeTextTexture("continue campaign", menufont, darkblue, menu_texture_offset[2]);
	menu_texture[3] = SDL_GL_MakeTextTexture("new campaign", menufont, darkblue, menu_texture_offset[3]);
	menu_texture[4] = SDL_GL_MakeTextTexture("select level", menufont, darkblue, menu_texture_offset[4]);
	menu_texture[5] = SDL_GL_MakeTextTexture("authors and help", menufont, darkblue, menu_texture_offset[5]);
	menu_texture[6] = SDL_GL_MakeTextTexture("exit", menufont, darkblue, menu_texture_offset[6]);
	
	pause_menu_texture[0] = SDL_GL_MakeTextTexture("super arkanoid", titlefont, white, pause_menu_texture_offset[0]);
	pause_menu_texture[1] = SDL_GL_MakeTextTexture("game paused", titlefont, darkblue, pause_menu_texture_offset[1]);
	pause_menu_texture[2] = SDL_GL_MakeTextTexture("continue", menufont, darkblue, pause_menu_texture_offset[2]);
	pause_menu_texture[3] = SDL_GL_MakeTextTexture("controle", menufont, darkblue, pause_menu_texture_offset[3]);
	pause_menu_texture[4] = SDL_GL_MakeTextTexture("main menu", menufont, darkblue, pause_menu_texture_offset[4]);
	pause_menu_texture[5] = SDL_GL_MakeTextTexture("exit", menufont, darkblue, pause_menu_texture_offset[5]);
	
	level_texture[0] = SDL_GL_MakeTextTexture("score", gamefont, cyan, level_texture_offset[0]);
	level_texture[1] = SDL_GL_MakeTextTexture("level", gamefont, cyan, level_texture_offset[1]);
	level_texture[2] = SDL_GL_MakeTextTexture("lives", gamefont, cyan, level_texture_offset[2]);
	
	//wczytywanie liczb
	for (int i=0; i<10; i++) {
		char tmp[2] = {(char)(i+'0'),'\0'};
		number_texture[i] = SDL_GL_MakeTextTexture(tmp, gamefont, white, number_texture_offset[i]);
	}
	
	for (int i=0; i<10; i++) {
		char tmp[2] = {(char)(i+'0'),'\0'};
		menu_number_texture[i] = SDL_GL_MakeTextTexture(tmp, menufont, darkblue, menu_number_texture_offset[i]);
	}	
}

Screen::~Screen() {
	TTF_CloseFont(menufont);
	TTF_CloseFont(titlefont);
	TTF_CloseFont(gamefont);
	SDL_FreeSurface(screen);
}

void Screen::drawLevel(vbn bonuses, vbl blocks, Player *player, int blocksuntouched[]) {
	glClear(GL_COLOR_BUFFER_BIT);
    
	glEnable(GL_TEXTURE_2D);
	
	SDL_Rect location;
	location.x = location.y = 0;
	location.w = location.h = 1024;
	
	//renderowanie tła
	SDL_GL_RenderTexture(img_background, &location);		
		
	//renderowanie bonusów
	for (vbn::iterator it = bonuses.begin(); it != bonuses.end(); it++) {
		location.x = (*it).getCenter().x - (*it).getR();
		location.y = (*it).getCenter().y - (*it).getR();
		location.h=location.w=32;

		if((*it).get_type()==1) SDL_GL_RenderTexture(img_bonus01, &location);
		else if((*it).get_type()==2)  SDL_GL_RenderTexture(img_bonus02, &location);
		else if((*it).get_type()==3)  SDL_GL_RenderTexture(img_bonus03, &location);
		else if((*it).get_type()==4)  SDL_GL_RenderTexture(img_bonus04, &location);
		else if((*it).get_type()==5)  SDL_GL_RenderTexture(img_bonus05, &location);		
	}	
	
	//renderowanie piłek
	glBindTexture(GL_TEXTURE_2D, img_ball);
	for (vba::iterator it = player->balls.begin(); it != player->balls.end(); it++) {
		location.x = (*it).getCenter().x - (*it).getR();
		location.y = (*it).getCenter().y - (*it).getR();		
		location.h=location.w=32;

		SDL_GL_RenderTexture(img_ball, &location);	
	}
			
	//renderowanie bloków	
	for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++) {
		location.x = (*it).getUpperLeft().x-3;
		location.y = (*it).getBottomRight().y-3;		
		location.h=location.w=128;

		if((*it).get_type()==1) SDL_GL_RenderTexture(img_block0101, &location);
		else if((*it).get_type()==2) {
			if((*it).get_lives()==1) SDL_GL_RenderTexture(img_block0201, &location);
			else if((*it).get_lives()==2) SDL_GL_RenderTexture(img_block0202, &location);
		}
		else if((*it).get_type()==3) {
			if((*it).get_lives()==(*it).get_maxlives()) SDL_GL_RenderTexture(img_block0302, &location);
			else SDL_GL_RenderTexture(img_block0301, &location);
		}
	}
	
	//renderowanie dolnego blocka bonusowego
	if(blocksuntouched[0]) {
		location.x = 0;
		location.y = 590;
		location.w=location.h=1024;
		SDL_GL_RenderTexture(img_block0001, &location);
	}			
			
	//renderowanie platformy
	location.x = player->platform->getUpperLeft().x;
	location.y = player->platform->getBottomRight().y;
	location.h=location.w=16;
	SDL_GL_RenderTexture(img_pad_left, &location);
	
	location.x += 3;
	location.w = player->platform->get_size();
	glBindTexture(GL_TEXTURE_2D, img_pad_center);
	SDL_GL_RenderTexture(img_pad_center, &location);
		
	location.x += player->platform->get_size();
	location.w = 16;
	SDL_GL_RenderTexture(img_pad_right, &location);
	
	//renderowanie napisów
	int prscore=player->getPoints();
	char printscore[9];
	printscore[8]='\0';
	for(int i=7; i>=0; i--) {
		printscore[i]=char(prscore%10+'0');
		prscore/=10;
	}
	
	int prlevel=player->getLevel();
	char printlevel[3];
	printlevel[2]='\0';
	for(int i=1; i>=0; i--) {
		printlevel[i]=char(prlevel%10+'0');
		prlevel/=10;
	}
	
	int prlives=player->getLives();
	char printlives[3];
	printlives[2]='\0';
	for(int i=1; i>=0; i--) {
		printlives[i]=char(prlives%10+'0');
		prlives/=10;
	}	
	
	//renderowanie "score"
	level_texture_offset[0].x = 10; level_texture_offset[0].y = 0;
	SDL_GL_RenderTexture(level_texture[0], &level_texture_offset[0]);
	
	for (int i=0; i<8; i++) {
		int k = (int)(printscore[i]-'0');
		number_texture_offset[k].x = 63+i*10; number_texture_offset[k].y = 0;
		SDL_GL_RenderTexture(number_texture[k], &number_texture_offset[k]);
	}
	
	//renderowanie "level"
	level_texture_offset[1].x = 146; level_texture_offset[1].y = 0;
	SDL_GL_RenderTexture(level_texture[1], &level_texture_offset[1]);
	
	for (int i=0; i<2; i++) {
		int k = (int)(printlevel[i]-'0');
		number_texture_offset[k].x = 190+i*10; number_texture_offset[k].y = 0;
		SDL_GL_RenderTexture(number_texture[k], &number_texture_offset[k]);
	}

	//renderowanie "lives"
	level_texture_offset[2].x = 212; level_texture_offset[2].y = 0;
	SDL_GL_RenderTexture(level_texture[2], &level_texture_offset[2]);

	for (int i=0; i<2; i++) {
		int k = (int)(printlives[i]-'0');
		number_texture_offset[k].x = 254+i*10; number_texture_offset[k].y = 0;
		SDL_GL_RenderTexture(number_texture[k], &number_texture_offset[k]);
	}
	
	SDL_GL_SwapBuffers();
}

void Screen::drawMenu(const int &pos, int level_to_play) {
	SDL_Rect location;
	
	glClear( GL_COLOR_BUFFER_BIT );	
	
	glDisable( GL_TEXTURE_2D );
	
	//renderowanie podświetlenia na biało	
	location.x = 0;
	location.y = 250 + pos*65;
	location.w = 960;
	location.h = 65;
	glColor3f(1,1,1);
	glBegin( GL_QUADS );
		// Top-left vertex (corner)
		glTexCoord2i(0, 0);
		glVertex3f( location.x, location.y, 0);
		// Bottom-left vertex (corner)
		glTexCoord2i(1, 0);
		glVertex3f(location.x+location.w, location.y, 0);
		// Bottom-right vertex (corner)
		glTexCoord2i( 1, 1 );
		glVertex3f(location.x+location.w, location.y+location.h, 0);
		// Top-right vertex (corner)
		glTexCoord2i(0, 1);
		glVertex3f(location.x, location.y+location.h, 0);
	glEnd();	
	
	//renderowanie "super"
	location.x = 65; location.y = 65;
	menu_texture_offset[0].x = location.x; menu_texture_offset[0].y = location.y;
	SDL_GL_RenderTexture(menu_texture[0], &menu_texture_offset[0]);

	//renderowanie "arkanoid"
	location.x += 130;
	menu_texture_offset[1].x = location.x; menu_texture_offset[1].y = location.y;
	SDL_GL_RenderTexture(menu_texture[1], &menu_texture_offset[1]);

	//renderowanie "continue campaign"
	location.x -= 130;	
	location.y = 256;
	menu_texture_offset[2].x = location.x; menu_texture_offset[2].y = location.y;
	SDL_GL_RenderTexture(menu_texture[2], &menu_texture_offset[2]);
	
	//renderowanie "new campaign"
	location.y += 65;
	menu_texture_offset[3].x = location.x; menu_texture_offset[3].y = location.y;
	SDL_GL_RenderTexture(menu_texture[3], &menu_texture_offset[3]);
	
	//renderowanie "select level"
	location.y += 65;
	menu_texture_offset[4].x = location.x; menu_texture_offset[4].y = location.y;
	SDL_GL_RenderTexture(menu_texture[4], &menu_texture_offset[4]);

	//renderowanie numeru poziomu
	char printlevel[7] = "< xx >";
	for(int i=3; i>=2; i--) {
		printlevel[i]=char(level_to_play%10+'0');
		level_to_play/=10;
	}
	
	location.x += 740;
	for (int i=2; i<4; i++) {
		int k = (int)(printlevel[i]-'0');
		menu_number_texture_offset[k].x = location.x+i*18; menu_number_texture_offset[k].y = location.y;
		SDL_GL_RenderTexture(menu_number_texture[k], &menu_number_texture_offset[k]);
	}
	
	//renderowanie "authors and help"
	location.y += 65;
	location.x -= 740;
	menu_texture_offset[5].x = location.x; menu_texture_offset[5].y = location.y;
	SDL_GL_RenderTexture(menu_texture[5], &menu_texture_offset[5]);

	//renderowanie "exit"
	location.y += 65;
	menu_texture_offset[6].x = location.x; menu_texture_offset[6].y = location.y;
	SDL_GL_RenderTexture(menu_texture[6], &menu_texture_offset[6]);

	SDL_GL_SwapBuffers();
}

void Screen::drawPauseMenu(const int &pause_pos) {
	glDisable( GL_TEXTURE_2D );

	SDL_Rect location;	

	//renderowanie tła menu
	location.x = 560;
	location.y = 0;
	location.w = 400;
	location.h = 640;
	glColor3f(95.0/255.0,192.0/255.0,231.0/255.0);
	glBegin( GL_QUADS );
		// Top-left vertex (corner)
		glTexCoord2i(0, 0);
		glVertex3f(location.x, location.y, 0);
		// Bottom-left vertex (corner)
		glTexCoord2i(1, 0);
		glVertex3f(location.x+location.w, location.y, 0);
		// Bottom-right vertex (corner)
		glTexCoord2i(1, 1);
		glVertex3f(location.x+location.w, location.y+location.h, 0);
		// Top-right vertex (corner)
		glTexCoord2i(0, 1);
		glVertex3f(location.x, location.y+location.h, 0);
	glEnd();
	
	//renderowanie podświetlenia na biało
	location.y = 250 + pause_pos*65;
	location.h = 65;
	glColor3f(1,1,1);
	glBegin( GL_QUADS );
		// Top-left vertex (corner)
		glTexCoord2i(0, 0);
		glVertex3f(location.x, location.y, 0);
		// Bottom-left vertex (corner)
		glTexCoord2i(1, 0);
		glVertex3f(location.x+location.w, location.y, 0);
		// Bottom-right vertex (corner)
		glTexCoord2i(1, 1);
		glVertex3f(location.x+location.w, location.y+location.h, 0);
		// Top-right vertex (corner)
		glTexCoord2i(0, 1);
		glVertex3f(location.x, location.y+location.h, 0);
	glEnd();
	
	//renderowanie "super arkanoid"
	location.x = 605; location.y = 40;
	pause_menu_texture_offset[0].x = location.x; pause_menu_texture_offset[0].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[0], &pause_menu_texture_offset[0]);
	
	//renderowanie "game paused"
	location.y+=45;
	pause_menu_texture_offset[1].x = location.x; pause_menu_texture_offset[1].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[1], &pause_menu_texture_offset[1]);
	
	//renderowanie "continue"	
	location.y = 321;
	pause_menu_texture_offset[2].x = location.x; pause_menu_texture_offset[2].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[2], &pause_menu_texture_offset[2]);
	
	//renderowanie "controls"
	location.y+=65;
	pause_menu_texture_offset[3].x = location.x; pause_menu_texture_offset[3].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[3], &pause_menu_texture_offset[3]);
	location.y+=65;
	
	//renderowanie "main menu"
	pause_menu_texture_offset[4].x = location.x; pause_menu_texture_offset[4].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[4], &pause_menu_texture_offset[4]);
	location.y+=65;
	
	//renderowanie "exit"
	pause_menu_texture_offset[5].x = location.x; pause_menu_texture_offset[5].y = location.y;
	SDL_GL_RenderTexture(pause_menu_texture[5], &pause_menu_texture_offset[5]);
	
	SDL_GL_SwapBuffers();
}
