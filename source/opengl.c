#include <gl/gl.h>

#include "opengl.h"

struct rgb framebuffer[160 * 144];

void drawFramebuffer(void) {
	//framebuffer[50 * 160 + 50].r = 255;
	//framebuffer[50 * 160 + 50].g = 255;
	//framebuffer[50 * 160 + 50].b = 255;
	
	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
}
