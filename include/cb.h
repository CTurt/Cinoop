#pragma once

struct extendedInstruction {
	char *disassembly;
	void (*execute)(void);
	//unsigned char ticks;
} extern const extendedInstructions[256];

extern const unsigned char extendedInstructionTicks[256];

void cb_n(unsigned char instruction);

void rlc_b(void);
void rlc_c(void);
void rlc_d(void);
void rlc_e(void);
void rlc_h(void);
void rlc_l(void);
void rlc_hlp(void);
void rlc_a(void);
void rrc_b(void);
void rrc_c(void);
void rrc_d(void);
void rrc_e(void);
void rrc_h(void);
void rrc_l(void);
void rrc_hlp(void);
void rrc_a(void);
void sla_a(void);
void swap_b(void);
void swap_c(void);
void swap_d(void);
void swap_e(void);
void swap_h(void);
void swap_l(void);
void swap_hlp(void);
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
void bit_6_b(void);
void bit_6_a(void);
void bit_7_hlp(void);
void bit_7_a(void);
void res_0_hlp(void);
void res_0_a(void);
