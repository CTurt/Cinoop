#include <gccore.h>
#include <ogcsys.h>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "rom.h"
#include "registers.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"
#include "display.h"

#define NOFS
#ifdef NOFS
	#include "string.h"
	
	#include "memory.h"
	
	#include "tetris_bin.h"
#endif

#include "main.h"

u32 first_frame = 1;
GXRModeObj *rmode;
vu16 oldstate;
vu16 keystate;
vu16 keydown;
vu16 keyup;
PADStatus pad[4];

void quit(void) {
	while(1) VIDEO_WaitVSync();;
}

int main(void) {
	VIDEO_Init();
	
	rmode = VIDEO_GetPreferredMode(NULL);
	
	PAD_Init();
	
	framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	VIDEO_Configure(rmode);
	
	VIDEO_SetNextFramebuffer(framebuffer);
	
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
	
	console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * 2);
	
	printf("Starting...\n");
	
	#ifndef NOFS
		if(!fatInitDefault()) {
			printf("FAT init failed!\n");
			quit();
		}
		
		if(!loadROM("tetris.gb")) {
			printf("Failed to load ROM!\n");
			quit();
		}
	#else
		memcpy(cart, tetris_bin, tetris_bin_size);
		tetrisPatch = 1;
	#endif
	
	printf("ROM load passed!\n");
	
	reset();
	
	while(1) {
		cpuStep();
		gpuStep();
		interruptStep();
		
		//VIDEO_WaitVSync();
		PAD_ScanPads();
		
		int buttonsDown = PAD_ButtonsDown(0);
		
		if(buttonsDown & PAD_BUTTON_START) {
			exit(0);
		}
	}
}
