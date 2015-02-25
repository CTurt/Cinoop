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
		
		// JP nn
		case 0xc3:
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
}
