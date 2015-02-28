#include "memory.h"

#include "gpu.h"

struct gpu gpu;

unsigned char tiles[384][8][8];

void updateTile(unsigned short address, unsigned char value) {
	address -= 0x8000;
	
	unsigned short tile = (address >> 4) & 511;
	unsigned short y = (address >> 1) & 7;
	
	unsigned char x, bitIndex;
	for(x = 0; x < 8; x++) {
		bitIndex = 1 << (7 - x);
		tiles[tile][y][x] = ((vram[address] & bitIndex) ? 1 : 0) + ((vram[address + 1] & bitIndex) ? 2 : 0);
	}
}
