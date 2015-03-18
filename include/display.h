#pragma once

#if defined WIN || defined LIN || defined DS3
	struct rgb {
		unsigned char r, g, b;
	} extern const palette[4];
#endif

#if defined WIN || defined LIN
	extern struct rgb framebuffer[160 * 144];
	
	void drawFramebuffer(void);
#endif

#ifdef DS
	#define layer 3
	
	extern const unsigned short palette[4];
	extern unsigned char dirtyMap;
	
	void copyMap(void);
#endif

#ifdef GC
	extern unsigned int *framebuffer;
#endif

#ifndef DS
	void renderScanline(void);
#endif
