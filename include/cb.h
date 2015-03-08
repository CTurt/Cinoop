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
void bit_1_b(void);
void bit_2_b(void);
void bit_3_a(void);
void bit_4_b(void);
void bit_4_c(void);
void bit_4_d(void);
void bit_4_e(void);
void bit_4_h(void);
void bit_4_l(void);
void bit_5_b(void);
void bit_5_c(void);
void bit_5_d(void);
void bit_5_e(void);
void bit_5_h(void);
void bit_5_l(void);
void bit_5_hlp(void);
void bit_5_a(void);
void bit_6_a(void);
void bit_7_hlp(void);
void bit_7_a(void);
void res_0_hlp(void);
void res_0_a(void);
