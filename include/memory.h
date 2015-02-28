#pragma once

extern unsigned char *cart;

extern unsigned char io[0x100];
extern unsigned char vram[0x2000];
extern unsigned char oam[0x100];
extern unsigned char ram[0x2000];
extern unsigned char zeroPage[0x80];

unsigned char readByte(unsigned short address);
unsigned short readShort(unsigned short address);

void writeByte(unsigned short address, unsigned char value);
void writeShort(unsigned short address, unsigned short value);
