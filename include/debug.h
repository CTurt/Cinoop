#pragma once

// Debug enables
//#define DEBUG_STACK // prints writes and reads to the stack
//#define DEBUG_JUMP // prints jumps
//#define DEBUG_SPEED // disables speed limiting

extern unsigned char realtimeDebugEnable;

#ifdef WIN
	void realtimeDebug(void);
#else
	#define realtimeDebug()
#endif

#ifdef WIN
	#ifndef DEBUG_JUMP
		#define debugJump()
	#else
		void debugJump(void);
	#endif
#else
	#define debugJump()
#endif

#ifdef WIN
	void printRegisters(void);
#else
	#define printRegisters()
#endif
