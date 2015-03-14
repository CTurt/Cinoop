#pragma once

#include "platform.h"

#define GPU_CONTROL_BGENABLE (1 << 0)
#define GPU_CONTROL_SPRITEENABLE (1 << 1)
#define GPU_CONTROL_SPRITEVDOUBLE (1 << 2)
#define GPU_CONTROL_TILEMAP (1 << 3)
#define GPU_CONTROL_TILESET (1 << 4)
#define GPU_CONTROL_WINDOWENABLE (1 << 5)
#define GPU_CONTROL_WINDOWTILEMAP (1 << 6)
#define GPU_CONTROL_DISPLAYENABLE (1 << 7)

struct gpu {
	unsigned char control;
	unsigned char scrollX;
	unsigned char scrollY;
	unsigned char scanline;
	unsigned char bgPalette;
	unsigned char spritePalette[2];
	unsigned long tick;
} extern gpu;

struct sprite {
	unsigned char y;
	unsigned char x;
	unsigned char tile;
	struct options {
		#ifdef LITTLE_E
			unsigned char priority : 1;
			unsigned char vFlip : 1;
			unsigned char hFlip : 1;
			unsigned char palette : 1;
		#else
			unsigned char palette : 1;
			unsigned char hFlip : 1;
			unsigned char vFlip : 1;
			unsigned char priority : 1;
		#endif
	}; struct options options;
};

extern unsigned char tiles[384][8][8];

void gpuStep(void);

void hblank(void);

void renderScanline(void);

void updateTile(unsigned short address, unsigned char value);
//void updateOAM(unsigned short address, unsigned char value);
