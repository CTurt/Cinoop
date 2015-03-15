#include <3ds.h>

#include "memory.h"
#include "gpu.h"
#include "drawing.h"

#include "display.h"

const struct rgb palette[4] = {
	{ 255, 255, 255 },
	{ 192, 192, 192 },
	{ 96, 96, 96 },
	{ 0, 0, 0 },
};

void renderScanline(void) {
	u8 *framebuffer = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	
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
		
		drawPixelTopRGBFramebuffer(framebuffer, (400 - 160) / 2 + (pixelOffset % 160), (240 - 144) / 2 + (pixelOffset / 160), palette[tiles[tile][x][y]].r, palette[tiles[tile][x][y]].g, palette[tiles[tile][x][y]].b);
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
