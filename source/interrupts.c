#ifdef DS
#include "fakeWindows.h"
#else
#include "opengl.h"
#endif

#include "cpu.h"
#include "memory.h"
#include "registers.h"

#include "interrupts.h"

struct interrupt interrupt;

void interruptStep(void) {
	if(interrupt.master && interrupt.enable && interrupt.flags) {
		unsigned char fire = interrupt.enable & interrupt.flags;
		
		if(fire & INTERRUPTS_VBLANK) {
			interrupt.flags &= ~INTERRUPTS_VBLANK;
			vblank();
		}
		
		if(fire & INTERRUPTS_LCDSTAT) {
			interrupt.flags &= ~INTERRUPTS_LCDSTAT;
			lcdStat();
		}
		
		if(fire & INTERRUPTS_TIMER) {
			interrupt.flags &= ~INTERRUPTS_TIMER;
			timer();
		}
		
		if(fire & INTERRUPTS_SERIAL) {
			interrupt.flags &= ~INTERRUPTS_SERIAL;
			serial();
		}
		
		if(fire & INTERRUPTS_JOYPAD) {
			interrupt.flags &= ~INTERRUPTS_JOYPAD;
			joypad();
		}
	}
}

void vblank(void) {
	#ifndef DS
	drawFramebuffer();
	#endif
	
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x40;
	
	ticks += 12;
}

void lcdStat(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x48;
	
	ticks += 12;
}

void timer(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x50;
	
	ticks += 12;
}

void serial(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x58;
	
	ticks += 12;
}

void joypad(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x60;
	
	ticks += 12;
}

void returnFromInterrupt(void) {
	interrupt.master = 1;
	registers.pc = readShortFromStack();
}
