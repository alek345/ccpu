#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	Uint8 r, g, b;
} Screen_Color;

static Screen_Color colors[16];

Screen* screen_init()
{
	Screen* s = (Screen*)malloc(sizeof(Screen));
	
	SDL_Init(SDL_INIT_VIDEO);

	s->window = SDL_CreateWindow("ccpu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_RESIZABLE);
	if(s->window == NULL) {
		printf("Failed to create window!\n");
		exit(-1);
	}

	s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
	if(s->renderer == NULL) {
		printf("Failed to create renderer!\n");
		exit(-1);
	}
	SDL_RenderSetLogicalSize(s->renderer, 640, 360);

	SDL_Surface* img = SDL_LoadBMP("font.bmp");
	if(img == NULL) {
		printf("Failed to load 'font.bmp'\n");
		exit(-1);
	}
	SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 255, 0, 255));

	s->font = SDL_CreateTextureFromSurface(s->renderer, img);
	SDL_FreeSurface(img);
	if(s->font == NULL) {
		printf("Failed to create texture!\n");
		exit(-1);
	}

	s->closerequested = 0;

	colors[0] = (Screen_Color){0, 0, 0};
	colors[1] = (Screen_Color){0, 0, 0xC0};
	colors[2] = (Screen_Color){0, 0xC0, 0};
	colors[3] = (Screen_Color){0, 0xC0, 0xC0};
	colors[4] = (Screen_Color){0xC0, 0, 0};
	colors[5] = (Screen_Color){0xC0, 0, 0xC0};
	colors[6] = (Screen_Color){0xC0, 0x80, 0};
	colors[7] = (Screen_Color){0xC0, 0xC0, 0xC0};
	colors[8] = (Screen_Color){0x80, 0x80, 0x80};
	colors[9] = (Screen_Color){0, 0, 0xFF};
	colors[10] = (Screen_Color){0, 0xFF, 0};
	colors[11] = (Screen_Color){0, 0xFF, 0xFF};
	colors[12] = (Screen_Color){0xFF, 0, 0};
	colors[13] = (Screen_Color){0xFF, 0, 0xFF};
	colors[14] = (Screen_Color){0xFF, 0xFF, 0};
	colors[15] = (Screen_Color){0xFF, 0xFF, 0xFF};

	return s;
}

void screen_update(Screen* s, CPU* c)
{
	int i;
	int mem = 0;
	SDL_Rect fr;
	fr.w = 8;
	fr.h = 12;
	SDL_Rect sr;
	sr.x = 0;
	sr.y = 0;
	sr.w = 8;
	sr.h = 12;
	
	SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0);
	SDL_RenderClear(s->renderer);	

	int x = 0;
	int y = 0;
	int blinky = 0;
	for(i = 0; i < 80*30; i++) {
		u8 ch = c->mem[0x8000+(mem++)];
		u8 col = c->mem[0x8000+(mem++)];
		if(col == 0) {
			continue;
		}		

		u8 ch_x = (ch % 16) * 8;
		u8 ch_y = (ch / 16) * 12;

		fr.x = ch_x;
		fr.y = ch_y;
		
		if(x >= 80) {
			x = 0;
			y++;
		}
		sr.x = x * 8;
		sr.y = y * 12;
		
		u8 bg = (col & 0xF0) >> 4;
		if(bg != 0) {
			SDL_SetRenderDrawColor(s->renderer, colors[bg].r, colors[bg].g, colors[bg].b, 255);
			SDL_RenderFillRect(s->renderer, &sr);
		}
		u8 fg = (col & 0xF);
		SDL_SetTextureColorMod(s->font, colors[fg].r, colors[fg].g, colors[fg].b);
		SDL_RenderCopy(s->renderer, s->font, &fr, &sr);
		
		if(blinky >= 60) {
			blinky = 0;
		}else if(blinky >= 30) {
			blinky++;
		}else {
			SDL_RenderFillRect(s->renderer, &(SDL_Rect){c->mem[0x24]*8, c->mem[0x25]*12+10, 8, 2});
			blinky++;
		}

		x++;
	}
	SDL_RenderPresent(s->renderer);
}

void screen_cleanup(Screen* s)
{
	SDL_DestroyTexture(s->font);
	SDL_DestroyRenderer(s->renderer);
	SDL_DestroyWindow(s->window);
	SDL_Quit();
}

void screen_pollevents(Screen* s, CPU* c)
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_QUIT:
				s->closerequested = 1;
				break;
			case SDL_KEYUP:
				cpu_add_key_to_buffer(c, e.key.keysym.sym);
				break;
		}
	}
}

int screen_closerequested(Screen* s)
{
	return s->closerequested;
}
