#ifdef DS
#include "fakeWindows.h"
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "debug.h"
#include "main.h"

#include "cb.h"

const struct extendedInstruction extendedInstructions[256] = {
	{ "RLC B", rlc_b },      // 0x00
	{ "RLC C", rlc_c },      // 0x01
	{ "RLC D", rlc_d },      // 0x02
	{ "RLC E", rlc_e },      // 0x03
	{ "RLC H", rlc_h },      // 0x04
	{ "RLC L", rlc_l },      // 0x05
	{ "RLC (HL)", rlc_hlp }, // 0x06
	{ "RLC A", rlc_a },      // 0x07
	{ "RRC B", rrc_b },      // 0x08
	{ "RRC C", rrc_c },      // 0x09
	{ "RRC D", rrc_d },      // 0x0a
	{ "RRC E", rrc_e },      // 0x0b
	{ "RRC H", rrc_h },      // 0x0c
	{ "RRC L", rrc_l },      // 0x0d
	{ "RRC (HL)", rrc_hlp }, // 0x0e
	{ "RRC A", rrc_a },      // 0x0f
	{ "RL B", rl_b },        // 0x10
	{ "RL C", rl_c },        // 0x11
	{ "RL D", rl_d },        // 0x12
	{ "RL E", rl_e },        // 0x13
	{ "RL H", rl_h },        // 0x14
	{ "RL L", rl_l },        // 0x15
	{ "RL (HL)", rl_hlp },   // 0x16
	{ "RL A", rl_a },        // 0x17
	{ "RR B", rr_b },        // 0x18
	{ "RR C", rr_c },        // 0x19
	{ "RR D", rr_d },        // 0x1a
	{ "RR E", rr_e },        // 0x1b
	{ "RR H", rr_h },        // 0x1c
	{ "RR L", rr_l },        // 0x1d
	{ "RR (HL)", rr_hlp },   // 0x1e
	{ "RR A", rr_a },        // 0x1f
	{ "SLA B", sla_b },      // 0x20
	{ "SLA C", sla_c },      // 0x21
	{ "SLA D", sla_d },      // 0x22
	{ "SLA E", sla_e },      // 0x23
	{ "SLA H", sla_h },      // 0x24
	{ "SLA L", sla_l },      // 0x25
	{ "SLA (HL)", sla_hlp }, // 0x26
	{ "SLA A", sla_a },      // 0x27
	{ "SRA B", NULL },       // 0x28
	{ "SRA C", NULL },       // 0x29
	{ "SRA D", NULL },       // 0x2a
	{ "SRA E", NULL },       // 0x2b
	{ "SRA H", NULL },       // 0x2c
	{ "SRA L", NULL },       // 0x2d
	{ "SRA (HL)", NULL },    // 0x2e
	{ "SRA A", NULL },       // 0x2f
	{ "SWAP B", swap_b },    // 0x30
	{ "SWAP C", swap_c },    // 0x31
	{ "SWAP D", swap_d },    // 0x32
	{ "SWAP E", swap_e },    // 0x33
	{ "SWAP H", swap_h },    // 0x34
	{ "SWAP L", swap_l },    // 0x35
	{ "SWAP (HL)", swap_hlp },// 0x36
	{ "SWAP A", swap_a },    // 0x37
	{ "SRL B", NULL },       // 0x38
	{ "SRL C", NULL },       // 0x39
	{ "SRL D", NULL },       // 0x3a
	{ "SRL E", NULL },       // 0x3b
	{ "SRL H", NULL },       // 0x3c
	{ "SRL L", NULL },       // 0x3d
	{ "SRL (HL)", NULL },    // 0x3e
	{ "SRL A", srl_a },      // 0x3f
	{ "BIT 0, B", bit_0_b }, // 0x40
	{ "BIT 0, C", bit_0_c }, // 0x41
	{ "BIT 0, D", bit_0_d }, // 0x42
	{ "BIT 0, E", bit_0_e }, // 0x43
	{ "BIT 0, H", bit_0_h }, // 0x44
	{ "BIT 0, L", bit_0_l }, // 0x45
	{ "BIT 0, (HL)", bit_0_hlp },// 0x46
	{ "BIT 0, A", bit_0_a }, // 0x47
	{ "BIT 1, B", bit_1_b }, // 0x48
	{ "BIT 1, C", NULL },    // 0x49
	{ "BIT 1, D", NULL },    // 0x4a
	{ "BIT 1, E", NULL },    // 0x4b
	{ "BIT 1, H", NULL },    // 0x4c
	{ "BIT 1, L", NULL },    // 0x4d
	{ "BIT 1, (HL)", NULL }, // 0x4e
	{ "BIT 1, A", NULL },    // 0x4f
	{ "BIT 2, B", bit_2_b }, // 0x50
	{ "BIT 2, C", bit_2_c }, // 0x51
	{ "BIT 2, D", bit_2_d }, // 0x52
	{ "BIT 2, E", bit_2_e }, // 0x53
	{ "BIT 2, H", bit_2_h }, // 0x54
	{ "BIT 2, L", bit_2_l }, // 0x55
	{ "BIT 2, (HL)", bit_2_hlp }, // 0x56
	{ "BIT 2, A", bit_2_a }, // 0x57
	{ "BIT 3, B", bit_3_b }, // 0x58
	{ "BIT 3, C", bit_3_c }, // 0x59
	{ "BIT 3, D", bit_3_d }, // 0x5a
	{ "BIT 3, E", bit_3_e }, // 0x5b
	{ "BIT 3, H", bit_3_h }, // 0x5c
	{ "BIT 3, L", bit_3_l }, // 0x5d
	{ "BIT 3, (HL)", bit_3_hlp }, // 0x5e
	{ "BIT 3, A", bit_3_a }, // 0x5f
	{ "BIT 4, B", bit_4_b }, // 0x60
	{ "BIT 4, C", bit_4_c }, // 0x61
	{ "BIT 4, D", bit_4_d }, // 0x62
	{ "BIT 4, E", bit_4_e }, // 0x63
	{ "BIT 4, H", bit_4_h }, // 0x64
	{ "BIT 4, L", bit_4_l }, // 0x65
	{ "BIT 4, (HL)", NULL }, // 0x66
	{ "BIT 4, A", NULL },    // 0x67
	{ "BIT 5, B", bit_5_b }, // 0x68
	{ "BIT 5, C", bit_5_c }, // 0x69
	{ "BIT 5, D", bit_5_d }, // 0x6a
	{ "BIT 5, E", bit_5_e }, // 0x6b
	{ "BIT 6, H", bit_5_h }, // 0x6c
	{ "BIT 6, L", bit_5_l }, // 0x6d
	{ "BIT 5, (HL)", bit_5_hlp }, // 0x6e
	{ "BIT 5, A", bit_5_a }, // 0x6f
	{ "BIT 6, B", bit_6_b }, // 0x70
	{ "BIT 6, C", NULL },    // 0x71
	{ "BIT 6, D", NULL },    // 0x72
	{ "BIT 6, E", NULL },    // 0x73
	{ "BIT 6, H", NULL },    // 0x74
	{ "BIT 6, L", NULL },    // 0x75
	{ "BIT 6, (HL)", NULL }, // 0x76
	{ "BIT 6, A", bit_6_a }, // 0x77
	{ "BIT 7, B", bit_7_b }, // 0x78
	{ "BIT 7, C", bit_7_c }, // 0x79
	{ "BIT 7, D", bit_7_d }, // 0x7a
	{ "BIT 7, E", bit_7_e }, // 0x7b
	{ "BIT 7, H", bit_7_h }, // 0x7c
	{ "BIT 7, L", bit_7_l }, // 0x7d
	{ "BIT 7, (HL)", bit_7_hlp }, // 0x7e
	{ "BIT 7, A", bit_7_a }, // 0x7f
	{ "RES 0, B", NULL },    // 0x80
	{ "RES 0, C", NULL },    // 0x81
	{ "RES 0, D", NULL },    // 0x82
	{ "RES 0, E", NULL },    // 0x83
	{ "RES 0, H", NULL },    // 0x84
	{ "RES 0, L", NULL },    // 0x85
	{ "RES 0, (HL)", res_0_hlp }, // 0x86
	{ "RES 0, A", res_0_a }, // 0x87
	{ "RES 1, B", NULL },    // 0x88
	{ "RES 1, C", NULL },    // 0x89
	{ "RES 1, D", NULL },    // 0x8a
	{ "RES 1, E", NULL },    // 0x8b
	{ "RES 1, H", NULL },    // 0x8c
	{ "RES 1, L", NULL },    // 0x8d
	{ "RES 1, (HL)", NULL }, // 0x8e
	{ "RES 1, A", NULL },    // 0x8f
	{ "RES 2, B", NULL },    // 0x90
	{ "RES 2, C", NULL },    // 0x91
	{ "RES 2, D", NULL },    // 0x92
	{ "RES 2, E", NULL },    // 0x93
	{ "RES 2, H", NULL },    // 0x94
	{ "RES 2, L", NULL },    // 0x95
	{ "RES 2, (HL)", NULL }, // 0x96
	{ "RES 2, A", NULL },    // 0x97
	{ "RES 3, B", NULL },    // 0x98
	{ "RES 3, C", NULL },    // 0x99
	{ "RES 3, D", NULL },    // 0x9a
	{ "RES 3, E", NULL },    // 0x9b
	{ "RES 3, H", NULL },    // 0x9c
	{ "RES 3, L", NULL },    // 0x9d
	{ "RES 3, (HL)", NULL }, // 0x9e
	{ "RES 3, A", NULL },    // 0x9f
	{ "RES 4, B", NULL },    // 0xa0
	{ "RES 4, C", NULL },    // 0xa1
	{ "RES 4, D", NULL },    // 0xa2
	{ "RES 4, E", NULL },    // 0xa3
	{ "RES 4, H", NULL },    // 0xa4
	{ "RES 4, L", NULL },    // 0xa5
	{ "RES 4, (HL)", NULL }, // 0xa6
	{ "RES 4, A", NULL },    // 0xa7
	{ "RES 5, B", NULL },    // 0xa8
	{ "RES 5, C", NULL },    // 0xa9
	{ "RES 5, D", NULL },    // 0xaa
	{ "RES 5, E", NULL },    // 0xab
	{ "RES 5, H", NULL },    // 0xac
	{ "RES 5, L", NULL },    // 0xad
	{ "RES 5, (HL)", NULL }, // 0xae
	{ "RES 5, A", NULL },    // 0xaf
	{ "RES 6, B", NULL },    // 0xb0
	{ "RES 6, C", NULL },    // 0xb1
	{ "RES 6, D", NULL },    // 0xb2
	{ "RES 6, E", NULL },    // 0xb3
	{ "RES 6, H", NULL },    // 0xb4
	{ "RES 6, L", NULL },    // 0xb5
	{ "RES 6, (HL)", NULL }, // 0xb6
	{ "RES 6, A", NULL },    // 0xb7
	{ "RES 7, B", NULL },    // 0xb8
	{ "RES 7, C", NULL },    // 0xb9
	{ "RES 7, D", NULL },    // 0xba
	{ "RES 7, E", NULL },    // 0xbb
	{ "RES 7, H", NULL },    // 0xbc
	{ "RES 7, L", NULL },    // 0xbd
	{ "RES 7, (HL)", NULL }, // 0xbe
	{ "RES 7, A", NULL },    // 0xbf
	{ "SET 0, B", NULL },    // 0xc0
	{ "SET 0, C", NULL },    // 0xc1
	{ "SET 0, D", NULL },    // 0xc2
	{ "SET 0, E", NULL },    // 0xc3
	{ "SET 0, H", NULL },    // 0xc4
	{ "SET 0, L", NULL },    // 0xc5
	{ "SET 0, (HL)", NULL }, // 0xc6
	{ "SET 0, A", NULL },    // 0xc7
	{ "SET 1, B", NULL },    // 0xc8
	{ "SET 1, C", NULL },    // 0xc9
	{ "SET 1, D", NULL },    // 0xca
	{ "SET 1, E", NULL },    // 0xcb
	{ "SET 1, H", NULL },    // 0xcc
	{ "SET 1, L", NULL },    // 0xcd
	{ "SET 1, (HL)", NULL }, // 0xce
	{ "SET 1, A", NULL },    // 0xcf
	{ "SET 2, B", NULL },    // 0xd0
	{ "SET 2, C", NULL },    // 0xd1
	{ "SET 2, D", NULL },    // 0xd2
	{ "SET 2, E", NULL },    // 0xd3
	{ "SET 2, H", NULL },    // 0xd4
	{ "SET 2, L", NULL },    // 0xd5
	{ "SET 2, (HL)", NULL }, // 0xd6
	{ "SET 2, A", NULL },    // 0xd7
	{ "SET 3, B", NULL },    // 0xd8
	{ "SET 3, C", NULL },    // 0xd9
	{ "SET 3, D", NULL },    // 0xda
	{ "SET 3, E", NULL },    // 0xdb
	{ "SET 3, H", NULL },    // 0xdc
	{ "SET 3, L", NULL },    // 0xdd
	{ "SET 3, (HL)", NULL }, // 0xde
	{ "SET 3, A", NULL },    // 0xdf
	{ "SET 4, B", NULL },    // 0xe0
	{ "SET 4, C", NULL },    // 0xe1
	{ "SET 4, D", NULL },    // 0xe2
	{ "SET 4, E", NULL },    // 0xe3
	{ "SET 4, H", NULL },    // 0xe4
	{ "SET 4, L", NULL },    // 0xe5
	{ "SET 4, (HL)", NULL }, // 0xe6
	{ "SET 4, A", NULL },    // 0xe7
	{ "SET 5, B", NULL },    // 0xe8
	{ "SET 5, C", NULL },    // 0xe9
	{ "SET 5, D", NULL },    // 0xea
	{ "SET 5, E", NULL },    // 0xeb
	{ "SET 5, H", NULL },    // 0xec
	{ "SET 5, L", NULL },    // 0xed
	{ "SET 5, (HL)", NULL }, // 0xee
	{ "SET 5, A", NULL },    // 0xef
	{ "SET 6, B", NULL },    // 0xf0
	{ "SET 6, C", NULL },    // 0xf1
	{ "SET 6, D", NULL },    // 0xf2
	{ "SET 6, E", NULL },    // 0xf3
	{ "SET 6, H", NULL },    // 0xf4
	{ "SET 6, L", NULL },    // 0xf5
	{ "SET 6, (HL)", NULL }, // 0xf6
	{ "SET 6, A", NULL },    // 0xf7
	{ "SET 7, B", set_7_b }, // 0xf8
	{ "SET 7, C", set_7_c }, // 0xf9
	{ "SET 7, D", set_7_d }, // 0xfa
	{ "SET 7, E", set_7_e }, // 0xfb
	{ "SET 7, H", set_7_h }, // 0xfc
	{ "SET 7, L", set_7_l }, // 0xfd
	{ "SET 7, (HL)", set_7_hlp }, // 0xfe
	{ "SET 7, A", set_7_a }, // 0xff
};

