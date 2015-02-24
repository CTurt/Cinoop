#pragma once

extern unsigned char *cart;

extern unsigned char io[0x100];
extern unsigned char vram[0x2000];
extern unsigned char oam[0x100];
extern unsigned char ram[0x2000];

unsigned char read(unsigned short address);
