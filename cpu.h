#ifndef _CPU_H
#define _CPU_H
#include <stdint.h>
#include <stdlib.h>

#define NOP		0x00	// NOP instruction
#define HLT		0x01	// Halts the cpu
#define LDAX	0x02	// Load acc with register x
#define LDAY	0x03	// Load acc with register y
#define LDXY	0x04	// Load x with register y
#define LDXA	0x05	// Load x with register acc
#define LDYX	0x06	// Load y with register x
#define LDYA 	0x07	// Load y with register acc
#define LDHA	0x08	// Load the high byte of address with next byte
#define LDLA	0x09	// Load the low byte of address with next byte
#define LDXM	0x0A	// Load x with byte at address in memory
#define LDYM	0x0B	// Load y with byte at address in memory
#define LDAM	0x0C	// Load acc with byte at address in memory
#define INCA	0x0D	// Increment acc
#define INCX	0x0E	// Increment x
#define INCY	0x0F	// Increment y
#define DECA	0x10	// Decrement acc
#define DECX	0x11	// Decrement x
#define DECY	0x12	// Decrement y
#define ADAX	0x13	// Add x to acc
#define ADAY	0x14	// Add y to acc
#define SUAX	0x15	// Sub x from acc
#define SUAY	0x16	// Sub y from acc
#define STRA	0x17	// Store acc at address in memory
#define STRX	0x18	// Store x at address in memory
#define STRY	0x19	// Store y at address in memory

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct{
	u8* mem;
	u16 address;
	u8 acc;
	u8 x;
	u8 y;
	u16 ip;
	u8 hlt;
} CPU;

// Intialize and return a CPU struct ready for use
CPU* cpu_init(u16 start_addr);

// Do one full machine cycle, which may be 1 or more clock cycles long
void cpu_cycle(CPU* c);

#endif /* _CPU_H */