const unsigned char extendedInstructionTicks[256] = {
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x0_
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x1_
	8, 8, 8, 8, 8,  8, 16, 8,  0, 0, 0, 0, 0, 0,  0, 0, // 0x2_
	8, 8, 8, 8, 8,  8, 16, 8,  0, 0, 0, 0, 0, 0,  0, 8, // 0x3_
	8, 8, 8, 8, 8,  8, 12, 0,  8, 0, 0, 0, 0, 0,  0, 0, // 0x4_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x5_
	8, 8, 8, 8, 8,  0,  0, 0,  8, 8, 8, 8, 8, 8,  8, 8, // 0x6_
	8, 0, 0, 0, 0,  0,  0, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x7_
	0, 0, 0, 0, 0,  0, 12, 8,  0, 0, 0, 0, 0, 0,  0, 0, // 0x8_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0x9_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xa_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xb_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xc_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xd_
	0, 0, 0, 0, 0,  0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xe_
	0, 0, 0, 0, 0,  0,  0, 0,  8, 8, 8, 8, 8, 8, 12, 8  // 0xf_
};

void cb_n(unsigned char instruction) {
	if(!extendedInstructions[instruction].execute) {
		printf("Unimplemented extended instruction 0x%02x (%s)!\n",  instruction, extendedInstructions[instruction].disassembly);
		
		char d[100];
		sprintf(d, "Unimplemented extended instruction 0x%02x!\n\nCheck stdout for more details.", instruction);
		MessageBox(NULL, d, "Cinoop", MB_OK);
		
		registers.pc -= 2;
		printRegisters();
		quit();
	}
	
	extendedInstructions[instruction].execute();
	
	ticks += extendedInstructionTicks[instruction];
}

