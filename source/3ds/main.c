#include <3ds.h>

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
	
	drawString(10, 10, "Cinoop coming to 3DS!");
	gfxFlushBuffers();
	gfxSwapBuffers();
	drawString(10, 10, "Cinoop coming to 3DS!");
	
	fsInit();
	
	while(aptMainLoop()) {
		hidScanInput();
		//irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition circlePad;
		//hidCstickRead(&cstick);
		hidCircleRead(&circlePad);
		touchPosition touch;
		touchRead(&touch);
		
		
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
		
		gfxFlushBuffers();
		gspWaitForVBlank();
		gfxSwapBuffers();
	}
	
	//svcCloseHandle(fileHandle);
	fsExit();
	
	gfxExit();
	acExit();
	//irrstExit();
	hidExit();
	aptExit();
	srvExit();
	
	return 0;
}
