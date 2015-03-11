#pragma once

#include <gl/gl.h>

struct rgb {
	unsigned char r, g, b;
} extern framebuffer[160 * 144];

extern const struct rgb palette[4];

void renderScanline(void);
void drawFramebuffer(void);
