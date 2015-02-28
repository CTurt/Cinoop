#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "registers.h"
#include "memory.h"

#include "cpu.h"

// http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// http://imrannazar.com/Gameboy-Z80-Opcode-Map
const struct instruction instructions[256] = {
	{ "NOP", 0, nop },									    // 0x00
	{ "LD BC, 0x%04X", 2, NULL },					        // 0x01
	{ "LD (BC), A", 0, NULL },				                // 0x02
	{ "INC BC", 0, NULL },							        // 0x03
	{ "INC B", 0, NULL },								    // 0x04
	{ "DEC B", 0, NULL },								    // 0x05
	{ "LD B, 0x%02X", 1, ld_b_n },						    // 0x06
	{ "RLCA", 0, NULL },								    // 0x07
	{ "LD (0x%04X), SP", 2, NULL },			                // 0x08
	{ "ADD HL, BC", 0, NULL },						        // 0x09
	{ "LD A, (BC)", 0, NULL },				                // 0x0a
	{ "DEC BC", 0, NULL },							        // 0x0b
	{ "INC C", 0, NULL },								    // 0x0c
	{ "DEC C", 0, NULL },								    // 0x0d
	{ "LD C, 0x%02X", 1, ld_c_n },						    // 0x0e
	{ "RRCA", 0, NULL },								    // 0x0f
	{ "STOP", 1, NULL },								    // 0x10
	{ "LD DE, 0x%04X", 2, NULL },					        // 0x11
	{ "LD (DE), A", 0, NULL },				                // 0x12
	{ "INC DE", 0, NULL },							        // 0x13
	{ "INC D", 0, NULL },								    // 0x14
	{ "DEC D", 0, NULL },								    // 0x15
	{ "LD D, 0x%02X", 1, NULL },						    // 0x16
	{ "RLA", 0, NULL },									    // 0x17
	{ "JR 0x%02X", 1, NULL },							    // 0x18
	{ "ADD HL, DE", 0, NULL },						        // 0x19
	{ "LD A,(DE)", 0, NULL },				                // 0x1a
	{ "DEC DE", 0, NULL },							        // 0x1b
	{ "INC E", 0, NULL },								    // 0x1c
	{ "DEC E", 0, NULL },								    // 0x1d
	{ "LD E, 0x%02X", 1, NULL },						    // 0x1e
	{ "RRA", 0, NULL },									    // 0x1f
	{ "JR NZ, 0x%02X", 1, NULL },						    // 0x20
	{ "LD HL, 0x%04X", 2, ld_hl_nn },				        // 0x21
	{ "LD (HL+), A", 0, NULL },			                    // 0x22
	{ "INC HL", 0, NULL },							        // 0x23
	{ "INC H", 0, NULL },								    // 0x24
	{ "DEC H", 0, NULL },								    // 0x25
	{ "LD H, 0x%02X", 1, NULL },						    // 0x26
	{ "DAA", 0, NULL },									    // 0x27
	{ "JR Z,0x%02X", 1, NULL },						        // 0x28
	{ "ADD HL, HL", 0, NULL },						        // 0x29
	{ "LD A, (HL+)", 0, NULL },			                    // 0x2a
	{ "DEC HL", 0, NULL },							        // 0x2b
	{ "INC L", 0, NULL },								    // 0x2c
	{ "DEC L", 0, NULL },								    // 0x2d
	{ "LD L, 0x%02X", 1, NULL },						    // 0x2e
	{ "CPL", 0, NULL },									    // 0x2f
	{ "JR NC, 0x%02X", 1, NULL },						    // 0x30
	{ "LD SP, 0x%04X", 2, NULL },					        // 0x31
	{ "LD (HL-), A", 0, NULL },			                    // 0x32
	{ "INC SP", 0, NULL },							        // 0x33
	{ "INC (HL)", 0, NULL },					            // 0x34
	{ "DEC (HL)", 0, NULL },					            // 0x35
	{ "LD (HL), 0x%02X", 1, NULL },			                // 0x36
	{ "SCF", 0, NULL },									    // 0x37
	{ "JR C,0x%02X", 1, NULL },						        // 0x38
	{ "ADD HL, SP", 0, NULL },						        // 0x39
	{ "LD A, (HL-)", 0, NULL },			                    // 0x3a
	{ "DEC SP", 0, NULL },							        // 0x3b
	{ "INC A", 0, inc_a },								    // 0x3c
	{ "DEC A", 0, NULL },								    // 0x3d
	{ "LD A, 0x%02X", 1, NULL },						    // 0x3e
	{ "CCF", 0, NULL },									    // 0x3f
	{ "LD B, B", 0, NULL },								    // 0x40
	{ "LD B, C", 0, NULL },							        // 0x41
	{ "LD B, D", 0, NULL },							        // 0x42
	{ "LD B, E", 0, ld_b_e },						        // 0x43
	{ "LD B, H", 0, NULL },							        // 0x44
	{ "LD B, L", 0, NULL },							        // 0x45
	{ "LD B, (HL)", 0, NULL },				                // 0x46
	{ "LD B, A", 0, NULL },							        // 0x47
	{ "LD C, B", 0, NULL },							        // 0x48
	{ "LD C, C", 0, NULL },								    // 0x49
	{ "LD C, D", 0, NULL },							        // 0x4a
	{ "LD C, E", 0, NULL },							        // 0x4b
	{ "LD C, H", 0, NULL },							        // 0x4c
	{ "LD C, L", 0, NULL },							        // 0x4d
	{ "LD C, (HL)", 0, NULL },				                // 0x4e
	{ "LD C, A", 0, NULL },							        // 0x4f
	{ "LD D, B", 0, NULL },							        // 0x50
	{ "LD D, C", 0, NULL },							        // 0x51
	{ "LD D, D", 0, NULL },								    // 0x52
	{ "LD D, E", 0, NULL },							        // 0x53
	{ "LD D, H", 0, NULL },							        // 0x54
	{ "LD D, L", 0, NULL },							        // 0x55
	{ "LD D, (HL)", 0, NULL },				                // 0x56
	{ "LD D, A", 0, NULL },							        // 0x57
	{ "LD E, B", 0, NULL },							        // 0x58
	{ "LD E, C", 0, NULL },							        // 0x59
	{ "LD E, D", 0, NULL },							        // 0x5a
	{ "LD E, E", 0, NULL },								    // 0x5b
	{ "LD E, H", 0, NULL },							        // 0x5c
	{ "LD E, L", 0, NULL },							        // 0x5d
	{ "LD E, (HL)", 0, NULL },				                // 0x5e
	{ "LD E, A", 0, NULL },							        // 0x5f
	{ "LD H, B", 0, NULL },							        // 0x60
	{ "LD H, C", 0, NULL },							        // 0x61
	{ "LD H, D", 0, NULL },							        // 0x62
	{ "LD H, E", 0, NULL },							        // 0x63
	{ "LD H, H", 0, NULL },								    // 0x64
	{ "LD H, L", 0, NULL },							        // 0x65
	{ "LD H, (HL)", 0, NULL },				                // 0x66
	{ "LD H, A", 0, NULL },							        // 0x67
	{ "LD L, B", 0, NULL },							        // 0x68
	{ "LD L, C", 0, NULL },							        // 0x69
	{ "LD L, D", 0, NULL },							        // 0x6a
	{ "LD L, E", 0, NULL },							        // 0x6b
	{ "LD L, H", 0, NULL },							        // 0x6c
	{ "LD L, L", 0, NULL },								    // 0x6d
	{ "LD L, (HL)", 0, NULL },				                // 0x6e
	{ "LD L, A", 0, NULL },							        // 0x6f
	{ "LD (HL), B", 0, NULL },				                // 0x70
	{ "LD (HL), C", 0, NULL },				                // 0x71
	{ "LD (HL), D", 0, NULL },				                // 0x72
	{ "LD (HL), E", 0, NULL },				                // 0x73
	{ "LD (HL), H", 0, NULL },				                // 0x74
	{ "LD (HL), L", 0, NULL },				                // 0x75
	{ "HALT", 0, NULL },								    // 0x76
	{ "LD (HL), A", 0, NULL },				                // 0x77
	{ "LD A, B", 0, NULL },							        // 0x78
	{ "LD A, C", 0, NULL },							        // 0x79
	{ "LD A, D", 0, NULL },							        // 0x7a
	{ "LD A, E", 0, NULL },							        // 0x7b
	{ "LD A, H", 0, NULL },							        // 0x7c
	{ "LD A, L", 0, NULL },							        // 0x7d
	{ "LD A, (HL)", 0, NULL },				                // 0x7e
	{ "LD A, A", 0, NULL },								    // 0x7f
	{ "ADD A, B", 0, NULL },							    // 0x80
	{ "ADD A, C", 0, NULL },							    // 0x81
	{ "ADD A, D", 0, NULL },							    // 0x82
	{ "ADD A, E", 0, NULL },							    // 0x83
	{ "ADD A, H", 0, NULL },							    // 0x84
	{ "ADD A, L", 0, NULL },							    // 0x85
	{ "ADD A, (HL)", 0, NULL },			                    // 0x86
	{ "ADD A", 0, NULL },							        // 0x87
	{ "ADC A, B", 0, NULL },							    // 0x88
	{ "ADC A, C", 0, NULL },							    // 0x89
	{ "ADC A, D", 0, NULL },							    // 0x8a
	{ "ADC A, E", 0, NULL },							    // 0x8b
	{ "ADC A, H", 0, NULL },							    // 0x8c
	{ "ADC A, L", 0, NULL },							    // 0x8d
	{ "ADC A, (HL)", 0, NULL },			                    // 0x8e
	{ "ADC A, A", 0, NULL },							    // 0x8f
	{ "SUB B", 0, NULL },								    // 0x90
	{ "SUB C", 0, NULL },								    // 0x91
	{ "SUB D", 0, NULL },								    // 0x92
	{ "SUB E", 0, NULL },								    // 0x93
	{ "SUB H", 0, NULL },								    // 0x94
	{ "SUB L", 0, NULL },								    // 0x95
	{ "SUB (HL)", 0, NULL },					            // 0x96
	{ "SUB A", 0, NULL },								    // 0x97
	{ "SBC A, B", 0, NULL },							    // 0x98
	{ "SBC A, C", 0, NULL },							    // 0x99
	{ "SBC A, D", 0, NULL },							    // 0x9a
	{ "SBC A, E", 0, NULL },							    // 0x9b
	{ "SBC A, H", 0, NULL },							    // 0x9c
	{ "SBC A, L", 0, NULL },							    // 0x9d
	{ "SBC A, (HL)", 0, NULL },				                // 0x9e
	{ "SBC A, A", 0, NULL },							    // 0x9f
	{ "AND B", 0, NULL },								    // 0xa0
	{ "AND C", 0, NULL },								    // 0xa1
	{ "AND D", 0, NULL },								    // 0xa2
	{ "AND E", 0, NULL },								    // 0xa3
	{ "AND H", 0, NULL },								    // 0xa4
	{ "AND L", 0, NULL },								    // 0xa5
	{ "AND (HL)", 0, NULL },					            // 0xa6
	{ "AND A", 0, NULL },								    // 0xa7
	{ "XOR B", 0, NULL },								    // 0xa8
	{ "XOR C", 0, NULL },								    // 0xa9
	{ "XOR D", 0, NULL },								    // 0xaa
	{ "XOR E", 0, NULL },								    // 0xab
	{ "XOR H", 0, NULL },								    // 0xac
	{ "XOR L", 0, NULL },								    // 0xad
	{ "XOR (HL)", 0, NULL },					            // 0xae
	{ "XOR A", 0, xor_a },								    // 0xaf
	{ "OR B", 0, NULL },								    // 0xb0
	{ "OR C", 0, NULL },								    // 0xb1
	{ "OR D", 0, NULL },								    // 0xb2
	{ "OR E", 0, NULL },								    // 0xb3
	{ "OR H", 0, NULL },								    // 0xb4
	{ "OR L", 0, NULL },								    // 0xb5
	{ "OR (HL)", 0, NULL },					                // 0xb6
	{ "OR A", 0, NULL },								    // 0xb7
	{ "CP B", 0, NULL },								    // 0xb8
	{ "CP C", 0, NULL },								    // 0xb9
	{ "CP D", 0, NULL },								    // 0xba
	{ "CP E", 0, NULL },								    // 0xbb
	{ "CP H", 0, NULL },								    // 0xbc
	{ "CP L", 0, NULL },								    // 0xbd
	{ "CP (HL)", 0, NULL },					                // 0xbe
	{ "CP A", 0, NULL },								    // 0xbf
	{ "RET NZ", 0, NULL },							        // 0xc0
	{ "POP BC", 0, NULL },							        // 0xc1
	{ "JP NZ, 0x%04X", 2, NULL },					        // 0xc2
	{ "JP 0x%04X", 2, jp_nn },							    // 0xc3
	{ "CALL NZ, 0x%04X", 2, NULL },					        // 0xc4
	{ "PUSH BC", 0, NULL },							        // 0xc5
	{ "ADD A, 0x%02X", 1, NULL },						    // 0xc6
	{ "RST 0x00", 0, NULL },							    // 0xc7
	{ "RET Z", 0, NULL },								    // 0xc8
	{ "RET", 0, NULL },									    // 0xc9
	{ "JP Z,0x%04X", 2, NULL },						        // 0xca
	{ "CB%02X", 1, NULL },								    // 0xcb
	{ "CALL Z, 0x%04X", 2, NULL },					        // 0xcc
	{ "CALL 0x%04X", 2, NULL },						        // 0xcd
	{ "ADC A, 0x%02X", 1, NULL },						    // 0xce
	{ "RST 0x08", 0, NULL },							    // 0xcf
	{ "RET NC", 0, NULL },							        // 0xd0
	{ "POP DE", 0, NULL },							        // 0xd1
	{ "JP NC, 0x%04X", 2, NULL },					        // 0xd2
	{ "UNKNOWN", 0, NULL },							        // 0xd3
	{ "CALL NC, 0x%04X", 2, NULL },				            // 0xd4
	{ "PUSH DE", 0, NULL },							        // 0xd5
	{ "SUB 0x%02X", 1, NULL },							    // 0xd6
	{ "RST 0x10", 0, NULL },							    // 0xd7
	{ "RET C", 0, NULL },								    // 0xd8
	{ "RETI", 0, NULL },								    // 0xd9
	{ "JP C, 0x%04X", 2, NULL },						    // 0xda
	{ "UNKNOWN", 0, NULL },							        // 0xdb
	{ "CALL C, 0x%04X", 2, NULL },					        // 0xdc
	{ "UNKNOWN", 0, NULL },							        // 0xdd
	{ "SBC A, 0x%02X", 1, NULL },						    // 0xde
	{ "RST 0x18", 0, NULL },							    // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1, NULL },	                // 0xe0
	{ "POP HL", 0, NULL },							        // 0xe1
	{ "LD (0xFF00 + C), A", 0, NULL },		                // 0xe2
	{ "UNKNOWN", 0, NULL },							        // 0xe3
	{ "UNKNOWN", 0, NULL },							        // 0xe4
	{ "PUSH HL", 0, NULL },							        // 0xe5
	{ "AND 0x%02X", 1, NULL },							    // 0xe6
	{ "RST 0x20", 0, NULL },							    // 0xe7
	{ "ADD SP,0x%02X", 1, NULL },					        // 0xe8
	{ "JP HL", 0, NULL },								    // 0xe9
	{ "LD (0x%04X), A", 2, NULL },			                // 0xea
	{ "UNKNOWN", 0, NULL },							        // 0xeb
	{ "UNKNOWN", 0, NULL },							        // 0xec
	{ "UNKNOWN", 0, NULL },							        // 0xed
	{ "XOR 0x%02X", 1, NULL },							    // 0xee
	{ "RST 0x28", 0, NULL },							    // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1, NULL },	                // 0xf0
	{ "POP AF", 0, NULL },							        // 0xf1
	{ "LD A, (0xFF00 + C)", 0, NULL },		                // 0xf2
	{ "DI", 0, NULL },									    // 0xf3
	{ "UNKNOWN", 0, NULL },							        // 0xf4
	{ "PUSH AF", 0, NULL },							        // 0xf5
	{ "OR 0x%02X", 1, NULL },							    // 0xf6
	{ "RST 0x30", 0, NULL },							    // 0xf7
	{ "LD HL, SP+0x%02X", 1, NULL },				        // 0xf8
	{ "LD SP, HL", 0, NULL },						        // 0xf9
	{ "LD A, (0x%04X)", 2, NULL },			                // 0xfa
	{ "EI", 0, NULL },									    // 0xfb
	{ "UNKNOWN", 0, NULL },							        // 0xfc
	{ "UNKNOWN", 0, NULL },							        // 0xfd
	{ "CP 0x%02X", 1, NULL },							    // 0xfe
	{ "RST 0x38", 0, NULL },							    // 0xff
};

