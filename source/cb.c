#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "debug.h"

#include "cb.h"

const struct extendedInstruction extendedInstructions[256] = {
	{ "RLC B", NULL },       // 0x00
	{ "RLC C", NULL },       // 0x01
	{ "RLC D", NULL },       // 0x02
	{ "RLC E", NULL },       // 0x03
	{ "RLC H", NULL },       // 0x04
	{ "RLC L", NULL },       // 0x05
	{ "RLC (HL)", NULL },    // 0x06
	{ "RLC A", NULL },       // 0x07
	{ "RRC B", NULL },       // 0x08
	{ "RRC C", NULL },       // 0x09
	{ "RRC D", NULL },       // 0x0a
	{ "RRC E", NULL },       // 0x0b
	{ "RRC H", NULL },       // 0x0c
	{ "RRC L", NULL },       // 0x0d
	{ "RRC (HL)", NULL },    // 0x0e
	{ "RRC A", NULL },       // 0x0f
	{ "RL B", NULL },        // 0x10
	{ "RL C", NULL },        // 0x11
	{ "RL D", NULL },        // 0x12
	{ "RL E", NULL },        // 0x13
	{ "RL H", NULL },        // 0x14
	{ "RL L", NULL },        // 0x15
	{ "RL (HL)", NULL },     // 0x16
	{ "RL A", NULL },        // 0x17
	{ "RR B", NULL },        // 0x18
	{ "RR C", NULL },        // 0x19
	{ "RR D", NULL },        // 0x1a
	{ "RR E", NULL },        // 0x1b
	{ "RR H", NULL },        // 0x1c
	{ "RR L", NULL },        // 0x1d
	{ "RR (HL)", NULL },     // 0x1e
	{ "RR A", NULL },        // 0x1f
	{ "SLA B", NULL },       // 0x20
	{ "SLA C", NULL },       // 0x21
	{ "SLA D", NULL },       // 0x22
	{ "SLA E", NULL },       // 0x23
	{ "SLA H", NULL },       // 0x24
	{ "SLA L", NULL },       // 0x25
	{ "SLA (HL)", NULL },    // 0x26
	{ "SLA A", sla_a },      // 0x27
	{ "SRA B", NULL },       // 0x28
	{ "SRA C", NULL },       // 0x29
	{ "SRA D", NULL },       // 0x2a
	{ "SRA E", NULL },       // 0x2b
	{ "SRA H", NULL },       // 0x2c
	{ "SRA L", NULL },       // 0x2d
	{ "SRA (HL)", NULL },    // 0x2e
	{ "SRA A", NULL },       // 0x2f
	{ "SWAP B", NULL },      // 0x30
	{ "SWAP C", NULL },      // 0x31
	{ "SWAP D", NULL },      // 0x32
	{ "SWAP E", swap_e },    // 0x33
	{ "SWAP H", NULL },      // 0x34
	{ "SWAP L", NULL },      // 0x35
	{ "SWAP (HL)", NULL },   // 0x36
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
	{ "BIT 0, C", NULL },    // 0x41
	{ "BIT 0, D", NULL },    // 0x42
	{ "BIT 0, E", NULL },    // 0x43
	{ "BIT 0, H", NULL },    // 0x44
	{ "BIT 0, L", NULL },    // 0x45
	{ "BIT 0, (HL)", NULL }, // 0x46
	{ "BIT 0, A", NULL },    // 0x47
	{ "BIT 1, B", bit_1_b }, // 0x48
	{ "BIT 1, C", NULL },    // 0x49
	{ "BIT 1, D", NULL },    // 0x4a
	{ "BIT 1, E", NULL },    // 0x4b
	{ "BIT 1, H", NULL },    // 0x4c
	{ "BIT 1, L", NULL },    // 0x4d
	{ "BIT 1, (HL)", NULL }, // 0x4e
	{ "BIT 1, A", NULL },    // 0x4f
	{ "BIT 2, B", bit_2_b }, // 0x50
	{ "BIT 2, C", NULL },    // 0x51
	{ "BIT 2, D", NULL },    // 0x52
	{ "BIT 2, E", NULL },    // 0x53
	{ "BIT 2, H", NULL },    // 0x54
	{ "BIT 2, L", NULL },    // 0x55
	{ "BIT 2, (HL)", NULL }, // 0x56
	{ "BIT 2, A", NULL },    // 0x57
	{ "BIT 3, B", NULL },    // 0x58
	{ "BIT 3, C", NULL },    // 0x59
	{ "BIT 3, D", NULL },    // 0x5a
	{ "BIT 3, E", NULL },    // 0x5b
	{ "BIT 3, H", NULL },    // 0x5c
	{ "BIT 3, L", NULL },    // 0x5d
	{ "BIT 3, (HL)", NULL }, // 0x5e
	{ "BIT 3, A", bit_3_a }, // 0x5f
	{ "BIT 4, B", bit_4_b }, // 0x60
	{ "BIT 4, C", NULL },    // 0x61
	{ "BIT 4, D", NULL },    // 0x62
	{ "BIT 4, E", NULL },    // 0x63
	{ "BIT 4, H", NULL },    // 0x64
	{ "BIT 4, L", NULL },    // 0x65
	{ "BIT 4, (HL)", NULL }, // 0x66
	{ "BIT 4, A", NULL },    // 0x67
	{ "BIT 5, B", NULL },    // 0x68
	{ "BIT 5, C", NULL },    // 0x69
	{ "BIT 5, D", NULL },    // 0x6a
	{ "BIT 5, E", NULL },    // 0x6b
	{ "BIT 6, H", NULL },    // 0x6c
	{ "BIT 6, L", NULL },    // 0x6d
	{ "BIT 5, (HL)", NULL }, // 0x6e
	{ "BIT 5, A", bit_5_a }, // 0x6f
	{ "BIT 6, B", NULL },    // 0x70
	{ "BIT 6, C", NULL },    // 0x71
	{ "BIT 6, D", NULL },    // 0x72
	{ "BIT 6, E", NULL },    // 0x73
	{ "BIT 6, H", NULL },    // 0x74
	{ "BIT 6, L", NULL },    // 0x75
	{ "BIT 6, (HL)", NULL }, // 0x76
	{ "BIT 6, A", bit_6_a }, // 0x77
	{ "BIT 7, B", NULL },    // 0x78
	{ "BIT 7, C", NULL },    // 0x79
	{ "BIT 7, D", NULL },    // 0x7a
	{ "BIT 7, E", NULL },    // 0x7b
	{ "BIT 7, H", NULL },    // 0x7c
	{ "BIT 7, L", NULL },    // 0x7d
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
	{ "SET 7, B", NULL },    // 0xf8
	{ "SET 7, C", NULL },    // 0xf9
	{ "SET 7, D", NULL },    // 0xfa
	{ "SET 7, E", NULL },    // 0xfb
	{ "SET 7, H", NULL },    // 0xfc
	{ "SET 7, L", NULL },    // 0xfd
	{ "SET 7, (HL)", NULL }, // 0xfe
	{ "SET 7, A", NULL },    // 0xff
};

