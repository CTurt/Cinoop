#include <3ds.h>

#include "drawing.h"

void drawPixelTopRGBFramebuffer(u8 *fb, int x, int y, u8 r, u8 g, u8 b) {
	y = 239 - y;
	u32 v = (y + x * 240) * 3;
	fb[v] = r;
	fb[v + 1] = g;
	fb[v + 2] = b;
}
