#pragma once

// Debug enables
//#define DEBUG_STACK
//#define DEBUG_JUMP

extern unsigned char realtimeDebugEnable;
extern unsigned char tetrisPatch;

void realtimeDebug(void);

#ifndef DEBUG_JUMP
	#define debugJump()
#else
	void debugJump(void);
#endif

void printRegisters(void);
