#pragma once

#include <gl/gl.h>

struct rgb {
	unsigned char r, g, b;
} extern framebuffer[160 * 144];

void drawFramebuffer(void);
