#pragma once

#ifdef DS
	#define framebuffer VRAM_A
#endif

#ifdef GC
	extern unsigned int *framebuffer;
#endif

#ifdef DS3
	struct rgb {
		unsigned char r, g, b;
	} extern const palette[4];
#endif
