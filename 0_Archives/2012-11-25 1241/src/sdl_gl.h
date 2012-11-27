#ifndef SDL_GL
#define SDL_GL

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>

GLuint SDL_GL_MakeTexture(SDL_Surface *surface);
void SDL_GL_RenderTexture(const GLuint &texture, SDL_Rect *location);
void SDL_GL_RenderText(const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location);
GLuint SDL_GL_MakeTextTexture(const char *text, TTF_Font *font, SDL_Color color, SDL_Rect &offset);

#endif
