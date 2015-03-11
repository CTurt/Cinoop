#include <windows.h>
#include <LDFS.h>

#include "memory.h"
#include "gpu.h"
#include "debug.h"
#include "opengl.h"

struct rgb framebuffer[160 * 144];

const struct rgb palette[4] = {
	{ 255, 255, 255 },
	{ 192, 192, 192 },
	{ 96, 96, 96 },
	{ 0, 0, 0 },
};

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
	
	unsigned char scanlineRow[160];
	
	// if bg enabled
	int i;
	for(i = 0; i < 160; i++) {
		scanlineRow[i] = tiles[tile][x][y];
		
		framebuffer[pixelOffset].r = palette[tiles[tile][x][y]].r;
		framebuffer[pixelOffset].g = palette[tiles[tile][x][y]].g;
		framebuffer[pixelOffset].b = palette[tiles[tile][x][y]].b;
		pixelOffset++;
		
		x++;
		if(x == 8) {
			x = 0;
			lineOffset = (lineOffset + 1) & 31;
			tile = vram[mapOffset + lineOffset];
			if((gpu.control & GPU_CONTROL_TILESET) == 1 && tile < 128) tile += 256;
		}
	}
	
	// if sprites enabled
	for(i = 0; i < 40; i++) {
		struct sprite *sprite = &((struct sprite *)oam)[i];
		
		if(sprite->y <= gpu.scanline && (sprite->y + 8) > gpu.scanline) {
			//unsigned char paletteNumber = gpu.spritePalette[sprite->palette];
			int pixelOffset = (gpu.scanline * 160 + sprite->x) * 4;
			unsigned char *tileRow = tiles[sprite->tile][gpu.scanline - sprite->y];
			
			// handle flipping
			
			int x;
			for(x = 0; x < 8; x++) {
				if(sprite->x + x >= 0 && sprite->x + x < 160 && tileRow[x] && (sprite->priority || !scanlineRow[sprite->x + x])) {
					// handle flipping
					
					struct rgb colour = palette[tileRow[x]];
					
					if(pixelOffset >= 0 && pixelOffset < 160 * 144) {
						framebuffer[pixelOffset].r = colour.r;
						framebuffer[pixelOffset].g = colour.g;
						framebuffer[pixelOffset].b = colour.b;
						pixelOffset++;
					}
				}
			}
		}
	}
}

void drawFramebuffer(void) {
	// Should render to a texture instead of obsolete glDrawPixels
	
	//framebuffer[x + y * 160].r = 255;
	//framebuffer[x + y * 160].g = 255;
	//framebuffer[x + y * 160].b = 255;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
	LDFS_SwapBuffers();
	
	#ifndef DEBUG_SPEED
		LDFS_MaintainFramerate();
	#endif
}
