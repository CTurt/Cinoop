#pragma once

#define INTERRUPTS_VBLANK	(1 << 0)
#define INTERRUPTS_LCDSTAT	(1 << 1)
#define INTERRUPTS_TIMER	(1 << 2)
#define INTERRUPTS_SERIAL	(1 << 3)
#define INTERRUPTS_JOYPAD	(1 << 4)

struct interrupt {
	unsigned char master;
	unsigned char enable;
	unsigned char flags;
} extern interrupt;

void interruptStep(void);

void vblank(void);
void lcdStat(void);
void timer(void);
void serial(void);
void joypad(void);

void returnFromInterrupt(void);
