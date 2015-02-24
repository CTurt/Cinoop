#include <stdio.h>

#include "registers.h"

#include "cpu.h"

void reset(void) {
	registers.pc = 0x100;
}

void cpuStep(void) {
	printf("Undefined opcode!\n");
	exit(1);
}
