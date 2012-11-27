#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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
	ios_base::sync_with_stdio(0);
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	
}

int main() {
	init();

	vbo borders;
	vbl blocks;

	rectangle rec;
	point pt;

	//platforma
	pt.x = 380;
	pt.y = 600;
	rec.upperleft = pt;
	pt.x = 580;
	pt.y = 590;
	rec.bottomright = pt;

	Player player(INF, rec);
	Screen screen;

	Level level(player, screen);

	SDL_Quit();

	return 0;
}
