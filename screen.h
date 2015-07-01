#ifndef _SCREEN_H
#define _SCREEN_H

#include <SDL2/SDL.h>
#include "cpu.h"

typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* font;
	int closerequested;
} Screen;

Screen* screen_init(
	int window_w, int window_h, int screen_w, int screen_h, 
	int char_w, int char_h, int font_w, int font_h,
	int fullscreen);

void screen_update(Screen* s, CPU* c);
void screen_cleanup(Screen* s);

void screen_pollevents(Screen* s, CPU* c);
int screen_closerequested(Screen* s);

#endif /* _SCREEN_H */