void reset(void) {
	registers.a = 0x01;
	registers.f = 0xb0;
	registers.b = 0x00;
	registers.c = 0x13;
	registers.d = 0x00;
	registers.e = 0xd8;
	registers.h = 0x01;
	registers.l = 0x4d;
	registers.sp = 0xfffe;
	registers.pc = 0x100;
	registers.ime = 0x00;
}

void cpuStep(void) {
	unsigned char instruction;
	unsigned short operand = 0;
	
	instruction = readByte(registers.pc++);
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(registers.pc);
	if(instructions[instruction].operandLength == 2) operand = readShort(registers.pc);
	
	registers.pc += instructions[instruction].operandLength;
	
	if(!instructions[instruction].execute) {
		printf("Unimplemented instruction 0x%02x (",  instruction);
		if(instructions[instruction].operandLength) printf(instructions[instruction].disassembly, operand);
		else printf(instructions[instruction].disassembly);
		printf(")!\n");
		
		registers.pc--;
		printRegisters();
		exit(1);
	}
	
	switch(instructions[instruction].operandLength) {
		case 0:
			((void (*)(void))instructions[instruction].execute)();
			break;
		
		case 1:
			((void (*)(unsigned char))instructions[instruction].execute)((unsigned char)operand);
			break;
		
		case 2:
			((void (*)(unsigned short))instructions[instruction].execute)(operand);
			break;
	}
	
	// ticks += instructionTicks[instruction];
}

// 0x00
void nop(void) {  }

// 0x06
void ld_b_n(unsigned char operand) { registers.b = operand; }

// 0x0e
void ld_c_n(unsigned char operand) { registers.c = operand; }

// 0x21
void ld_hl_nn(unsigned short operand) { registers.hl = operand; }

// 0x3c
void inc_a(void) { registers.a++; }

// 0x43
void ld_b_e(void) { registers.b = registers.e; }

// 0xaf
void xor_a(void) { registers.a = 0; FLAGS_SET(FLAGS_ZERO); FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0xc3
void jp_nn(unsigned short operand) { registers.pc = operand; }
