#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "registers.h"
#include "memory.h"
#include "interrupts.h"
#include "gpu.h"
#include "opengl.h"

#include "cpu.h"

/*
References:

Opcode disassemblies:
http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
http://imrannazar.com/Gameboy-Z80-Opcode-Map
GBE source

Which instructions modify flags:
http://gameboy.mongenel.com/dmg/opcodes.html

Instruction implementation:
GBE source

Testing:
NO$GMB
*/

const struct instruction instructions[256] = {
	{ "NOP", 0, nop },									    // 0x00
	{ "LD BC, 0x%04X", 2, ld_bc_nn },				        // 0x01
	{ "LD (BC), A", 0, NULL },				                // 0x02
	{ "INC BC", 0, NULL },							        // 0x03
	{ "INC B", 0, NULL },								    // 0x04
	{ "DEC B", 0, dec_b },								    // 0x05
	{ "LD B, 0x%02X", 1, ld_b_n },						    // 0x06
	{ "RLCA", 0, NULL },								    // 0x07
	{ "LD (0x%04X), SP", 2, NULL },			                // 0x08
	{ "ADD HL, BC", 0, NULL },						        // 0x09
	{ "LD A, (BC)", 0, NULL },				                // 0x0a
	{ "DEC BC", 0, dec_bc },						        // 0x0b
	{ "INC C", 0, inc_c },								    // 0x0c
	{ "DEC C", 0, dec_c },								    // 0x0d
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
	{ "DEC DE", 0, dec_de },						        // 0x1b
	{ "INC E", 0, NULL },								    // 0x1c
	{ "DEC E", 0, NULL },								    // 0x1d
	{ "LD E, 0x%02X", 1, NULL },						    // 0x1e
	{ "RRA", 0, NULL },									    // 0x1f
	{ "JR NZ, 0x%02X", 1, jr_nz_n },					    // 0x20
	{ "LD HL, 0x%04X", 2, ld_hl_nn },				        // 0x21
	{ "LD (HL+), A", 0, NULL },			                    // 0x22
	{ "INC HL", 0, NULL },							        // 0x23
	{ "INC H", 0, NULL },								    // 0x24
	{ "DEC H", 0, NULL },								    // 0x25
	{ "LD H, 0x%02X", 1, NULL },						    // 0x26
	{ "DAA", 0, NULL },									    // 0x27
	{ "JR Z, 0x%02X", 1, jr_z_n },					        // 0x28
	{ "ADD HL, HL", 0, NULL },						        // 0x29
	{ "LDI A, (HL)", 0, ldi_a_hlp },	                    // 0x2a
	{ "DEC HL", 0, NULL },							        // 0x2b
	{ "INC L", 0, NULL },								    // 0x2c
	{ "DEC L", 0, NULL },								    // 0x2d
	{ "LD L, 0x%02X", 1, NULL },						    // 0x2e
	{ "CPL", 0, NULL },									    // 0x2f
	{ "JR NC, 0x%02X", 1, NULL },						    // 0x30
	{ "LD SP, 0x%04X", 2, ld_sp_nn },				        // 0x31
	{ "LDD (HL), A", 0, ldd_hlp_a },		                // 0x32
	{ "INC SP", 0, NULL },							        // 0x33
	{ "INC (HL)", 0, NULL },					            // 0x34
	{ "DEC (HL)", 0, NULL },					            // 0x35
	{ "LD (HL), 0x%02X", 1, ld_hlp_n },			            // 0x36
	{ "SCF", 0, NULL },									    // 0x37
	{ "JR C, 0x%02X", 1, jr_c_n },					        // 0x38
	{ "ADD HL, SP", 0, NULL },						        // 0x39
	{ "LD A, (HL-)", 0, NULL },			                    // 0x3a
	{ "DEC SP", 0, NULL },							        // 0x3b
	{ "INC A", 0, inc_a },								    // 0x3c
	{ "DEC A", 0, NULL },								    // 0x3d
	{ "LD A, 0x%02X", 1, ld_a_n },						    // 0x3e
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
	{ "LD A, B", 0, ld_a_b },						        // 0x78
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
	{ "AND A", 0, and_a },								    // 0xa7
	{ "XOR B", 0, NULL },								    // 0xa8
	{ "XOR C", 0, NULL },								    // 0xa9
	{ "XOR D", 0, NULL },								    // 0xaa
	{ "XOR E", 0, NULL },								    // 0xab
	{ "XOR H", 0, NULL },								    // 0xac
	{ "XOR L", 0, NULL },								    // 0xad
	{ "XOR (HL)", 0, NULL },					            // 0xae
	{ "XOR A", 0, xor_a },								    // 0xaf
	{ "OR B", 0, NULL },								    // 0xb0
	{ "OR C", 0, or_c },								    // 0xb1
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
	{ "RET NZ", 0, ret_nz },						        // 0xc0
	{ "POP BC", 0, NULL },							        // 0xc1
	{ "JP NZ, 0x%04X", 2, NULL },					        // 0xc2
	{ "JP 0x%04X", 2, jp_nn },							    // 0xc3
	{ "CALL NZ, 0x%04X", 2, NULL },					        // 0xc4
	{ "PUSH BC", 0, push_bc },						        // 0xc5
	{ "ADD A, 0x%02X", 1, NULL },						    // 0xc6
	{ "RST 0x00", 0, NULL },							    // 0xc7
	{ "RET Z", 0, NULL },								    // 0xc8
	{ "RET", 0, ret },									    // 0xc9
	{ "JP Z,0x%04X", 2, NULL },						        // 0xca
	{ "CB%02X", 1, NULL },								    // 0xcb
	{ "CALL Z, 0x%04X", 2, NULL },					        // 0xcc
	{ "CALL 0x%04X", 2, call_nn },					        // 0xcd
	{ "ADC A, 0x%02X", 1, NULL },						    // 0xce
	{ "RST 0x08", 0, NULL },							    // 0xcf
	{ "RET NC", 0, NULL },							        // 0xd0
	{ "POP DE", 0, NULL },							        // 0xd1
	{ "JP NC, 0x%04X", 2, NULL },					        // 0xd2
	{ "UNKNOWN", 0, NULL },							        // 0xd3
	{ "CALL NC, 0x%04X", 2, NULL },				            // 0xd4
	{ "PUSH DE", 0, push_de },						        // 0xd5
	{ "SUB 0x%02X", 1, NULL },							    // 0xd6
	{ "RST 0x10", 0, NULL },							    // 0xd7
	{ "RET C", 0, NULL },								    // 0xd8
	{ "RETI", 0, returnFromInterrupt },					    // 0xd9
	{ "JP C, 0x%04X", 2, NULL },						    // 0xda
	{ "UNKNOWN", 0, NULL },							        // 0xdb
	{ "CALL C, 0x%04X", 2, NULL },					        // 0xdc
	{ "UNKNOWN", 0, NULL },							        // 0xdd
	{ "SBC A, 0x%02X", 1, NULL },						    // 0xde
	{ "RST 0x18", 0, NULL },							    // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1, ld_ff_n_ap },	        // 0xe0
	{ "POP HL", 0, NULL },							        // 0xe1
	{ "LD (0xFF00 + C), A", 0, ld_ff_c_a },	                // 0xe2
	{ "UNKNOWN", 0, NULL },							        // 0xe3
	{ "UNKNOWN", 0, NULL },							        // 0xe4
	{ "PUSH HL", 0, push_hl },						        // 0xe5
	{ "AND 0x%02X", 1, NULL },							    // 0xe6
	{ "RST 0x20", 0, NULL },							    // 0xe7
	{ "ADD SP,0x%02X", 1, NULL },					        // 0xe8
	{ "JP HL", 0, NULL },								    // 0xe9
	{ "LD (0x%04X), A", 2, ld_nnp_a },			            // 0xea
	{ "UNKNOWN", 0, NULL },							        // 0xeb
	{ "UNKNOWN", 0, NULL },							        // 0xec
	{ "UNKNOWN", 0, NULL },							        // 0xed
	{ "XOR 0x%02X", 1, NULL },							    // 0xee
	{ "RST 0x28", 0, NULL },							    // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1, ld_ff_ap_n },	        // 0xf0
	{ "POP AF", 0, NULL },							        // 0xf1
	{ "LD A, (0xFF00 + C)", 0, NULL },		                // 0xf2
	{ "DI", 0, di },									    // 0xf3
	{ "UNKNOWN", 0, NULL },							        // 0xf4
	{ "PUSH AF", 0, push_af },						        // 0xf5
	{ "OR 0x%02X", 1, NULL },							    // 0xf6
	{ "RST 0x30", 0, NULL },							    // 0xf7
	{ "LD HL, SP+0x%02X", 1, NULL },				        // 0xf8
	{ "LD SP, HL", 0, NULL },						        // 0xf9
	{ "LD A, (0x%04X)", 2, NULL },			                // 0xfa
	{ "EI", 0, ei },									    // 0xfb
	{ "UNKNOWN", 0, NULL },							        // 0xfc
	{ "UNKNOWN", 0, NULL },							        // 0xfd
	{ "CP 0x%02X", 1, cp_n },							    // 0xfe
	{ "RST 0x38", 0, NULL },							    // 0xff
};

