#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "screen.h"

CPU* c = NULL;
int stepping = 0;
int cpu_running = 1;

int cpu_thread_func(void* args)
{
	FILE* f;	

	while(cpu_running) {
		if(stepping) {			
			printf("IP: 0x%04X, ADDRESS: 0x%04X\n", c->ip, c->address);
			printf("ACC: 0x%02X, X: 0x%02X, Y: 0x%02X\n", c->acc, c->x, c->y);
			printf("HLT: %s\n", c->hlt ? "true" : "false");
			
get_input:
			printf(": ");
			char in = getchar();
			switch (in) {
				case 'x':
				case 'X':
				case 'q':
				case 'Q':
					cpu_running = 0;
					return 0;
					break;
				case 'd':
				case 'D':
					f = fopen("memdump.bin", "wb");
					fwrite(c->mem, sizeof(u8), 0x10000, f);
					fclose(f);
					goto get_input;
					break;
			}
		}
		if(c->hlt) { 
			cpu_running = 0;
			return 0;
		}		

		cpu_cycle(c);
	}


	return 0;
}

int main(int argc, char** argv)
{
	int i;
	int window = 1;
	int showhelp = 0;
	int fps = 0;

	if(argc > 2) {
		for(i = 2; i < argc; i++) {
			if(strcmp(argv[i], "-s") == 0) {
				stepping = 1;
			}else if(strcmp(argv[i], "-noscreen") == 0) {
				window = 0;
			}else if(strcmp(argv[i], "-h") == 0) {
				showhelp = 1;
			}else if(strcmp(argv[i], "-fps") == 0) {
				fps = 1;
			}
			else {
				printf("Ignoring unknown argument '%s'. Use '-h' for more information\n", argv[i]);
			}
		}
	}

	if(argc < 2 | showhelp) {
		printf("Usage: ccpu <binary> [options]\n\nOptions:\n\t-s - Stepping mode\n\t-noscreen - No screen/window\n\t-fps - Output fps\n");
		return -1;
	}

	FILE* f = fopen(argv[1], "rb");
	if(f == NULL) {
		printf("Failed to open '%s'\n", argv[1]);
		return -1;
	}
	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	rewind(f);
	u8* buffer = (u8*)malloc(sizeof(u8)*length);
	int read = fread(buffer, sizeof(u8), length, f);
	fclose(f);

	if(read != length) {
		printf("Failed to read file '%s'\n", argv[1]);
		return -1;
	}

	f = fopen("rom.bin", "rb");
	if(f == NULL) {
		printf("Could not open 'rom.bin'\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	int rom_length = ftell(f);
	if(rom_length > 0x100) {
		printf("'rom.bin' is %04X bytes long, but can not be longer than 0x100 bytes\n");
		return -1;
	}
	rewind(f);
	u8* rom_buffer = (u8*)malloc(sizeof(u8)*rom_length);
	read = fread(rom_buffer, sizeof(u8), rom_length, f);
	if(read != rom_length) {
		printf("Failed to read 'rom.bin'\n");
		return -1;
	}
	fclose(f);

	c = cpu_init(0x1000);

	for(i = 0; i < length; i++) {
		c->mem[c->ip+i] = buffer[i];
	}

	for(i = 0; i < rom_length; i++) {
		c->mem[0xF000+i] = rom_buffer[i];
	}

	c->ip = 0xF000;

	SDL_Thread* ccpu_thread = SDL_CreateThread(cpu_thread_func, "ccpu_thread", NULL);
	
	if(window) {
	Screen* s = screen_init(640, 360, 640, 360, 8, 12, 16, 16);

	Uint32 now = SDL_GetTicks();
	Uint32 lastTime = now;
	Uint32 fpsCounterTime = now;
	Uint32 interval = 16;

	int frames = 0;

	while(!screen_closerequested(s)) {
		now = SDL_GetTicks();
		
		if(now - lastTime > interval) {
			screen_update(s, c);
			frames++;
			lastTime = now;	
		}

		if(now - fpsCounterTime > 1000) {
			if(fps) printf("fps %d\n", frames);
			frames = 0;
			fpsCounterTime = now;
		}

		screen_pollevents(s, c);
		if(!cpu_running) break;
	}
	screen_cleanup(s);
	SDL_DetachThread(ccpu_thread);
	}else {
		SDL_WaitThread(ccpu_thread, NULL);
	}
	
	return 0;
}
