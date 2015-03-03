#pragma once

#define FLAGS_ZERO 0x80 // 1 << 7
#define FLAGS_NEGATIVE 0x40 // 1 << 6
#define FLAGS_HALFCARRY 0x20 // 1 << 5
#define FLAGS_CARRY 0x10 // 1 << 4

#define FLAGS_ISZERO		(registers.f & FLAGS_ZERO)
#define FLAGS_ISNEGATIVE	(registers.f & FLAGS_NEGATIVE)
#define FLAGS_ISCARRY		(registers.f & FLAGS_CARRY)
#define FLAGS_ISHALFCARRY	(registers.f & FLAGS_HALFCARRY)

#define FLAGS_ISSET(x)	(registers.f & (x))
#define FLAGS_SET(x)	(registers.f |= (x))
#define FLAGS_CLEAR(x)	(registers.f &= ~(x))

struct instruction {
	char *disassembly;
	unsigned char operandLength;
	void *execute;
	//unsigned char ticks;
} extern const instructions[256];

extern const unsigned char instructionTicks[256];

extern unsigned long ticks;

void reset(void);
void cpuStep(void);

void nop(void);
void ld_bc_nn(unsigned short operand);
void ld_bcp_a(void);
void inc_bc(void);
void inc_b(void);
void dec_b(void);
void ld_b_n(unsigned char operand);
void ld_nnp_sp(unsigned short operand);
void dec_bc(void);
void inc_c(void);
void dec_c(void);
void ld_c_n(unsigned char operand);
void dec_de(void);
void jr_nz_n(char operand);
void ld_hl_nn(unsigned short operand);
void daa(void);
void jr_z_n(char operand);
void ldi_a_hlp(void);
void ld_sp_nn(unsigned short operand);
void ldd_hlp_a(void);
void inc_hlp(void);
void ld_hlp_n(unsigned char operand);
void jr_c_n(char operand);
void inc_a(void);
void dec_a(void);
void ld_a_n(unsigned char operand);
void ld_b_e(void);
void ld_hlp_a(void);
void ld_a_b(void);
void ld_a_c(void);
void ld_a_d(void);
void ld_a_e(void);
void ld_a_h(void);
void ld_a_l(void);
void and_a(void);
void xor_a(void);
void or_c(void);
void ret_nz(void);
void jp_nn(unsigned short operand);
void push_bc(void);
void ret_z(void);
void ret(void);
void call_nn(unsigned short operand);
void push_de(void);
void ld_ff_n_ap(unsigned char operand);
void pop_hl(void);
void ld_ff_c_a(void);
void push_hl(void);
void and_n(unsigned char operand);
void ld_nnp_a(unsigned short operand);
void ld_ff_ap_n(unsigned char operand);
void di(void);
void push_af(void);
void ld_a_nnp(unsigned short operand);
void ei(void);
void cp_n(unsigned char operand);
