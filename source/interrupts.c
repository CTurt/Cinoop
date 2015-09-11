#include "platform.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "display.h"
#include "keys.h"
#include "main.h"

#include "interrupts.h"

struct interrupt interrupt;

void interruptStep(void) {
	if(interrupt.master && interrupt.enable && interrupt.flags) {
		unsigned char fire = interrupt.enable & interrupt.flags;
		
		if(fire & INTERRUPTS_VBLANK) {
			interrupt.flags &= ~INTERRUPTS_VBLANK;
			vblank();
		}
		
		if(fire & INTERRUPTS_LCDSTAT) {
			interrupt.flags &= ~INTERRUPTS_LCDSTAT;
			lcdStat();
		}
		
		if(fire & INTERRUPTS_TIMER) {
			interrupt.flags &= ~INTERRUPTS_TIMER;
			timer();
		}
		
		if(fire & INTERRUPTS_SERIAL) {
			interrupt.flags &= ~INTERRUPTS_SERIAL;
			serial();
		}
		
		if(fire & INTERRUPTS_JOYPAD) {
			interrupt.flags &= ~INTERRUPTS_JOYPAD;
			joypad();
		}
	}
}

void vblank(void) {
	#ifdef WIN
		drawFramebuffer();
	#endif
	
	#ifdef PS4
		void readController(void);
		readController();
		
		extern unsigned int buttons;
		keys.c = (unsigned char)buttons;
		
		sceKernelUsleep(1000000 / 60);
	#endif
	
	#ifdef DS
		// The Game Boy and DS use the same key layout, how convenient :)
		keys.c = (unsigned char)REG_KEYINPUT;
		
		// Support scrolling
		copyMap();
	#endif
	
	#ifdef GC
		VIDEO_WaitVSync();
	#endif
	
	#ifdef DS3
		hidScanInput();
		u32 kHeld = hidKeysHeld();
		
		keys.keys1 = 0x0f;
		keys.keys2 = 0x0f;
		
		if(kHeld & KEY_B) keys.b = 0;
		if(kHeld & KEY_A) keys.a = 0;
		if(kHeld & KEY_START) keys.start = 0;
		if(kHeld & KEY_SELECT) keys.select = 0;
		if(kHeld & KEY_UP) keys.up = 0;
		if(kHeld & KEY_DOWN) keys.down = 0;
		if(kHeld & KEY_LEFT) keys.left = 0;
		if(kHeld & KEY_RIGHT) keys.right = 0;
		
		//gfxFlushBuffers();
		gspWaitForVBlank();
		gfxSwapBuffers();
		
		if(!aptMainLoop() || ((kHeld & KEY_START) && (kHeld & KEY_SELECT) && (kHeld & KEY_L) && (kHeld & KEY_R))) quit();
	#endif
	
	#if defined LIN || defined __APPLE__
		drawFramebuffer();
	#endif
	
	#ifdef PSP
		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);
		
		keys.keys1 = 0x0f;
		keys.keys2 = 0x0f;
		
		if(pad.Buttons & PSP_CTRL_SQUARE) keys.b = 0;
		if(pad.Buttons & PSP_CTRL_CROSS) keys.a = 0;
		if(pad.Buttons & PSP_CTRL_START) keys.start = 0;
		if(pad.Buttons & PSP_CTRL_SELECT) keys.select = 0;
		if(pad.Buttons & PSP_CTRL_UP) keys.up = 0;
		if(pad.Buttons & PSP_CTRL_DOWN) keys.down = 0;
		if(pad.Buttons & PSP_CTRL_LEFT) keys.left = 0;
		if(pad.Buttons & PSP_CTRL_RIGHT) keys.right = 0;
		
		sceDisplayWaitVblankStart();
	#endif
	
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x40;
	
	ticks += 12;
}

void lcdStat(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x48;
	
	ticks += 12;
}

void timer(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x50;
	
	ticks += 12;
}

void serial(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x58;
	
	ticks += 12;
}

void joypad(void) {
	interrupt.master = 0;
	writeShortToStack(registers.pc);
	registers.pc = 0x60;
	
	ticks += 12;
}

void returnFromInterrupt(void) {
	interrupt.master = 1;
	registers.pc = readShortFromStack();
}
