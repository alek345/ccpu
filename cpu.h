#ifndef _CPU_H
#define _CPU_H
#include <stdint.h>
#include <stdlib.h>

// * = Implemented in assembler and cpu.c
// # = Implemented in cpu.c
#define NOP		0x00	// *NOP instruction
#define HLT		0x01	// *Halts the cpu
#define LDAX	0x02	// *Load acc with register x
#define LDAY	0x03	// *Load acc with register y
#define LDXY	0x04	// *Load x with register y
#define LDXA	0x05	// *Load x with register acc
#define LDYX	0x06	// *Load y with register x
#define LDYA 	0x07	// *Load y with register acc
#define LDHA	0x08	// *Load the high byte of address with next byte
#define LDLA	0x09	// *Load the low byte of address with next byte
#define LDXM	0x0A	// *Load x with byte at address in memory
#define LDYM	0x0B	// *Load y with byte at address in memory
#define LDAM	0x0C	// *Load acc with byte at address in memory
#define INCA	0x0D	// *Increment acc
#define INCX	0x0E	// *Increment x
#define INCY	0x0F	// *Increment y
#define DECA	0x10	// *Decrement acc
#define DECX	0x11	// *Decrement x
#define DECY	0x12	// *Decrement y
#define ADAX	0x13	// *Add x to acc
#define ADAY	0x14	// *Add y to acc
#define SUAX	0x15	// *Sub x from acc
#define SUAY	0x16	// *Sub y from acc
#define STRA	0x17	// *Store acc at address in memory
#define STRX	0x18	// *Store x at address in memory
#define STRY	0x19	// *Store y at address in memory
#define LDAC	0x1A	// *Load acc with value of next byte
#define LDXC	0x1B	// *Load x with value of next byte
#define LDYC	0x1C	// *Load y with value of next byte
#define JMP		0x1D	// *Set IP to address
#define INCD	0x1E	// *Increment address
#define CMPXY	0x1F	// *Compare x and y
#define CMPAX	0x20	// *Compare a and x
#define CMPAY	0x21	// *Compare a and y
#define JE		0x22	// *Jump on equal
#define JNE		0x23	// *Jimp when not equal
#define JL		0x24	// *Jump when first reg is lower then second reg
#define JG		0x25	// *Jump when first reg is higher then second reg
#define ADDA 	0x26	// *Add acc to address
#define ADDX	0x27	// *Add x to address
#define ADDY	0x28	// *Add y to address
#define RET		0x29	// *Pop address from call level stack, and jump there
#define STRHD 	0x2A	// *Store higer address at address in memory
#define STRLD	0x2B	// *Store lower address at address in memory
#define LDAHD	0x2C	// *Load acc with higer address
#define LDXHD	0x2D	// *Load x with higher address
#define LDYHD	0x2E	// *Load y with higher address
#define LDALD	0x2F	// *Load acc with lower address
#define LDXLD	0x30	// *Load x with higher address
#define LDYLD	0x31	// *Load y with higher address
#define INT		0x32	// *Trigger software interrupt with the value of next byte
#define JSR		0x33	// *Set IP to address, also push next intruction to call level stack
#define MUAX	0x34	// *Multiply acc with x, store result in acc
#define MUAY	0x35	// *Multiply acc with y, store result in acc
#define DIAX	0x36	// *Divide acc by x, store result in acc
#define DIAY	0x37	// *Divide acc by y, store result in acc
#define MUAXM 	0x38 	// *Multiply acc with x, store result at address in memory. This result will be stored as a word
#define MUAYM	0x39	// *Multiply acc with y, store result at address in memory. This result will be stored as a word
#define DIAXM	0x3A 	// *Divide acc by x, store result at address in memory
#define DIAYM	0x3B	// *Divide acc by y, store result at address in memory
#define ADDXY	0x3C	// *Add x:y to address
#define SUDXY	0x3D	// *Sub x:y from address
#define PUSHA 	0x3E	// *Push acc to stack
#define PUSHX	0x3F	// *Push x to stack
#define PUSHY	0x40	// *Push y to stack
#define POP		0x41	// *Pop stack
#define POPA 	0x42 	// *Pop stack into acc
#define POPX	0x43	// *Pop stack into x
#define POPY	0x44	// *Pop stack into y
#define PUSHHD	0x45	// *Push high address to stack
#define PUSHLD	0x46	// *Push low address to stack
#define POPHD	0x47	// *Pop stack into high address
#define POPLD	0x48	// *Pop stack into low address
#define PUSHM	0x49	// *Push value at address in memory to stack
#define POPM	0x4A	// *Pop stack into address in memory

#define KEYBOARD_BUFFER_LENGTH	32

#define IVT_START			0x0300
#define STACK_START			0x0400
#define CALLSTACK_START		0x0500
#define VIDEO_START			0x8000

#define KEYBOARD_BUFFER_INDEX	0x21
#define STACK_INDEX				0x22
#define CALLSTACK_INDEX			0x23
#define SCREEN_CURSOR_X			0x24
#define SCREEN_CURSOR_Y			0x25
#define SCREEN_COLOR			0x26

typedef uint8_t u8;
typedef uint16_t u16;

typedef enum{
	NONE,
	EQUAL,
	NEQUAL,
	GREATER,
	LESS
} CMP;

typedef struct{
	u8* mem;
	u16 address;
	u8 acc;
	u8 x;
	u8 y;
	u16 ip;
	u8 hlt;
	CMP cmp;
} CPU;

// Intialize and return a CPU struct ready for use
CPU* cpu_init(u16 start_addr);

// Do one full machine cycle, which may be 1 or more clock cycles long
void cpu_cycle(CPU* c);

void cpu_add_key_to_buffer(CPU* c, u8 key);

#endif /* _CPU_H */
