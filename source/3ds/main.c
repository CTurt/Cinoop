#include <3ds.h>

#include <stdio.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"
#include "drawing.h"

#include "main.h"

// To embed the ROM inside of the binary, place it as "data/tetris.bin"
// and uncomment the define below
// this is useful for running Cinoop in a GameCube emulator which doesn't support FS
//#define NOFS
#ifdef NOFS
	#include "string.h"
	
	#include "memory.h"
	
	#include "tetris_bin.h"
#endif

void quit(void) {
	while(1) gspWaitForVBlank();
}

int main(void) {
	gfxInitDefault();
	
	gfxSetDoubleBuffering(GFX_TOP, false);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	
	consoleInit(GFX_BOTTOM, NULL);
	
	#ifndef NOFS
		printf("Initing FS...\n");
		fsInit();
		
		printf("Loading ROM...\n");
		
		if(!loadROM("/tetris.gb")) {
			printf("Failed to load ROM!\n");
			
			quit();
		}
	#else
		printf("Loading ROM...\n");
		
		memcpy(cart, tetris_bin, tetris_bin_size);
		tetrisPatch = 1;
	#endif
	
	reset();
	
	while(aptMainLoop()) {
		hidScanInput();
		u32 kHeld = hidKeysHeld();
		
		cpuStep();
		gpuStep();
		interruptStep();
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
	}
	
	svcCloseHandle(fileHandle);
	fsExit();
	
	gfxExit();
	
	return 0;
}
