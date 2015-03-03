#pragma once

struct extendedInstruction {
	char *disassembly;
	void *execute;
	//unsigned char ticks;
} extern const extendedInstructions[256];

void cb_n(unsigned char instruction);
