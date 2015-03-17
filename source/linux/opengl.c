#include "platform.h"

#include "main.h"
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
	int mapOffset = (gpu.control & GPU_CONTROL_TILEMAP) ? 0x1c00 : 0x1800;
	mapOffset += (((gpu.scanline + gpu.scrollY) & 255) >> 3) << 5;
	
	int lineOffset = (gpu.scrollX >> 3);
	
	int x = gpu.scrollX & 7;
	int y = (gpu.scanline + gpu.scrollY) & 7;
	
	int pixelOffset = gpu.scanline * 160;
	
	unsigned short tile = (unsigned short)vram[mapOffset + lineOffset];
	//if((gpu.control & GPU_CONTROL_TILESET) && tile < 128) tile += 256;
	
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
			//if((gpu.control & GPU_CONTROL_TILESET) && tile < 128) tile += 256;
		}
	}
	
	// if sprites enabled
	for(i = 0; i < 40; i++) {
		struct sprite sprite = ((struct sprite *)oam)[i];
		
		/*if(sprite.tile) {
			int x, y;
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 8; x++) {
					int p = sprite.x - 8 + x + (sprite.y - 16 + y) * 160;
					if(p >= 0 && p < 144 * 160) {
						framebuffer[p].r = palette[tiles[sprite.tile][x][y]].r;
						framebuffer[p].g = palette[tiles[sprite.tile][x][y]].g;
						framebuffer[p].b = palette[tiles[sprite.tile][x][y]].b;
					}
				}
			}
		}*/
		
		sprite.x -= 8;
		sprite.y -= 16;
		
		if(sprite.y <= gpu.scanline && (sprite.y + 8) > gpu.scanline) {
			//unsigned char paletteNumber = gpu.spritePalette[sprite->palette];
			int pixelOffset = (gpu.scanline * 160 + sprite.x);
			
			unsigned char tileRow;
			if(sprite.vFlip) tileRow = 7 - gpu.scanline - sprite.y;
			else tileRow = gpu.scanline - sprite.y;
			
			int x;
			for(x = 0; x < 8; x++) {
				if(sprite.x + x >= 0 && sprite.x + x < 160 && (sprite.priority || !scanlineRow[sprite.x + x])) {
					unsigned char colour;
					
					if(sprite.hFlip) colour = tiles[sprite.tile][7 - x][tileRow];
					else colour = tiles[sprite.tile][x][tileRow];
					
					if(colour && pixelOffset >= 0 && pixelOffset < 160 * 144) {
						framebuffer[pixelOffset].r = palette[colour].r;
						framebuffer[pixelOffset].g = palette[colour].g;
						framebuffer[pixelOffset].b = palette[colour].b;
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
	glXSwapBuffers(dpy, win);
}
