#include "memory.h"
#include "registers.h"

#include "interrupts.h"

struct interrupt interrupt;

void vblank(void) {
	interrupt.master = 0;
	
	registers.sp -= 2;
	writeShort(registers.sp, registers.pc);
	
	registers.pc = 0x40;
	
	// timer 3, 12
}

void returnFromInterrupt(void) {
	interrupt.master = 1;
	
	registers.pc = readShort(registers.sp);
	registers.sp += 2;
}
