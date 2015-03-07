#include <windows.h>
#include <LDFS.h>

#include "memory.h"
#include "cpu.h"
#include "interrupts.h"
#include "opengl.h"

#include "gpu.h"

struct gpu gpu;

// There must be space for 512 tiles, but only 384 can be used
unsigned char tiles[512][8][8];

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
					vblank();
					
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
				
				renderScanline();
				
				gpu.tick -= 172;
			}
			
			break;
	}
}

void hblank(void) {
	gpu.scanline++;
}

void renderScanline(void) {
	// draw tileset to framebuffer as a test
	/*{
		int i;
		for(i = 0; i < (144 / 8) * (160 / 8); i++) {
			int x;
			for(x = 0; x < 8; x++) {
				int y;
				for(y = 0; y < 8; y++) {
					framebuffer[(i * 8 % 160) + x + (y + i * 8 / 160 * 8) * 160].r = palette[tiles[i][x][y]].r;
					framebuffer[(i * 8 % 160) + x + (y + i * 8 / 160 * 8) * 160].g = palette[tiles[i][x][y]].g;
					framebuffer[(i * 8 % 160) + x + (y + i * 8 / 160 * 8) * 160].b = palette[tiles[i][x][y]].b;
				}
			}
		}
	}*/
	
	int mapOffset = (gpu.control & GPU_CONTROL_TILEMAP) ? 0x1c00 : 0x1800;
	mapOffset += (((gpu.scanline + gpu.scrollY) & 255) >> 3) << 5;
	
	int lineOffset = (gpu.scrollX >> 3);
	
	int x = gpu.scrollX & 7;
	int y = (gpu.scanline + gpu.scrollY) & 7;
	
	int pixelOffset = gpu.scanline * 160;
	
	unsigned char tile = vram[mapOffset + lineOffset];
	
	if((gpu.control & GPU_CONTROL_TILESET) == 1 && tile < 128) tile += 256;
	
	int i;
	for(i = 0; i < 160; i++) {
		framebuffer[pixelOffset].r = palette[tiles[tile][x][y]].r;
		framebuffer[pixelOffset].g = palette[tiles[tile][x][y]].g;
		framebuffer[pixelOffset].b = palette[tiles[tile][x][y]].b;
		pixelOffset += 1;
		
		x++;
		if(x == 8) {
			x = 0;
			lineOffset = (lineOffset + 1) & 31;
			tile = vram[mapOffset + lineOffset];
			if((gpu.control & GPU_CONTROL_TILESET) == 1 && tile < 128) tile += 256;
		}
	}
}

void updateTile(unsigned short address, unsigned char value) {
	address &= 0x1ffe;
	
	unsigned short tile = (address >> 4) & 511;	
	unsigned short y = (address >> 1) & 7;
	
	unsigned char x, bitIndex;
	
	for(x = 0; x < 8; x++) {
		bitIndex = 1 << (7 - x);
		tiles[tile][x][y] = ((vram[address] & bitIndex) ? 1 : 0) + ((vram[address + 1] & bitIndex) ? 2 : 0);
	}
}
