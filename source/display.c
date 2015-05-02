#ifdef LIN
	#include <time.h>
	#include <unistd.h>
#endif

#include "platform.h"
#include "memory.h"
#include "gpu.h"
#include "debug.h"
#include "main.h"

#include "display.h"

#if defined WIN || defined LIN
	struct rgb framebuffer[160 * 144];
#endif

#if defined WIN || defined LIN || defined DS3
	const struct rgb palette[4] = {
		{ 255, 255, 255 },
		{ 192, 192, 192 },
		{ 96, 96, 96 },
		{ 0, 0, 0 },
	};
#endif

#ifdef DS
	const unsigned short palette[4] = {
		RGB15(31, 31, 31),
		RGB15(24, 24, 24),
		RGB15(12, 12, 12),
		RGB15(0, 0, 0),
	};
	
	unsigned char dirtyTileset = 0;
#endif

#ifdef GC
	unsigned int *framebuffer = NULL;
	
	const unsigned int palette[4] = {
		0xFF80FF80,
		0xC080C080,
		0x60806080,
		0x00800080,
	};
#endif

#ifndef DS
void renderScanline(void) {
	#ifdef DS3
		u8 *framebuffer = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	#endif
	
	int mapOffset = (gpu.control & GPU_CONTROL_TILEMAP) ? 0x1c00 : 0x1800;
	mapOffset += (((gpu.scanline + gpu.scrollY) & 255) >> 3) << 5;
	
	int lineOffset = (gpu.scrollX >> 3);
	
	int x = gpu.scrollX & 7;
	int y = (gpu.scanline + gpu.scrollY) & 7;
	
	int pixelOffset;
	#if defined WIN || defined LIN
		pixelOffset = gpu.scanline * 160;
	#endif
	
	#ifdef DS3
		pixelOffset = (((400 - 160) / 2) * 240 + (239 - ((240 - 144) / 2 + gpu.scanline))) * 3;
	#endif
	
	#ifdef GC
		pixelOffset = (320 - 160) / 2 + ((240 - 144) / 2 + gpu.scanline) * 640;
	#endif
	
	unsigned short tile = (unsigned short)vram[mapOffset + lineOffset];
	//if((gpu.control & GPU_CONTROL_TILESET) && tile < 128) tile += 256;
	
	unsigned char scanlineRow[160];
	
	// if bg enabled
	int i;
	for(i = 0; i < 160; i++) {
		unsigned char colour = tiles[tile][y][x];
		
		scanlineRow[i] = colour;
		
		#if defined WIN || defined LIN
			framebuffer[pixelOffset].r = backgroundPalette[colour].r;
			framebuffer[pixelOffset].g = backgroundPalette[colour].g;
			framebuffer[pixelOffset].b = backgroundPalette[colour].b;
		#endif
		
		#ifdef DS3
			framebuffer[pixelOffset] = backgroundPalette[colour].r;
			framebuffer[pixelOffset + 1] = backgroundPalette[colour].g;
			framebuffer[pixelOffset + 2] = backgroundPalette[colour].b;
		#endif
		
		#ifdef GC
			framebuffer[pixelOffset] = backgroundPalette[colour];
			framebuffer[pixelOffset + 320] = backgroundPalette[colour];
		#endif
		
		#ifdef DS3
			pixelOffset += 240 * 3;
		#else
			pixelOffset++;
		#endif
		
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
		
		int sx = sprite.x - 8;
		int sy = sprite.y - 16;
		
		if(sy <= gpu.scanline && (sy + 8) > gpu.scanline) {
			COLOUR *pal = spritePalette[sprite.palette];
			
			int pixelOffset;
			#if defined WIN || defined LIN
				pixelOffset = gpu.scanline * 160 + sx;
			#endif
			
			#ifdef DS3
				pixelOffset = ((((400 - 160) / 2) + sx) * 240 + (239 - ((240 - 144) / 2 + gpu.scanline))) * 3;
			#endif
			
			#ifdef GC
				pixelOffset = (320 - 160) / 2 + sx + ((240 - 144) / 2 + gpu.scanline) * 640;
			#endif
			
			unsigned char tileRow;
			if(sprite.vFlip) tileRow = 7 - (gpu.scanline - sy);
			else tileRow = gpu.scanline - sy;
			
			int x;
			for(x = 0; x < 8; x++) {
				if(sx + x >= 0 && sx + x < 160 && (~sprite.priority || !scanlineRow[sx + x])) {
					unsigned char colour;
					
					if(sprite.hFlip) colour = tiles[sprite.tile][tileRow][7 - x];
					else colour = tiles[sprite.tile][tileRow][x];
					
					if(colour) {
						#if defined WIN || defined LIN
							framebuffer[pixelOffset].r = pal[colour].r;
							framebuffer[pixelOffset].g = pal[colour].g;
							framebuffer[pixelOffset].b = pal[colour].b;
						#endif
						
						#ifdef DS3
							framebuffer[pixelOffset] = pal[colour].r;
							framebuffer[pixelOffset + 1] = pal[colour].g;
							framebuffer[pixelOffset + 2] = pal[colour].b;
						#endif
						
						#ifdef GC
							framebuffer[pixelOffset] = pal[colour];
							framebuffer[pixelOffset + 320] = pal[colour];
						#endif
					}
					
					#ifdef DS3
						pixelOffset += 240 * 3;
					#else
						pixelOffset++;
					#endif
				}
			}
		}
	}
}
#endif

#ifdef DS
void copyMap(void) {
	int mapOffset = (gpu.control & GPU_CONTROL_TILEMAP) ? 0x1c00 : 0x1800;
	mapOffset += ((gpu.scrollY & 255) >> 3) << 5;
	
	int x, y;
	for(y = 0; y < 144 / 8; y++) {
		for(x = 0; x < 160 / 8; x++) {
			bgGetMapPtr(layer)[((256 - 160) / 2) / 8 + x + (y + ((192 - 144) / 2) / 8) * 256 / 8] = vram[mapOffset + x + y * 256 / 8];
		}
	}
}
#endif

#if defined WIN || defined LIN
void drawFramebuffer(void) {
	// Should render to a texture instead of obsolete glDrawPixels
	
	//framebuffer[x + y * 160].r = 255;
	//framebuffer[x + y * 160].g = 255;
	//framebuffer[x + y * 160].b = 255;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
	
	#ifdef WIN
		LDFS_SwapBuffers();
		
		#ifndef DEBUG_SPEED
			LDFS_MaintainFramerate();
		#endif
	#endif
	
	#ifdef LIN
		glXSwapBuffers(dpy, win);
		
		#ifndef DEBUG_SPEED
			static struct timespec frameStart;
			struct timespec frameEnd;
			
			long mtime, seconds, useconds;
			
			do {
				clock_gettime(CLOCK_MONOTONIC, &frameEnd);
				seconds = frameEnd.tv_sec - frameStart.tv_sec;
				useconds = frameEnd.tv_nsec - frameStart.tv_nsec;
				
				mtime = (seconds * 1000 + useconds / (1000.0 * 1000.0));
			} while(mtime < 1 / 60.0);
			
			clock_gettime(CLOCK_MONOTONIC, &frameStart);
		#endif
	#endif
}
#endif
