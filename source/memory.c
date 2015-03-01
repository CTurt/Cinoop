#include "gpu.h"
#include "interrupts.h"
#include "debug.h"

#include "memory.h"

unsigned char *cart;
unsigned char io[0x100];
unsigned char vram[0x2000];
unsigned char oam[0x100];
unsigned char ram[0x2000];
unsigned char zeroPage[0x80];

unsigned char readByte(unsigned short address) {
	// Set read breakpoints here
	//if(address == 0x0300) {
	// realtimeDebuggerEnable = 1;
	//}
	
	if(!cart) return 0;
	
	else if(address <= 0x7fff)
		return cart[address];
	
	else if(address >= 0xa000 && address <= 0xbfff)
		return cart[address - 0xa000];
	
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
	
	else if(address == 0xff40) return gpu.control;
	else if(address == 0xff42) return gpu.scrollY;
	else if(address == 0xff43) return gpu.scrollX;
	else if(address == 0xff44) return gpu.scanline; // read only
	
	else if(address == 0xff0f) return interrupt.flags;
	else if(address == 0xffff) return interrupt.enable;
	
	return 0;
}

unsigned short readShort(unsigned short address) {
	return readByte(address) | (readByte(address + 1) << 8);
}

void writeByte(unsigned short address, unsigned char value) {
	// Set write breakpoints here
	//if(address == 0x0300) {
	//	realtimeDebuggerEnable = 1;
	//}
	
	if(!cart) return;
	
	else if(address >= 0x8000 && address <= 0x9fff) {
		vram[address - 0x8000] = value;
		updateTile(address, value);
	}
	
	if(address >= 0xc000 && address <= 0xdfff)
		ram[address - 0xc000] = value;
	
	else if(address >= 0xe000 && address <= 0xfdff)
		ram[address - 0xe000] = value;
	
	else if(address >= 0xfe00 && address <= 0xfeff)
		oam[address - 0xfe00] = value;
		
	else if(address >= 0xff80 && address <= 0xfffe)
		zeroPage[address - 0xff80] = value;
	
	else if(address == 0xff40) gpu.control = value;
	else if(address == 0xff42) gpu.scrollY = value;
	else if(address == 0xff43) gpu.scrollX = value;
	else if(address == 0xff47) gpu.bgPalette = value; // write only
	
	else if(address == 0xff0f) interrupt.flags = value;
	else if(address == 0xffff) interrupt.enable = value;
}

void writeShort(unsigned short address, unsigned short value) {
	writeByte(address, (unsigned char)(value & 0xff00));
	writeByte(address + 1, (unsigned char)(value & 0x00ff));
}
