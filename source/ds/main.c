#include <nds.h>
#include <fat.h>

#include <stdio.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"
#include "display.h"

#include "registers.h"
#include "memory.h"

#include "main.h"

void dsVblank(void) {
	if(dirtyMap) {
		memcpy(bgGetGfxPtr(layer), tiles, 160 / 8 * 144 / 8 * 32);
		dirtyMap = 0;
	}
}

void quit(void) {
	void irqDummy(void);
	irqSet(IRQ_VBLANK, irqDummy);
	while(1) swiWaitForVBlank();
}

int main(void) {
	// Framebuffer mode
	//videoSetMode(MODE_FB0);
	//vramSetBankA(VRAM_A_LCD);
	
	// http://mtheall.com/vram.html#T0=1&NT0=384&MB0=13&TB0=0&S0=0
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(layer, BgType_Text8bpp, BgSize_T_256x256, 13, 0);
	
	BG_PALETTE[0] = palette[0];
	BG_PALETTE[1] = palette[1];
	BG_PALETTE[2] = palette[2];
	BG_PALETTE[3] = palette[3];
	
	memset(bgGetGfxPtr(layer) + 384 * 32, 3, 64);
	
	int i;
	for(i = 0; i < 256 / 8 * 192 / 8; i++) {
		bgGetMapPtr(layer)[i] = 384;
	}
	
	irqSet(IRQ_VBLANK, dsVblank);
	
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
