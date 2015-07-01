#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	Uint8 r, g, b;
} Screen_Color;

static Screen_Color colors[16];
static int screen_w;
static int screen_h;
static int char_w;
static int char_h;
static int font_w;
static int font_h;

Screen* screen_init(int win_w, int win_h, int scr_w, int scr_h, int ch_w, int ch_h, int fnt_w, int fnt_h, int fullscreen)
{
	screen_w = scr_w;
	screen_h = scr_h;
	char_w = ch_w;
	char_h = ch_h;
	font_w = fnt_w;
	font_h = fnt_h;

	Screen* s = (Screen*)malloc(sizeof(Screen));
	
	SDL_Init(SDL_INIT_VIDEO);

	s->window = SDL_CreateWindow("ccpu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
		 fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP  : SDL_WINDOW_RESIZABLE);
	if(s->window == NULL) {
		printf("Failed to create window!\n");
		exit(-1);
	}

	s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
	if(s->renderer == NULL) {
		printf("Failed to create renderer!\n");
		exit(-1);
	}
	SDL_RenderSetLogicalSize(s->renderer, screen_w, screen_h);

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

static int blinky = 0;
void screen_update(Screen* s, CPU* c)
{
	int i;
	int mem = 0;
	SDL_Rect fr;
	fr.w = char_w;
	fr.h = char_h;
	SDL_Rect sr;
	sr.x = 0;
	sr.y = 0;
	sr.w = char_w;
	sr.h = char_h;
	
	SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0);
	SDL_RenderClear(s->renderer);	

	int x = 0;
	int y = 0;
	for(i = 0; i < 80*30; i++) {
		u8 ch = c->mem[VIDEO_START+(mem++)];
		u8 col = c->mem[VIDEO_START+(mem++)];
		if(col == 0) {
			x++;
			if(x >= 80) {
				x = 0;
				y++;
			}
			continue;
		}		

		u8 ch_x = (ch % font_w) * char_w;
		u8 ch_y = (ch / font_h) * char_h;

		fr.x = ch_x;
		fr.y = ch_y;
		
		sr.x = x * char_w;
		sr.y = y * char_h;
		
		u8 bg = (col & 0xF0) >> 4;
		if(bg != 0) {
			SDL_SetRenderDrawColor(s->renderer, colors[bg].r, colors[bg].g, colors[bg].b, 255);
			SDL_RenderFillRect(s->renderer, &sr);
		}
		u8 fg = (col & 0xF);
		SDL_SetTextureColorMod(s->font, colors[fg].r, colors[fg].g, colors[fg].b);
		SDL_RenderCopy(s->renderer, s->font, &fr, &sr);

		x++;
		if(x >= 80) {
			x = 0;
			y++;
		}
	}
	
	u8 fg = c->mem[SCREEN_COLOR] & 0x0F;
	if(c->mem[SCREEN_BLINK_INTERVAL] == 0){
		
	}else if(c->mem[SCREEN_BLINK_INTERVAL] == 1) {
		SDL_SetRenderDrawColor(s->renderer, colors[fg].r, colors[fg].g, colors[fg].b, 255);
		SDL_RenderFillRect(s->renderer,&(SDL_Rect){c->mem[SCREEN_CURSOR_X]*char_w, c->mem[SCREEN_CURSOR_Y]*char_h+(char_h-2), char_w, 2});
	}else if(blinky >= c->mem[SCREEN_BLINK_INTERVAL]) {
		blinky++;
		if(blinky >= c->mem[SCREEN_BLINK_INTERVAL]*2) blinky = 0;
	}else {
		SDL_SetRenderDrawColor(s->renderer, colors[fg].r, colors[fg].g, colors[fg].b, 255);
		SDL_RenderFillRect(s->renderer,&(SDL_Rect){c->mem[SCREEN_CURSOR_X]*char_w, c->mem[SCREEN_CURSOR_Y]*char_h+(char_h-2), char_w, 2});
		blinky++;
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
