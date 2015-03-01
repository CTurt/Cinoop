#pragma once

extern unsigned char realtimeDebugEnable;

void realtimeDebug(void);

#define debugJump()
//void debugJump(void);

void printRegisters(void);
