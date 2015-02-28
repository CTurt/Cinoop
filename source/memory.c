#include "interrupts.h"

#include "memory.h"

unsigned char *cart;
unsigned char io[0x100];
unsigned char vram[0x2000];
unsigned char oam[0x100];
unsigned char ram[0x2000];
unsigned char zeroPage[0x80];

unsigned char readByte(unsigned short address) {
	if(!cart) return 0;
	
	if(address <= 0x7fff || (address >= 0xa000 && address <= 0xbfff))
		return cart[address];
	
	else if(address >= 0x8000 && address <= 0x9fff)
		return vram[address - 0x8000];
	
	else if(address >= 0xc000 && address <= 0xdfff)
		return ram[address - 0xC000];
	
	else if(address >= 0xe000 && address <= 0xfdff)
		return ram[address - 0xe000];
	
	else if(address >= 0xfe00 && address <= 0xfeff)
		return oam[address - 0xfe00];
	
	else if(address >= 0xff80 && address <= 0xfffe)
		return zeroPage[address - 0xff80];
	
	else if(address == 0xffff) return interrupt.enable;
	
	return 0;
}

unsigned short readShort(unsigned short address) {
	return readByte(address) | (readByte(address + 1) << 8);
}

void writeByte(unsigned short address, unsigned char value) {
	if(!cart) return;
	
	if(address >= 0xc000 && address <= 0xdfff)
		ram[address - 0xc000] = value;
	
	else if(address >= 0xfe00 && address <= 0xfeff)
		oam[address - 0xfe00] = value;
		
	else if(address >= 0xff80 && address <= 0xfffe)
		zeroPage[address - 0xff80] = value;
	
	else if(address == 0xffff) interrupt.enable = value;
}

void writeShort(unsigned short address, unsigned short value) {
	writeByte(address, (unsigned char)(value & 0xff00));
	writeByte(address + 1, (unsigned char)(value & 0x00ff));
}
