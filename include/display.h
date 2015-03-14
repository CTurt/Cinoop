#pragma once

#ifdef DS
	#define framebuffer VRAM_A
#endif

#ifdef GC
	extern unsigned int *framebuffer;
#endif
