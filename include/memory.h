#pragma once

#include <stdlib.h>

extern const unsigned char ioReset[0x100];

extern unsigned char *cart;
extern unsigned char sram[0x2000];
extern unsigned char io[0x100];
extern unsigned char vram[0x2000];
extern unsigned char oam[0x100];
extern unsigned char wram[0x2000];
extern unsigned char hram[0x80];

extern unsigned char romBank;
extern unsigned char ramBank;
extern unsigned char ramEnable;
extern unsigned char expansionMode;

void copy(unsigned short destination, unsigned short source, size_t length);

unsigned char readByte(unsigned short address);
unsigned short readShort(unsigned short address);
unsigned short readShortFromStack(void);

void writeByte(unsigned short address, unsigned char value);
void writeShort(unsigned short address, unsigned short value);
void writeShortToStack(unsigned short value);
