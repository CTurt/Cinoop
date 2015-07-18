#include <stdlib.h>
#include <math.h>

#include "platform.h"

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"

#include "main.h"

PSP_MODULE_INFO("cinoop", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void quit(void) {
	sceKernelExitGame();
	exit(0);
}

int main(int argc, char **argv) {
	pspDebugScreenInit();
	sceDisplaySetFrameBuf((void *)framebuffer, 512, 1, 0);
	
	printf("Starting...\n");
	
	printf("Loading file \"%s\"...\n", "tetris.gb");
	
	if(!loadROM("tetris.gb")) {
		printf("Failed!\n");
		return 1;
	}
	
	printf("Passed!\n");
	
	srand(time(NULL));
	reset();
	
	while(1) {
		cpuStep();
		gpuStep();
		interruptStep();
	}
	
	quit();
	
	return 0;
}
