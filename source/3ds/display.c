#include <3ds.h>

#include "memory.h"
#include "gpu.h"
#include "drawing.h"

#include "display.h"

const unsigned short palette[4] = {
	RGB(31, 31, 31),
	RGB(24, 24, 24),
	RGB(12, 12, 12),
	RGB(0, 0, 0),
};

void renderScanline(void) {
	u16 *framebuffer = (u16 *)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
	
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
		
		framebuffer[(400 - 160) / 2 + (240 - 144) / 2 * 400 + (pixelOffset / 160) * 400 + pixelOffset % 160] = palette[tiles[tile][x][y]];
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
