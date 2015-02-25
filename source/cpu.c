#include <stdio.h>

#include "registers.h"

#include "cpu.h"

void reset(void) {
	registers.a = 0x01;
	registers.b = 0x00;
	registers.c = 0x13;
	registers.d = 0x00;
	registers.e = 0xd8;
	registers.h = 0x01;
	registers.l = 0x4d;
	registers.pc = 0x100;
	registers.flags = 0xb0;
	registers.sp = 0xfffe;
	registers.ime = 0x00;
}

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
}