const unsigned char instructionTicks[256] = {
	2, 6, 4, 4, 2, 2, 4, 4, 10, 4, 4, 4, 2, 2, 4, 4, // 0x0_
	2, 6, 4, 4, 2, 2, 4, 4,  4, 4, 4, 4, 2, 2, 4, 4, // 0x1_
	0, 6, 4, 4, 2, 2, 4, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x2_
	4, 6, 4, 4, 6, 6, 6, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x3_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x4_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x5_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x6_
	4, 4, 4, 4, 4, 4, 2, 4,  2, 2, 2, 2, 2, 2, 4, 2, // 0x7_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x8_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x9_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xa_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xb_
	0, 6, 6, 6, 6, 8, 4, 8,  4, 2, 6, 0, 6, 6, 4, 8, // 0xc_
	4, 6, 6, 0, 6, 8, 4, 8,  4, 8, 6, 0, 6, 0, 4, 8, // 0xd_
	6, 6, 4, 0, 0, 8, 4, 8,  8, 2, 8, 0, 0, 0, 4, 8, // 0xe_
	6, 6, 4, 2, 0, 8, 4, 8,  6, 4, 8, 2, 0, 0, 4, 8  // 0xf_
};

unsigned long ticks;

void reset(void) {
	memset(sram, 0, sizeof(sram));
	memset(io, 0, sizeof(io));
	memset(vram, 0, sizeof(vram));
	memset(oam, 0, sizeof(oam));
	memset(wram, 0, sizeof(wram));
	memset(hram, 0, sizeof(hram));
	
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
	
	interrupt.master = 1;
	interrupt.enable = 0;
	interrupt.flags = 0;
	
	memset(tiles, 0, sizeof(tiles));
	
	gpu.control = 0;
	gpu.scrollX = 0;
	gpu.scrollY = 0;
	gpu.scanline = 0;
	gpu.bgPalette = 0;
	gpu.tick = 0;
	
	ticks = 0;
	
	memset(framebuffer, 255, sizeof(framebuffer));
	
	writeByte(0xFF05, 0);
	writeByte(0xFF06, 0);
	writeByte(0xFF07, 0);
	writeByte(0xFF10, 0x80);
	writeByte(0xFF11, 0xBF);
	writeByte(0xFF12, 0xF3);
	writeByte(0xFF14, 0xBF);
	writeByte(0xFF16, 0x3F);
	writeByte(0xFF17, 0x00);
	writeByte(0xFF19, 0xBF);
	writeByte(0xFF1A, 0x7A);
	writeByte(0xFF1B, 0xFF);
	writeByte(0xFF1C, 0x9F);
	writeByte(0xFF1E, 0xBF);
	writeByte(0xFF20, 0xFF);
	writeByte(0xFF21, 0x00);
	writeByte(0xFF22, 0x00);
	writeByte(0xFF23, 0xBF);
	writeByte(0xFF24, 0x77);
	writeByte(0xFF25, 0xF3);
	writeByte(0xFF26, 0xF1);
	writeByte(0xFF40, 0x91);
	writeByte(0xFF42, 0x00);
	writeByte(0xFF43, 0x00);
	writeByte(0xFF45, 0x00);
	writeByte(0xFF47, 0xFC);
	writeByte(0xFF48, 0xFF);
	writeByte(0xFF49, 0xFF);
	writeByte(0xFF4A, 0x00);
	writeByte(0xFF4B, 0x00);
	writeByte(0xFFFF, 0x00);
}