const unsigned char extendedInstructionTicks[256] = {
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0x0_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0x1_
	0, 0, 0, 0, 0, 0,  0, 8,  0, 0, 0, 0, 0, 0,  0, 0, // 0x2_
	0, 0, 0, 8, 0, 0,  0, 8,  0, 0, 0, 0, 0, 0,  0, 8, // 0x3_
	8, 0, 0, 0, 0, 0,  0, 0,  8, 0, 0, 0, 0, 0,  0, 0, // 0x4_
	8, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 8, // 0x5_
	8, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 8, // 0x6_
	0, 0, 0, 0, 0, 0,  0, 8,  0, 0, 0, 0, 0, 0, 12, 8, // 0x7_
	0, 0, 0, 0, 0, 0, 12, 8,  0, 0, 0, 0, 0, 0,  0, 0, // 0x8_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0x9_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xa_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xb_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xc_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xd_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0, // 0xe_
	0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0,  0, 0  // 0xf_
};

void cb_n(unsigned char instruction) {
	if(!extendedInstructions[instruction].execute) {
		printf("Unimplemented extended instruction 0x%02x (%s)!\n",  instruction, extendedInstructions[instruction].disassembly);
		
		char d[100];
		sprintf(d, "Unimplemented extended instruction 0x%02x!\n\nCheck stdout for more details.", instruction);
		MessageBox(NULL, d, "Cinoop", MB_OK);
		
		registers.pc -= 2;
		printRegisters();
		exit(1);
	}
	
	extendedInstructions[instruction].execute();
	
	ticks += extendedInstructionTicks[instruction];
}

static void swap(unsigned char *destination) {
	*destination = ((*destination & 0xf) << 4) | ((*destination & 0xf0) >> 4);
	
	if(*destination) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY | FLAGS_CARRY);
}

static void bit(unsigned char bit, unsigned char value) {
	if(value & bit) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

// 0x27
void sla_a(void) {
	if(registers.a & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	registers.a <<= 1;
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0x33
void swap_e(void) { swap(&registers.e); }

// 0x37
void swap_a(void) { swap(&registers.a); }

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

// 0x48
void bit_1_b(void) { bit(1 << 1, registers.b); }

// 0x50
void bit_2_b(void) { bit(1 << 2, registers.b); }

// 0x5f
void bit_3_a(void) { bit(1 << 3, registers.a); }

// 0x60
void bit_4_b(void) { bit(1 << 4, registers.b); }

// 0x6f
void bit_5_a(void) { bit(1 << 5, registers.a); }

// 0x77
void bit_6_a(void) { bit(1 << 6, registers.a); }

// 0x7e
void bit_7_hlp(void) { bit(1 << 7, readByte(registers.hl)); }

// 0x7f
void bit_7_a(void) { bit(1 << 7, registers.a); }

// 0x86
void res_0_hlp(void) { writeByte(registers.hl, readByte(registers.hl) & ~0x01); }

// 0x87
void res_0_a(void) { registers.a &= ~0x01; }
