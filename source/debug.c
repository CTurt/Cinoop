#include <windows.h>
#include <stdio.h>

#include "registers.h"
#include "memory.h"
#include "cpu.h"
#include "interrupts.h"

#include "debug.h"

unsigned char realtimeDebugEnable = 1;

void realtimeDebug(void) {
	char debugMessage[5000];
	char *debugMessageP = debugMessage;
	
	unsigned char instruction = readByte(registers.pc);
	unsigned short operand = 0;
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(registers.pc + 1);
	if(instructions[instruction].operandLength == 2) operand = readShort(registers.pc + 1);
	
	if(instructions[instruction].operandLength) debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly, operand);
	else debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly);
	
	debugMessageP += sprintf(debugMessageP, "\n\nA: 0x%02x\n", registers.a);
	debugMessageP += sprintf(debugMessageP, "F: 0x%02x\n", registers.f);
	debugMessageP += sprintf(debugMessageP, "B: 0x%02x\n", registers.b);
	debugMessageP += sprintf(debugMessageP, "C: 0x%02x\n", registers.c);
	debugMessageP += sprintf(debugMessageP, "D: 0x%02x\n", registers.d);
	debugMessageP += sprintf(debugMessageP, "E: 0x%02x\n", registers.e);
	debugMessageP += sprintf(debugMessageP, "H: 0x%02x\n", registers.h);
	debugMessageP += sprintf(debugMessageP, "L: 0x%02x\n", registers.l);
	debugMessageP += sprintf(debugMessageP, "SP: 0x%02x\n", registers.sp);
	debugMessageP += sprintf(debugMessageP, "PC: 0x%04x\n", registers.pc);
	debugMessageP += sprintf(debugMessageP, "IME: 0x%04x\n", interrupt.master);
	debugMessageP += sprintf(debugMessageP, "IE: 0x%02x\n", interrupt.enable);
	debugMessageP += sprintf(debugMessageP, "IF: 0x%02x\n", interrupt.flags);
	
	debugMessageP += sprintf(debugMessageP, "\nContinue debugging?\n");
	
	realtimeDebugEnable = MessageBox(NULL, debugMessage, "Cinoop Breakpoint", MB_YESNO) == IDYES ? 1 : 0;
}

void debugJump(void) {
	static unsigned short lastPC = 0;
	
	if(registers.pc != lastPC) {
		printf("Jumped to 0x%04x\n", registers.pc);
		lastPC = registers.pc;
	}
}

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
