#include <stdio.h>
#include <stdlib.h>

#include "registers.h"
#include "memory.h"

#include "cpu.h"

const unsigned char instructionTicks[256] = {
	2, 6, 4, 4, 2, 2, 4, 4, 10, 4, 4, 4, 2, 2, 4, 4,
	2, 6, 4, 4, 2, 2, 4, 4,  4, 4, 4, 4, 2, 2, 4, 4,
	4, 6, 4, 4, 2, 2, 4, 2,  4, 4, 4, 4, 2, 2, 4, 2,
	4, 6, 4, 4, 6, 6, 6, 2,  4, 4, 4, 4, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	4, 4, 4, 4, 4, 4, 2, 4,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	4, 6, 6, 6, 6, 8, 4, 8,  4, 2, 6, 0, 6, 6, 4, 8,
	4, 6, 6, 0, 6, 8, 4, 8,  4, 8, 6, 0, 6, 0, 4, 8,
	6, 6, 4, 0, 0, 8, 4, 8,  8, 2, 8, 0, 0, 0, 4, 8,
	6, 6, 4, 2, 0, 8, 4, 8,  6, 4, 8, 2, 0, 0, 4, 8
};

void reset(void) {
	registers.a = 0x01;
	registers.f = 0xb0;
	registers.b = 0x00;
	registers.c = 0x13;
	registers.d = 0x00;
	registers.e = 0xd8;
	registers.h = 0x01;
	registers.l = 0x4d;
	registers.sp = 0xfffe;
	registers.pc = 0x100;
	registers.ime = 0x00;
}

// http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// http://imrannazar.com/Gameboy-Z80-Opcode-Map
void cpuStep(void) {
	unsigned char instruction = readByte(registers.pc++);
	
	switch(instruction) {
		// NOP
		case 0x00:
			break;
		
		// INC A
		case 0x3c:
			registers.a++;
			break;
			
		// LD B,E
		case 0x43:
			registers.b = registers.e;
			break;
		
		// XOR A
		case 0xaf:
			registers.a = 0;
			
			//FLAGS_SET(gameboy_proc, FLAGS_ZERO);
			//FLAGS_CLEAR(gameboy_proc, FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
			
			break;
		
		// JP nn
		case 0xc3:
			registers.pc = readShort(registers.pc);
			break;
		
		// LDH (n),A
		case 0xe0:
			//write(read(registers.pc++) | 0xFF00, registers.a);
			break;
		
		default:
			printf("Undefined instruction %#01x!\n", instruction);
			exit(1);
			break;
	}
	
	// ticks += instructionTicks[instruction];
}
