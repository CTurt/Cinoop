#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "opengl.h"

#include "interrupts.h"

struct interrupt interrupt;

void interruptStep(void) {
	if(interrupt.master && interrupt.enable && interrupt.flags) {
		unsigned char fired = interrupt.enable & interrupt.flags;
		
		if(fired & INTERRUPTS_VBLANK) {
			interrupt.flags &= ~INTERRUPTS_VBLANK;
			vblank();
		}
	}
}

void vblank(void) {
	drawFramebuffer();
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x40;
	
	ticks += 12;
}

void returnFromInterrupt(void) {
	interrupt.master = 1;
	registers.pc = readShortFromStack();
}
