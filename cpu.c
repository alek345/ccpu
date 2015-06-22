#include "cpu.h"

CPU* cpu_init(u16 start_addr)
{
	CPU* c = (CPU*) malloc(sizeof(CPU));
	c->mem = (u8*) malloc(sizeof(u8) * 0x10000);
	c->acc = 0;
	c->address = 0;
	c->x = 0;
	c->y = 0;
	c->ip = start_addr;	
	c->hlt = 0;
	
	return c;
}

void cpu_cycle(CPU* c)
{
	u8 op = c->mem[c->ip++];
	if(op == NOP) return;
	
	switch(op) {
		case HLT:
			c->hlt = 1;
			break;
		case LDAX:
			c->acc = c->x;
			break;
		case LDAY:
			c->acc = c->y;
			break;
		case LDXY:
			c->x = c->y;
			break;
		case LDXA:
			c->x = c->acc;
			break;
		case LDYX:
			c->y = c->x;
			break;
		case LDYA:
			c->y = c->acc;
			break;
		case LDHA:
			c->address |= c->mem[c->ip++] << 8;
			break;
		case LDLA:
			c->address |= c->mem[c->ip++];
			break;
		case LDXM:
			c->x = c->mem[c->address];
			break;
		case LDYM:
			c->y = c->mem[c->address];
			break;
		case LDAM:
			c->acc = c->mem[c->address];
			break;
		case INCA:
			c->acc++;
			break;
		case INCX:
			c->x++;
			break;
		case INCY:
			c->y++;
			break;
		case DECA:
			c->acc--;
			break;
		case DECX:
			c->x--;
			break;
		case DECY:
			c->y--;
			break;
		case ADAX:
			c->acc += c->x;
			break;
		case ADAY:
			c->acc += c->y;
			break;
		case SUAX:
			c->acc -= c->x;
			break;
		case SUAY:
			c->acc -= c->y;
			break;
		case STRA:
			c->mem[c->address] = c->acc;
			break;
		case STRX:
			c->mem[c->address] = c->x;
			break;
		case STRY:
			c->mem[c->address] = c->y;
			break;
	}
}
