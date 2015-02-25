#include "memory.h"

unsigned char *cart;
unsigned char io[0x100];
unsigned char vram[0x2000];
unsigned char oam[0x100];
unsigned char ram[0x2000];

unsigned char read(unsigned short address) {
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
	
	/*else if(address >= 0xff80 && address <= 0xfffe)
		return highRam[0xff & address];*/
	
	return 0;
}

void write(unsigned short address, unsigned char value) {
	if(!cart) return;
	
	if(address >= 0xc000 && address <= 0xdfff)
		ram[address - 0xc000] = value;
	
	else if(address >= 0xfe00 && address <= 0xfeff)
		oam[address - 0xfe00] = value;
	
	//else if(address >= 0xff80 && address <= 0xfffe)
		//highRam[0xff & address] = value;
}
