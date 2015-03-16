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
	
	int i;
	for(i = 0; i < 512 + 64; i++) {
		bgGetMapPtr(layer)[i] = vram[mapOffset + i];
	}
}
