#include "memory.h"
#include "registers.h"

#include "interrupts.h"

struct interrupt interrupt;

void vblank(void) {
	interrupt.master = 0;
	
	writeShortToStack(registers.pc);
	
	registers.pc = 0x40;
	
	// timer 3, 12
}

void returnFromInterrupt(void) {
	interrupt.master = 1;
	
	registers.pc = readShortFromStack();
}
