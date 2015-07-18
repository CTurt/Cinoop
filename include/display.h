#pragma once

#if defined WIN || defined LIN || defined DS3
	struct rgb {
		unsigned char r, g, b;
	};
	
	typedef struct rgb COLOUR;
#endif

#if defined WIN || defined LIN
	extern COLOUR framebuffer[160 * 144];
	
	void drawFramebuffer(void);
#endif

#ifdef DS
	#define layer 3
	
	typedef unsigned short COLOUR;
	
	extern unsigned char dirtyTileset;
	
	void copyMap(void);
#endif

#ifdef GC
	typedef unsigned int COLOUR;
	
	extern COLOUR *framebuffer;
#endif

#ifdef PSP
	typedef u32 COLOUR;
	
	#define framebuffer ((u32 *)0x44000000)
#endif

#ifndef DS
	void renderScanline(void);
#endif

extern const COLOUR palette[4];
