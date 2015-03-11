#include <nds.h>

#include "memory.h"
#include "gpu.h"

//#include "display.h"

const unsigned short palette[4] = {
	RGB15(31, 31, 31),
	RGB15(24, 24, 24),
	RGB15(12, 12, 12),
	RGB15(0, 0, 0),
};

void renderScanline(void) {
	// tileset
	/*{
		int i;
		for(i = 0; i < (144 / 8) * (160 / 8); i++) {
			int x;
			for(x = 0; x < 8; x++) {
				int y;
				for(y = 0; y < 8; y++) {
					VRAM_A[(i * 8 % 256) + x + (y + i * 8 / 256 * 8) * 256] = palette[tiles[i][x][y]];
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
	
	unsigned char scanlineRow[160];
	
	// if bg enabled
	int i;
	for(i = 0; i < 160; i++) {
		scanlineRow[i] = tiles[tile][x][y];
		
		VRAM_A[(256 - 160) / 2 + (192 - 144) / 2 * 256 + (pixelOffset / 160) * 256 + pixelOffset % 160] = palette[tiles[tile][x][y]];
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
