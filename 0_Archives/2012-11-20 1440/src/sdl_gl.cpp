#include "sdl_gl.h"
#include <cmath>

GLuint SDL_GL_MakeTexture(SDL_Surface *surface) {
	GLuint tex;
	
	//sprawdzanie czy boki są potęgami 2
	if ((surface->w & (surface->w - 1)) != 0) {
		printf("width is not a power of 2\n");
	}
	if ((surface->h & (surface->h - 1)) != 0) {
		printf("height is not a power of 2\n");
	}
   
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	
	return tex;
}

int nextpoweroftwo(int x) {
	double logbase2 = log(x) / log(2);
	return round(pow(2,ceil(logbase2)));
}

void SDL_GL_RenderTexture(const GLuint &texture, SDL_Rect *location) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		// Top-left vertex (corner)
		glTexCoord2i(0, 0);
		glVertex3f(location->x, location->y, 0);
		// Bottom-left vertex (corner)
		glTexCoord2i(1, 0);
		glVertex3f(location->x+location->w, location->y, 0);
		// Bottom-right vertex (corner)
		glTexCoord2i(1, 1);
		glVertex3f(location->x+location->w, location->y+location->h, 0);
		// Top-right vertex (corner)
		glTexCoord2i(0, 1);
		glVertex3f(location->x, location->y+location->h, 0);
	glEnd();
}

void SDL_GL_RenderText(const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location) {
	glLoadIdentity();
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	GLuint font_texture;
	
	initial = TTF_RenderText_Blended(font, text, color);
	SDL_SetAlpha(initial, 0, 0);
	
	location->w = nextpoweroftwo(initial->w);
	location->h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(SDL_SWSURFACE, location->w, location->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	glGenTextures(1, &font_texture);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, location->w, location->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, intermediary->pixels );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glColor3f(1,1,1);
	
	glBegin(GL_QUADS);
		// Top-left vertex (corner)
		glTexCoord2d(0, 0);
		glVertex3f(location->x, location->y, 0);
		// Bottom-left vertex (corner)
		glTexCoord2d(1, 0);
		glVertex3f(location->x + intermediary->w, location->y, 0);
		// Bottom-right vertex (corner)
		glTexCoord2d(1, 1);
		glVertex3f(location->x + intermediary->w, location->y + intermediary->h, 0);
		// Top-right vertex (corner)
		glTexCoord2d(0, 1);
		glVertex3f(location->x, location->y + intermediary->h, 0); 
	glEnd();

	glFinish();
	
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &font_texture);
}

GLuint SDL_GL_MakeTextTexture(const char *text, TTF_Font *font, SDL_Color color, SDL_Rect &offset) {
	GLuint texture;
	glGenTextures(1, &texture);
	glLoadIdentity();
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	
	initial = TTF_RenderText_Blended(font, text, color);
	SDL_SetAlpha(initial, 0, 0);
	
	offset.w = nextpoweroftwo(initial->w);
	offset.h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(SDL_SWSURFACE, offset.w, offset.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, offset.w, offset.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, intermediary->pixels );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	return texture;
}
