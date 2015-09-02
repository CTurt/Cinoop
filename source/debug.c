#ifndef PS4
	#include <stdio.h>
#endif

#include "platform.h"

#include "registers.h"
#include "memory.h"
#include "cpu.h"
#include "interrupts.h"

#include "debug.h"

unsigned char realtimeDebugEnable = 0;

#ifdef WIN
void realtimeDebug(void) {
	char debugMessage[5000];
	char *debugMessageP = debugMessage;
	
	unsigned char instruction = readByte(registers.pc);
	unsigned short operand = 0;
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(registers.pc + 1);
	if(instructions[instruction].operandLength == 2) operand = readShort(registers.pc + 1);
	
	if(instructions[instruction].operandLength) debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly, operand);
	else debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly);
	
	debugMessageP += sprintf(debugMessageP, "\n\nAF: 0x%04x\n", registers.af);
	debugMessageP += sprintf(debugMessageP, "BC: 0x%04x\n", registers.bc);
	debugMessageP += sprintf(debugMessageP, "DE: 0x%04x\n", registers.de);
	debugMessageP += sprintf(debugMessageP, "HL: 0x%04x\n", registers.hl);
	debugMessageP += sprintf(debugMessageP, "SP: 0x%04x\n", registers.sp);
	debugMessageP += sprintf(debugMessageP, "PC: 0x%04x\n", registers.pc);
	
	debugMessageP += sprintf(debugMessageP, "\nIME: 0x%02x\n", interrupt.master);
	debugMessageP += sprintf(debugMessageP, "IE: 0x%02x\n", interrupt.enable);
	debugMessageP += sprintf(debugMessageP, "IF: 0x%02x\n", interrupt.flags);
	
	debugMessageP += sprintf(debugMessageP, "\nContinue debugging?\n");
	
	realtimeDebugEnable = MessageBox(NULL, debugMessage, "Cinoop Breakpoint", MB_YESNO) == IDYES ? 1 : 0;
}

#ifdef DEBUG_JUMP
void debugJump(void) {
	static unsigned short lastPC = 0;
	
	if(registers.pc != lastPC) {
		printf("Jumped to 0x%04x\n", registers.pc);
		lastPC = registers.pc;
	}
}
#endif

void printRegisters(void) {
	printf("A: 0x%02x\n", registers.a);
	printf("F: 0x%02x\n", registers.f);
	printf("B: 0x%02x\n", registers.b);
	printf("C: 0x%02x\n", registers.c);
	printf("D: 0x%02x\n", registers.d);
	printf("E: 0x%02x\n", registers.e);
	printf("H: 0x%02x\n", registers.h);
	printf("L: 0x%02x\n", registers.l);
	printf("SP: 0x%04x\n", registers.sp);
	printf("PC: 0x%04x\n", registers.pc);
	printf("IME: 0x%02x\n", interrupt.master);
	printf("IE: 0x%02x\n", interrupt.enable);
	printf("IF: 0x%02x\n", interrupt.flags);
}
#endif
