// Experimental! Scales the background to fill the screen. Sprites will be displayed at incorrect positions.
//#define SCALE

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
	if(dirtyTileset) {
		// No way for backgrounds and sprites to share tileset on DS
		memcpy(bgGetGfxPtr(layer), tiles, 160 / 8 * 144 / 8 * 32);
		memcpy(SPRITE_GFX, tiles, 160 / 8 * 144 / 8 * 32);
		dirtyTileset = 0;
	}
	
	// Render sprites with correct priority!
	int i;
	for(i = 0; i < 40; i++) {
		struct sprite sprite = ((struct sprite *)oam)[i];
		
		//if(sprite.priority) {
			int sx = sprite.x - 8 + (256 - 160) / 2;
			int sy = sprite.y - 16 + (192 - 144) / 2;
			
			oamSet(&oamMain, i, sx, sy, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color, SPRITE_GFX + sprite.tile * 32, 0, false, false, false, false, false);
		//}
	}
	
	oamUpdate(&oamMain);
}

void quit(void) {
	void irqDummy(void);
	irqSet(IRQ_VBLANK, irqDummy);
	while(1) swiWaitForVBlank();
}

int main(void) {
	// http://mtheall.com/vram.html#T0=1&NT0=384&MB0=13&TB0=0&S0=0
	#ifdef SCALE
		videoSetMode(MODE_3_2D);
	#else
		videoSetMode(MODE_0_2D);
	#endif
	
	vramSetBankA(VRAM_A_MAIN_BG);
	
	#ifdef SCALE
		bgInit(layer, BgType_ExRotation, BgSize_ER_256x256, 13, 0);
		bgSetCenter(layer, 256 / 2, 192 / 2);
		bgSetScroll(layer, 256 / 2, 192 / 2);
		bgSetScale(layer, 256 - (192 - 144), 256 - (192 - 144));
	#else
		bgInit(layer, BgType_Text8bpp, BgSize_T_256x256, 13, 0);
	#endif
	
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	
	BG_PALETTE[0] = palette[0];
	BG_PALETTE[1] = palette[1];
	BG_PALETTE[2] = palette[2];
	BG_PALETTE[3] = palette[3];
	
	SPRITE_PALETTE[0] = palette[0];
	SPRITE_PALETTE[1] = palette[1];
	SPRITE_PALETTE[2] = palette[2];
	SPRITE_PALETTE[3] = palette[3];
	
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
