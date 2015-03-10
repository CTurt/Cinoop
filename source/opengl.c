#include <windows.h>
#include <LDFS.h>

#include "debug.h"
#include "opengl.h"

struct rgb framebuffer[160 * 144];

const struct rgb palette[4] = {
	{ 255, 255, 255 },
	{ 192, 192, 192 },
	{ 96, 96, 96 },
	{ 0, 0, 0 },
};

void drawFramebuffer(void) {
	// Should render to a texture instead of obsolete glDrawPixels
	
	//framebuffer[x + y * 160].r = 255;
	//framebuffer[x + y * 160].g = 255;
	//framebuffer[x + y * 160].b = 255;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
	LDFS_SwapBuffers();
	
	#ifndef DEBUG_SPEED
		LDFS_MaintainFramerate();
	#endif
}
