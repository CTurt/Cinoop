#pragma once

// Debug enables
//#define DEBUG_STACK // prints writes and reads to the stack
//#define DEBUG_JUMP // prints jumps
//#define DEBUG_SPEED // disables speed limiting

extern unsigned char realtimeDebugEnable;
extern unsigned char tetrisPatch;

#ifdef DS
	#define realtimeDebug()
#else
	void realtimeDebug(void);
#endif

#ifdef DS
	#define debugJump()
#else
#ifndef DEBUG_JUMP
	#define debugJump()
#else
	void debugJump(void);
#endif
#endif

#ifdef DS
	#define printRegisters()
#else
	void printRegisters(void);
#endif