void cpuStep(void) {
	unsigned char instruction;
	unsigned short operand = 0;
	
	// General breakpoints
	if(registers.pc == 0x2817) {
		realtimeDebugEnable = 1;
	}
	
	if(realtimeDebugEnable) realtimeDebug();
	
	instruction = readByte(registers.pc++);
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(registers.pc);
	if(instructions[instruction].operandLength == 2) operand = readShort(registers.pc);
	registers.pc += instructions[instruction].operandLength;
	
	if(!instructions[instruction].execute) {
		printf("Unimplemented instruction 0x%02x (",  instruction);
		if(instructions[instruction].operandLength) printf(instructions[instruction].disassembly, operand);
		else printf(instructions[instruction].disassembly);
		printf(")!\n");
		
		registers.pc -= instructions[instruction].operandLength + 1;
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
	
	ticks += instructionTicks[instruction];
	
	//int i;
	//for(i = 0; i < 40; i++) {
		//if(((struct sprite *)(oam + i * 4))->y == 80) {
		//	printf("s %d\n", i);
		//}
	//}
	
	gpuStep();
	
	if(interrupt.master && interrupt.enable && interrupt.flags) {
		if(interrupt.enable & interrupt.flags & INTERRUPTS_VBLANK) {
			interrupt.flags &= 255 - 0x01;
			vblank();
		}
	}
}

// 0x00
void nop(void) {  }

// 0x01
void ld_bc_nn(unsigned short operand) { registers.bc = operand; }

// 0x05
void dec_b(void) {
	if(registers.b & 0x0f) FLAGS_CLEAR(FLAGS_HALFCARRY);
	else FLAGS_SET(FLAGS_HALFCARRY);
	
	registers.b--;
	
	if(registers.b) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_SET(FLAGS_NEGATIVE);
}

// 0x06
void ld_b_n(unsigned char operand) { registers.b = operand; }

// 0x0b
void dec_bc(void) { registers.bc--; }

// 0x0c
void inc_c(void) {
	if((registers.c & 0x0f) == 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	registers.c++;
	
	if(registers.c) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

// 0x0d
void dec_c(void) {
	if(registers.c & 0x0f) FLAGS_CLEAR(FLAGS_HALFCARRY);
	else FLAGS_SET(FLAGS_HALFCARRY);
	
	registers.c--;
	
	if(registers.c) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_SET(FLAGS_NEGATIVE);
}

// 0x0e
void ld_c_n(unsigned char operand) { registers.c = operand; }

// 0x1b
void dec_de(void) { registers.de--; }

// 0x20
void jr_nz_n(char operand) {
	if(FLAGS_ISZERO) ticks += 8;
	else {
		registers.pc += operand;
		debugJump();
		ticks += 12;
	}
}

// 0x21
void ld_hl_nn(unsigned short operand) { registers.hl = operand; }

// 0x28
void jr_z_n(char operand) {
	if(FLAGS_ISZERO) {
		registers.pc += operand;
		debugJump();
		ticks += 12;
	}
	else ticks += 8;
}

// 0x2a
void ldi_a_hlp(void) { registers.a = readByte(registers.hl++); }

// 0x31
void ld_sp_nn(unsigned short operand) { registers.sp = operand; }

// 0x32
void ldd_hlp_a(void) { writeByte(registers.hl, registers.a); registers.hl--; }

// 0x36
void ld_hlp_n(unsigned char operand) { writeByte(registers.hl, operand); }

// 0x38
void jr_c_n(char operand) {
	if(FLAGS_ISCARRY) {
		registers.pc += operand;
		ticks += 12;
	}
	else ticks += 8;
}

// 0x3c
void inc_a(void) {
	if((registers.a & 0x0f) == 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	registers.a++;
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

// 0x3e
void ld_a_n(unsigned char operand) { registers.a = operand; }

// 0x43
void ld_b_e(void) { registers.b = registers.e; }

// 0x78
void ld_a_b(void) { registers.a = registers.b; }

// 0xa7
void and_a(void) {
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
}

// 0xaf
void xor_a(void) { registers.a = 0; FLAGS_SET(FLAGS_ZERO); FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0xb1
void or_c(void) {
	registers.a |= registers.c;
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0xc0
void ret_nz(void) {
	if(FLAGS_ISZERO) ticks += 8;
	else {
		registers.pc = readShortFromStack();
		debugJump();
		ticks += 20;
	}
}

// 0xc3
void jp_nn(unsigned short operand) {
	registers.pc = operand;
	debugJump();
}

// 0xc5
void push_bc(void) { writeShortToStack(registers.bc); }

// 0xc9
void ret(void) { registers.pc = readShort(registers.sp); registers.sp += 2; }

// 0xcd
void call_nn(unsigned short operand) { writeShortToStack(registers.pc); registers.pc = operand; }

// 0xd5
void push_de(void) { writeShortToStack(registers.de); }

// 0xe0
void ld_ff_n_ap(unsigned char operand) { writeByte(0xff00 + operand, registers.a); }

// 0xe2
void ld_ff_c_a(void) { writeByte(0xff00 + registers.c, registers.a); }

// 0xe5
void push_hl(void) { writeShortToStack(registers.hl); }

// 0xea
void ld_nnp_a(unsigned short operand) { writeByte(operand, registers.a); }

// 0xf0
void ld_ff_ap_n(unsigned char operand) { registers.a = readByte(0xff00 + operand); }

// 0xf3
void di(void) { interrupt.master = 0; }

// 0xf5
void push_af(void) { writeShortToStack(registers.af); }

// 0xfb
void ei(void) { interrupt.master = 1; }

// 0xfe
void cp_n(unsigned char operand) {
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if(registers.a == operand) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	if(operand > registers.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if((operand & 0x0f) > (registers.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
}
