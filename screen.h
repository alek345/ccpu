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

Screen* screen_init();

void screen_update(Screen* s, CPU* c);

void screen_pollevents(Screen* s);
int screen_closerequested(Screen* s);

#endif /* _SCREEN_H */
