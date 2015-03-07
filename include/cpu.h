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
void rlca(void);
void ld_nnp_sp(unsigned short operand);
void dec_bc(void);
void inc_c(void);
void dec_c(void);
void ld_c_n(unsigned char operand);
void rrca(void);
void ld_de_nn(unsigned short operand);
void ld_dep_a(void);
void inc_de(void);
void ld_d_n(unsigned char operand);
void jr_n(char operand);
void add_hl_de(void);
void ld_a_dep(void);
void dec_de(void);
void inc_e(void);
void dec_e(void);
void jr_nz_n(char operand);
void ld_hl_nn(unsigned short operand);
void ldi_hlp_a(void);
void inc_hl(void);
void inc_h(void);
void dec_h(void);
void daa(void);
void jr_z_n(char operand);
void ldi_a_hlp(void);
void inc_l(void);
void dec_l(void);
void cpl(void);
void ld_sp_nn(unsigned short operand);
void ldd_hlp_a(void);
void inc_hlp(void);
void dec_hlp(void);
void ld_hlp_n(unsigned char operand);
void jr_c_n(char operand);
void inc_a(void);
void dec_a(void);
void ld_a_n(unsigned char operand);
void ld_b_c(void);
void ld_b_d(void);
void ld_b_e(void);
void ld_b_h(void);
void ld_b_l(void);
void ld_b_hlp(void);
void ld_b_a(void);
void ld_c_b(void);
void ld_c_d(void);
void ld_c_e(void);
void ld_c_h(void);
void ld_c_l(void);
void ld_c_hlp(void);
void ld_c_a(void);
void ld_d_b(void);
void ld_d_c(void);
void ld_d_e(void);
void ld_d_h(void);
void ld_d_l(void);
void ld_d_hlp(void);
void ld_d_a(void);
void ld_e_b(void);
void ld_e_c(void);
void ld_e_d(void);
void ld_e_h(void);
void ld_e_l(void);
void ld_e_hlp(void);
void ld_e_a(void);
void ld_h_b(void);
void ld_h_c(void);
void ld_h_d(void);
void ld_h_e(void);
void ld_h_l(void);
void ld_h_hlp(void);
void ld_h_a(void);
void ld_l_b(void);
void ld_l_c(void);
void ld_l_d(void);
void ld_l_e(void);
void ld_l_h(void);
void ld_l_hlp(void);
void ld_l_a(void);
void halt(void);
void ld_hlp_a(void);
void ld_a_b(void);
void ld_a_c(void);
void ld_a_d(void);
void ld_a_e(void);
void ld_a_h(void);
void ld_a_l(void);
void ld_a_hlp(void);
void add_a_b(void);
void add_a_c(void);
void add_a_d(void);
void add_a_e(void);
void add_a_h(void);
void add_a_l(void);
void add_a_hlp(void);
void add_a_a(void);
void sub_b(void);
void sub_c(void);
void sub_d(void);
void sub_e(void);
void sub_h(void);
void sub_l(void);
void sub_hlp(void);
void sub_a(void);
void and_b(void);
void and_c(void);
void and_d(void);
void and_e(void);
void and_h(void);
void and_l(void);
void and_hlp(void);
void and_a(void);
void xor_b(void);
void xor_c(void);
void xor_d(void);
void xor_e(void);
void xor_h(void);
void xor_l(void);
void xor_hlp(void);
void xor_a(void);
void or_b(void);
void or_c(void);
void or_d(void);
void or_e(void);
void or_h(void);
void or_l(void);
void ret_nz(void);
void pop_bc(void);
void jp_nn(unsigned short operand);
void push_bc(void);
void ret_z(void);
void ret(void);
void jp_z_nn(unsigned short operand);
void call_nn(unsigned short operand);
void pop_de(void);
void push_de(void);
void ret_c(void);
void jp_c_nn(unsigned short operand);
void ld_ff_n_ap(unsigned char operand);
void pop_hl(void);
void ld_ff_c_a(void);
void push_hl(void);
void and_n(unsigned char operand);
void jp_hl(void);
void ld_nnp_a(unsigned short operand);
void rst_28(void);
void ld_ff_ap_n(unsigned char operand);
void pop_af(void);
void di(void);
void push_af(void);
void ld_a_nnp(unsigned short operand);
void ei(void);
void cp_n(unsigned char operand);
void rst_38(void);
