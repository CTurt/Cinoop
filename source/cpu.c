#include <stdio.h>

#include "registers.h"

#include "cpu.h"

void reset(void) {
	registers.pc = 0x100;
}

// http://imrannazar.com/Gameboy-Z80-Opcode-Map
void cpuStep(void) {
	unsigned char instruction = read(registers.pc++);
	
	switch(instruction) {
		// LD B,E
		case 0x43:
			registers.b = registers.e;
			break;
		
		default:
			printf("Undefined instruction %#01x!\n", instruction);
			exit(1);
			break;
	}
}
