#pragma once

struct extendedInstruction {
	char *disassembly;
	void (*execute)(void);
	//unsigned char ticks;
} extern const extendedInstructions[256];

extern const unsigned char extendedInstructionTicks[256];

void cb_n(unsigned char instruction);

void swap_a(void);
void res_0_a(void);
