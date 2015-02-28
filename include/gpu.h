#pragma once

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
} extern gpu;

extern unsigned char tiles[384][8][8];

void updateTile(unsigned short address, unsigned char value);
