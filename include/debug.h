#pragma once

extern unsigned char realtimeDebugEnable;

void realtimeDebug(void);

#define DEBUG_STACK

#define debugJump()
//void debugJump(void);

void printRegisters(void);
