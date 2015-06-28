#include "cpu.h"

void cpu_add_key_to_buffer(CPU* c, u8 key)
{
	if(c->mem[KEYBOARD_BUFFER_INDEX] >= KEYBOARD_BUFFER_LENGTH) return;
	c->mem[c->mem[KEYBOARD_BUFFER_INDEX]++] = key;
}

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
	u16 result;
	
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
		case RET:
			if(c->mem[CALLSTACK_INDEX] == 0) break;
			u8 lb = c->mem[CALLSTACK_START+(--c->mem[CALLSTACK_INDEX])];
			u8 hb = c->mem[CALLSTACK_START+(--c->mem[CALLSTACK_INDEX])];
			c->address = (hb << 8) | lb;
			c->ip = c->address;
			break;
		case STRHD:
			c->mem[c->address] = (c->address & 0xFF00) >> 8;
			break;
		case STRLD:
			c->mem[c->address] = c->address & 0xFF;
			break;
		case LDAHD:
			c->acc = (c->address & 0xFF00) >> 8;
			break;
		case LDXHD:
			c->x = (c->address & 0xFF00) >> 8;
			break;
		case LDYHD:
			c->y = (c->address & 0xFF00) >> 8;
			break;
		case LDALD:
			c->acc = c->address & 0xFF;
			break;
		case LDXLD:
			c->x = c->address & 0xFF;
			break;
		case LDYLD:
			c->y = c->address & 0xFF;
			break;
		case INT:
			if(c->mem[CALLSTACK_INDEX] >= 255) break;
			c->mem[CALLSTACK_START+(c->mem[CALLSTACK_INDEX]++)] = ((c->ip+1) & 0xFF00) >> 8;
			c->mem[CALLSTACK_START+(c->mem[CALLSTACK_INDEX]++)] = ((c->ip+1) & 0xFF);
			u8 n = c->mem[c->ip++];
			c->ip = (c->mem[IVT_START+(n*2)] << 8) | c->mem[IVT_START+(n*2)+1];
			break;
		case JSR:
			if(c->mem[CALLSTACK_INDEX] >= 255) break;
			c->mem[CALLSTACK_START+(c->mem[CALLSTACK_INDEX]++)] = ((c->ip) & 0xFF00) >> 8;
			c->mem[CALLSTACK_START+(c->mem[CALLSTACK_INDEX]++)] = ((c->ip) & 0xFF);
			c->ip = c->address;
			break;
		case MUAX:
			c->acc *= c->x;
			break;
		case MUAY:
			c->acc *= c->y;
			break;
		case DIAX:
			c->acc /= c->x;
			break;
		case DIAY:
			c->acc /= c->y;
			break;
		case MUAXM:
			result = c->acc * c->x;
			c->mem[c->address] = (result & 0xFF00) << 8;
			c->mem[c->address+1] = (result & 0xFF);
			break;
		case MUAYM:
			result = c->acc * c->y;
			c->mem[c->address] = (result & 0xFF00) << 8;
			c->mem[c->address+1] = (result & 0xFF);
			break;
		case DIAXM:
			c->mem[c->address] = c->acc / c->x;
			break;
		case DIAYM:
			c->mem[c->address] = c->acc / c->y;
			break;
		case ADDXY:
			c->address += (c->x << 8) | c->y;
			break;
		case SUDXY:
			c->address -= (c->x << 8) | c->y;
			break;
		case PUSHA:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = c->acc;
			break;
		case PUSHX:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = c->x;
			break;
		case PUSHY:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = c->y;
			break;
		case POP:
			c->mem[STACK_INDEX]--;
			break;
		case POPA:
			c->acc = c->mem[STACK_START+(--c->mem[STACK_INDEX])];
			break;
		case POPX:
			c->x = c->mem[STACK_START+(--c->mem[STACK_INDEX])];
			break;
		case POPY:
			c->y = c->mem[STACK_START+(--c->mem[STACK_INDEX])];
			break;
		case PUSHHD:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = (c->address & 0xFF00) >> 8;
			break;
		case PUSHLD:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = c->address & 0xFF;
			break;
		case POPHD:
			c->address &= ~0xFF00;
			c->address |= (c->mem[STACK_START+(--c->mem[STACK_INDEX])] << 8);
			break;
		case POPLD:
			c->address &= ~0xFF;
			c->address |= c->mem[STACK_START+(--c->mem[STACK_INDEX])];
			break;
		case PUSHM:
			c->mem[STACK_START+c->mem[STACK_INDEX]++] = c->mem[c->address];
			break;
		case POPM:
			c->mem[c->address] = c->mem[STACK_START+(--c->mem[STACK_INDEX])];
			break;
	}
}
