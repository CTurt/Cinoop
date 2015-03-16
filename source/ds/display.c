#include <nds.h>

#include "memory.h"
#include "gpu.h"

#include "display.h"

const unsigned short palette[4] = {
	RGB15(31, 31, 31),
	RGB15(24, 24, 24),
	RGB15(12, 12, 12),
	RGB15(0, 0, 0),
};

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
