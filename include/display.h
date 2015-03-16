#pragma once

#ifdef DS
	#define layer 3
	
	extern const unsigned short palette[4];
	extern unsigned char dirtyMap;
	
	void copyMap(void);
#endif

#ifdef GC
	extern unsigned int *framebuffer;
#endif

#ifdef DS3
	struct rgb {
		unsigned char r, g, b;
	} extern const palette[4];
#endif
