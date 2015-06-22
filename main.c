#include <stdio.h>
#include <string.h>
#include "cpu.h"

int main(int argc, char** argv)
{
	int stepping = 0;
	int i;

	if(argc > 2) {
		for(i = 2; i < argc; i++) {
			if(strcmp(argv[i], "-s") == 0) {
				stepping = 1;
			}
		}
	}

	if(argc >= 2) {
	}else {
		printf("Usage: ccpu <binary> [options]\n\nOptions:\n\t-s - Stepping mode\n");
		return -1;
	}

	FILE* f = fopen(argv[1], "rb");
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

	CPU* c = cpu_init(0x200);

	for(i = 0; i < length; i++) {
		c->mem[c->ip+i] = buffer[i];
	}
	
	while(1) {
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
					return 0;
					break;
				case 'd':
				case 'D':
					f = fopen("memdump.bin", "wb");
					fwrite(c->mem, sizeof(u8), 0x10000, f);
					fclose(f);
					goto get_input;
			}
		}
		if(c->hlt) break;
	
		cpu_cycle(c);
	}

	return 0;
}
