#include <stddef.h>

#ifdef WIN
	#include <windows.h>
	#include <LDFS.h>
	#include "opengl.h"
#endif

#include "platform.h"

#include "memory.h"
#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"

struct gpu gpu;

//#ifndef DS
	unsigned char tiles[384][8][8];
//#endif

/* References:
http://www.codeslinger.co.uk/pages/projects/gameboy/lcd.html
http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics
*/

void gpuStep(void) {
	enum gpuMode {
		GPU_MODE_HBLANK = 0,
		GPU_MODE_VBLANK = 1,
		GPU_MODE_OAM = 2,
		GPU_MODE_VRAM = 3,
	} static gpuMode = GPU_MODE_HBLANK;
	
	static int lastTicks = 0;
	
	gpu.tick += ticks - lastTicks;
	
	lastTicks = ticks;
	
	switch(gpuMode) {
		case GPU_MODE_HBLANK:
			if(gpu.tick >= 204) {
				hblank();
				
				if(gpu.scanline == 143) {
					if(interrupt.enable & INTERRUPTS_VBLANK) interrupt.flags |= INTERRUPTS_VBLANK;
					
					gpuMode = GPU_MODE_VBLANK;
				}
				
				else gpuMode = GPU_MODE_OAM;
				
				gpu.tick -= 204;
			}
			
			break;
		
		case GPU_MODE_VBLANK:
			if(gpu.tick >= 456) {
				gpu.scanline++;
				
				if(gpu.scanline > 153) {
					gpu.scanline = 0;
					gpuMode = GPU_MODE_OAM;
				}
				
				gpu.tick -= 456;
			}
			
			break;
		
		case GPU_MODE_OAM:
			if(gpu.tick >= 80) {
				gpuMode = GPU_MODE_VRAM;
				
				gpu.tick -= 80;
			}
			
			break;
		
		case GPU_MODE_VRAM:
			if(gpu.tick >= 172) {
				gpuMode = GPU_MODE_HBLANK;
				
				#ifndef DS
					renderScanline();
				#endif
				
				gpu.tick -= 172;
			}
			
			break;
	}
}

void hblank(void) {
	gpu.scanline++;
}

void updateTile(unsigned short address, unsigned char value) {
	address &= 0x1ffe;
	
	#ifdef DS
		unsigned char tempTile[8][8];
	#endif
	
	unsigned short tile = (address >> 4) & 511;
	unsigned short y = (address >> 1) & 7;
	
	unsigned char x, bitIndex;
	for(x = 0; x < 8; x++) {
		bitIndex = 1 << (7 - x);
		
		#ifdef DS
			tempTile[y][x] = ((vram[address] & bitIndex) ? 1 : 0) + ((vram[address + 1] & bitIndex) ? 2 : 0);
		#else
			//((unsigned char (*)[8][8])tiles)[tile][x][y] = ((vram[address] & bitIndex) ? 1 : 0) + ((vram[address + 1] & bitIndex) ? 2 : 0);
			tiles[tile][x][y] = ((vram[address] & bitIndex) ? 1 : 0) + ((vram[address + 1] & bitIndex) ? 2 : 0);
		#endif
	}
	
	#ifdef DS
		memcpy(tile * 32 + bgGetGfxPtr(layer), tempTile, sizeof(tempTile));
	#endif
}
