#include <nds.h>
#include <fat.h>

#include <stdio.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"

#include "main.h"

void quit(void) {
	while(1) swiWaitForVBlank();
}

int main(void) {
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	
	consoleDemoInit();
	
	printf("Starting...\n");
	
	if(!fatInitDefault()) {
		printf("FAT init failed!\n");
		quit();
	}
	
	if(!loadROM("tetris.gb")) {
		printf("Failed to load ROM!\n");
		quit();
	}
	
	printf("ROM load passed!\n");
	
	reset();
	
	while(1) {
		// The Game Boy and DS use the same key layout, how convenient :)
		keys.c = (unsigned char)REG_KEYINPUT;
		
		cpuStep();
		gpuStep();
		interruptStep();
	}
	
	return 0;
}
