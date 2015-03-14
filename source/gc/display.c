#include <gccore.h>

#include "memory.h"
#include "gpu.h"

#include "display.h"

unsigned int *framebuffer = NULL;

const unsigned int palette[4] = {
	0xFF80FF80,
	0xC080C080,
	0x60806080,
	0x00800080,
};

void renderScanline(void) {
	int mapOffset = (gpu.control & GPU_CONTROL_TILEMAP) ? 0x1c00 : 0x1800;
	mapOffset += (((gpu.scanline + gpu.scrollY) & 255) >> 3) << 5;
	
	int lineOffset = (gpu.scrollX >> 3);
	
	int x = gpu.scrollX & 7;
	int y = (gpu.scanline + gpu.scrollY) & 7;
	
	int pixelOffset = gpu.scanline * 160;
	
	unsigned char tile = vram[mapOffset + lineOffset];
	
	if((gpu.control & GPU_CONTROL_TILESET) == 1 && tile < 128) tile += 256;
	
	unsigned char scanlineRow[160];
	
	// if bg enabled
	int i;
	for(i = 0; i < 160; i++) {
		scanlineRow[i] = tiles[tile][x][y];
		
		framebuffer[(320 - 160) / 2 + (240 - 144) / 2 * 640 + (pixelOffset / 160) * 640 + pixelOffset % 160] = palette[tiles[tile][x][y]];
		framebuffer[(320 - 160) / 2 + (240 - 144) / 2 * 640 + (pixelOffset / 160) * 640 + pixelOffset % 160 + 320] = palette[tiles[tile][x][y]];
		pixelOffset++;
		
		x++;
		if(x == 8) {
			x = 0;
			lineOffset = (lineOffset + 1) & 31;
			tile = vram[mapOffset + lineOffset];
			if((gpu.control & GPU_CONTROL_TILESET) == 1 && tile < 128) tile += 256;
		}
	}
}
