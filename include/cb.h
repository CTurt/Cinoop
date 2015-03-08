#pragma once

struct extendedInstruction {
	char *disassembly;
	void (*execute)(void);
	//unsigned char ticks;
} extern const extendedInstructions[256];

extern const unsigned char extendedInstructionTicks[256];

void cb_n(unsigned char instruction);

void sla_a(void);
void swap_e(void);
void swap_a(void);
void srl_a(void);
void bit_0_b(void);
void bit_2_b(void);
void bit_3_a(void);
void bit_5_a(void);
void bit_6_a(void);
void bit_7_hlp(void);
void bit_7_a(void);
void res_0_hlp(void);
void res_0_a(void);