static unsigned char rlc(unsigned char value) {
	int carry = (value & 0x80) >> 7;
	
	if(value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	value <<= 1;
	value += carry;
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

static unsigned char rrc(unsigned char value) {
	int carry = value & 0x01;
	
	value >>= 1;
	
	if(carry) {
		FLAGS_SET(FLAGS_CARRY);
		value |= 0x80;
	}
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

static unsigned char rl(unsigned char value) {
	int carry = FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0;
	
	if(value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	value <<= 1;
	value += carry;
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

static unsigned char rr(unsigned char value) {
	value >>= 1;
	if(FLAGS_ISCARRY) value |= 0x80;
	
	if(value & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

static unsigned char sla(unsigned char value) {
	if(value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	value <<= 1;
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

static unsigned char swap(unsigned char value) {
	value = ((value & 0xf) << 4) | ((value & 0xf0) >> 4);
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY | FLAGS_CARRY);
	
	return value;
}

static void bit(unsigned char bit, unsigned char value) {
	if(value & bit) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

static unsigned char set(unsigned char bit, unsigned char value) {
	value |= bit;
	return value;
}

// 0x00
void rlc_b(void) { registers.b = rlc(registers.b); }

// 0x01
void rlc_c(void) { registers.c = rlc(registers.c); }

// 0x02
void rlc_d(void) { registers.d = rlc(registers.d); }

// 0x03
void rlc_e(void) { registers.e = rlc(registers.e); }

// 0x04
void rlc_h(void) { registers.h = rlc(registers.h); }

// 0x05
void rlc_l(void) { registers.l = rlc(registers.l); }

// 0x06
void rlc_hlp(void) { writeByte(registers.hl, rlc(readByte(registers.hl))); }

// 0x07
void rlc_a(void) { registers.a = rlc(registers.a); }

// 0x08
void rrc_b(void) { registers.b = rrc(registers.b); }

// 0x09
void rrc_c(void) { registers.c = rrc(registers.c); }

// 0x0a
void rrc_d(void) { registers.d = rrc(registers.d); }

// 0x0b
void rrc_e(void) { registers.e = rrc(registers.e); }

// 0x0c
void rrc_h(void) { registers.h = rrc(registers.h); }

// 0x0d
void rrc_l(void) { registers.l = rrc(registers.l); }

// 0x0e
void rrc_hlp(void) { writeByte(registers.hl, rrc(readByte(registers.hl))); }

// 0x0f
void rrc_a(void) { registers.a = rrc(registers.a); }

// 0x10
void rl_b(void) { registers.b = rl(registers.b); }

// 0x11
void rl_c(void) { registers.c = rl(registers.c); }

// 0x12
void rl_d(void) { registers.d = rl(registers.d); }

// 0x13
void rl_e(void) { registers.e = rl(registers.e); }

// 0x14
void rl_h(void) { registers.h = rl(registers.h); }

// 0x15
void rl_l(void) { registers.l = rl(registers.l); }

// 0x16
void rl_hlp(void) { writeByte(registers.hl, rl(readByte(registers.hl))); }

// 0x17
void rl_a(void) { registers.a = rl(registers.a); }

// 0x18
void rr_b(void) { registers.b = rr(registers.b); }

// 0x19
void rr_c(void) { registers.c = rr(registers.c); }

// 0x1a
void rr_d(void) { registers.d = rr(registers.d); }

// 0x1b
void rr_e(void) { registers.e = rr(registers.e); }

// 0x1c
void rr_h(void) { registers.h = rr(registers.h); }

// 0x1d
void rr_l(void) { registers.l = rr(registers.l); }

// 0x1e
void rr_hlp(void) { writeByte(registers.hl, rr(readByte(registers.hl))); }

// 0x1f
void rr_a(void) { registers.a = rr(registers.a); }

// 0x20
void sla_b(void) { registers.b = sla(registers.b); }

// 0x21
void sla_c(void) { registers.c = sla(registers.c); }

// 0x22
void sla_d(void) { registers.d = sla(registers.d); }

// 0x23
void sla_e(void) { registers.e = sla(registers.e); }

// 0x24
void sla_h(void) { registers.h = sla(registers.h); }

// 0x25
void sla_l(void) { registers.l = sla(registers.l); }

// 0x26
void sla_hlp(void) { writeByte(registers.hl, sla(readByte(registers.hl))); }

// 0x27
void sla_a(void) { registers.a = sla(registers.a); }

// 0x30
void swap_b(void) { registers.b = swap(registers.b); }

// 0x31
void swap_c(void) { registers.c = swap(registers.c); }

// 0x32
void swap_d(void) { registers.d = swap(registers.d); }

// 0x33
void swap_e(void) { registers.e = swap(registers.e); }

// 0x34
void swap_h(void) { registers.h = swap(registers.h); }

// 0x35
void swap_l(void) { registers.l = swap(registers.l); }

// 0x36
void swap_hlp(void) { writeByte(registers.hl, swap(readByte(registers.hl))); }

// 0x37
void swap_a(void) { registers.a = swap(registers.a); }

// 0x3f
void srl_a(void) {
	if(registers.a & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	registers.a >>= 1;
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0x40
void bit_0_b(void) { bit(1 << 0, registers.b); }

// 0x41
void bit_0_c(void) { bit(1 << 0, registers.c); }

// 0x42
void bit_0_d(void) { bit(1 << 0, registers.d); }

// 0x43
void bit_0_e(void) { bit(1 << 0, registers.e); }

// 0x44
void bit_0_h(void) { bit(1 << 0, registers.h); }

// 0x45
void bit_0_l(void) { bit(1 << 0, registers.l); }

// 0x46
void bit_0_hlp(void) { bit(1 << 0, readByte(registers.hl)); }

// 0x47
void bit_0_a(void) { bit(1 << 0, registers.a); }

// 0x48
void bit_1_b(void) { bit(1 << 1, registers.b); }

// 0x50
void bit_2_b(void) { bit(1 << 2, registers.b); }

// 0x51
void bit_2_c(void) { bit(1 << 2, registers.c); }

// 0x52
void bit_2_d(void) { bit(1 << 2, registers.d); }

// 0x53
void bit_2_e(void) { bit(1 << 2, registers.e); }

// 0x54
void bit_2_h(void) { bit(1 << 2, registers.h); }

// 0x55
void bit_2_l(void) { bit(1 << 2, registers.l); }

// 0x56
void bit_2_hlp(void) { bit(1 << 2, readByte(registers.hl)); }

// 0x57
void bit_2_a(void) { bit(1 << 2, registers.a); }

// 0x58
void bit_3_b(void) { bit(1 << 3, registers.b); }

// 0x59
void bit_3_c(void) { bit(1 << 3, registers.c); }

// 0x5a
void bit_3_d(void) { bit(1 << 3, registers.d); }

// 0x5b
void bit_3_e(void) { bit(1 << 3, registers.e); }

// 0x5c
void bit_3_h(void) { bit(1 << 3, registers.h); }

// 0x5d
void bit_3_l(void) { bit(1 << 3, registers.l); }

// 0x5e
void bit_3_hlp(void) { bit(1 << 3, readByte(registers.hl)); }

// 0x5f
void bit_3_a(void) { bit(1 << 3, registers.a); }

// 0x60
void bit_4_b(void) { bit(1 << 4, registers.b); }

// 0x61
void bit_4_c(void) { bit(1 << 4, registers.c); }

// 0x62
void bit_4_d(void) { bit(1 << 4, registers.d); }

// 0x63
void bit_4_e(void) { bit(1 << 4, registers.e); }

// 0x64
void bit_4_h(void) { bit(1 << 4, registers.h); }

// 0x65
void bit_4_l(void) { bit(1 << 4, registers.l); }

// 0x68
void bit_5_b(void) { bit(1 << 5, registers.b); }

// 0x69
void bit_5_c(void) { bit(1 << 5, registers.c); }

// 0x6a
void bit_5_d(void) { bit(1 << 5, registers.d); }

// 0x6b
void bit_5_e(void) { bit(1 << 5, registers.e); }

// 0x6c
void bit_5_h(void) { bit(1 << 5, registers.h); }

// 0x6d
void bit_5_l(void) { bit(1 << 5, registers.l); }

// 0x6e
void bit_5_hlp(void) { bit(1 << 5, readByte(registers.hl)); }

// 0x6f
void bit_5_a(void) { bit(1 << 5, registers.a); }

// 0x70
void bit_6_b(void) { bit(1 << 6, registers.b); }

// 0x77
void bit_6_a(void) { bit(1 << 6, registers.a); }

// 0x78
void bit_7_b(void) { bit(1 << 7, registers.b); }

// 0x79
void bit_7_c(void) { bit(1 << 7, registers.c); }

// 0x7a
void bit_7_d(void) { bit(1 << 7, registers.d); }

// 0x7b
void bit_7_e(void) { bit(1 << 7, registers.e); }

// 0x7c
void bit_7_h(void) { bit(1 << 7, registers.h); }

// 0x7d
void bit_7_l(void) { bit(1 << 7, registers.l); }

// 0x7e
void bit_7_hlp(void) { bit(1 << 7, readByte(registers.hl)); }

// 0x7f
void bit_7_a(void) { bit(1 << 7, registers.a); }

// 0x86
void res_0_hlp(void) { writeByte(registers.hl, readByte(registers.hl) & ~0x01); }

// 0x87
void res_0_a(void) { registers.a &= ~0x01; }

// 0xf8
void set_7_b(void) { registers.b = set(1 << 7, registers.b); }

// 0xf9
void set_7_c(void) { registers.c = set(1 << 7, registers.c); }

// 0xfa
void set_7_d(void) { registers.d = set(1 << 7, registers.d); }

// 0xfb
void set_7_e(void) { registers.e = set(1 << 7, registers.e); }

// 0xfc
void set_7_h(void) { registers.h = set(1 << 7, registers.h); }

// 0xfd
void set_7_l(void) { registers.l = set(1 << 7, registers.l); }

// 0xfe
void set_7_hlp(void) { writeByte(registers.hl, set(1 << 7, readByte(registers.hl))); }

// 0xff
void set_7_a(void) { registers.a = set(1 << 7, registers.a); }
