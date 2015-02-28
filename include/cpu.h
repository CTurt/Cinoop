#pragma once

#define FLAGS_ZERO 0x80
#define FLAGS_NEGATIVE 0x40
#define FLAGS_HALFCARRY 0x20
#define FLAGS_CARRY 0x10

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

void cpuStep(void);

void nop(void);
void dec_b(void);
void ld_b_n(unsigned char operand);
void ld_c_n(unsigned char operand);
void jr_nz_n(char operand);
void ld_hl_nn(unsigned short operand);
void ldd_hlp_a(void);
void inc_a(void);
void ld_b_e(void);
void xor_a(void);
void jp_nn(unsigned short operand);
