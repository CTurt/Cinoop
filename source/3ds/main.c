#include <3ds.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"
#include "drawing.h"

#include "main.h"

void quit(void) {
	while(1) gspWaitForVBlank();
}

int main(void) {
	srvInit();
	aptInit();
	hidInit(NULL);
	//irrstInit(NULL);
	acInit();
	gfxInit();
	gfxSet3D(false);
	
	clearScreen();
	
	clearScreen();
	drawString(10, 10, "Initing FS...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	fsInit();
	
	if(!loadROM("/tetris.gb")) {
		clearScreen();
		drawString(10, 10, "Failed to load ROM!");
		gfxFlushBuffers();
		gfxSwapBuffers();
		
		quit();
	}
	
	reset();
	
	while(aptMainLoop()) {
		hidScanInput();
		//irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition circlePad;
		//hidCstickRead(&cstick);
		hidCircleRead(&circlePad);
		touchPosition touch;
		touchRead(&touch);
		
		cpuStep();
		gpuStep();
		interruptStep();
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
	}
	
	svcCloseHandle(fileHandle);
	fsExit();
	
	gfxExit();
	acExit();
	//irrstExit();
	hidExit();
	aptExit();
	srvExit();
	
	return 0;
}
