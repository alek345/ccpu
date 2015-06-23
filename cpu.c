#include "cpu.h"

CMP compare(u8 first, u8 second)
{
	CMP cmp;
	
	if(first == second){
		cmp = EQUAL;
	}else{
		cmp = NEQUAL;
	}
	if(first > second) {
		cmp = GREATER;
	}else if(first < second){
		cmp = LESS;
	}

	return cmp;
}

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
	c->cmp = NONE;	

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
			c->address &= ~0xFF00;
			c->address |= (c->mem[c->ip++] << 8);
			break;
		case LDLA:
			c->address &= ~0xFF;
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
		case LDAC:
			c->acc = c->mem[c->ip++];
			break;
		case LDXC:
			c->x = c->mem[c->ip++];
			break;
		case LDYC:
			c->y = c->mem[c->ip++];
			break;
		case JMP:
			c->ip = c->address;
			break;
		case INCD:
			c->address++;
			break;
		case CMPXY:
			c->cmp = compare(c->x, c->y);
			break;
		case CMPAX:
			c->cmp = compare(c->acc, c->x);
			break;
		case CMPAY:
			c->cmp = compare(c->acc, c->y);
			break;
		case JE:
			if(c->cmp == EQUAL) c->ip = c->address;
			break;
		case JNE:
			if(c->cmp == NEQUAL) c->ip = c->address;
			break;
		case JL:
			if(c->cmp == LESS) c->ip = c->address;
			break;
		case JG:
			if(c->cmp == GREATER) c->ip = c->address;
			break;
		case ADDA:
			c->address += c->acc;
			break;
		case ADDX:
			c->address += c->x;
			break;
		case ADDY:
			c->address += c->y;
			break;
	}
}
