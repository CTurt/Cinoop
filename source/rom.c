#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>

#include "memory.h"

#include "rom.h"

const char *romTypeString[256] = {
	[ROM_PLAIN] = "ROM_PLAIN",
	[ROM_MBC1] = "ROM_MBC1",
	[ROM_MBC1_RAM] = "ROM_MBC1",
	[ROM_MBC1_RAM_BATT] = "ROM_MBC1_RAM_BATT",
	[ROM_MBC2] = "ROM_MBC2",
	[ROM_MBC2_BATTERY] = "ROM_MBC2_BATTERY",
	[ROM_RAM] = "ROM_RAM",
	[ROM_RAM_BATTERY] = "ROM_RAM_BATTERY",
	[ROM_MMM01] = "ROM_MMM01",
	[ROM_MMM01_SRAM] = "ROM_MMM01_SRAM",
	[ROM_MMM01_SRAM_BATT] = "ROM_MMM01_SRAM_BATT",
	[ROM_MBC3_TIMER_BATT] = "ROM_MBC3_TIMER_BATT",
	[ROM_MBC3_TIMER_RAM_BATT] = "ROM_MBC3_TIMER_RAM_BATT",
	[ROM_MBC3] = "ROM_MBC3",
	[ROM_MBC3_RAM] = "ROM_MBC3_RAM",
	[ROM_MBC3_RAM_BATT] = "ROM_MBC3_RAM_BATT",
	[ROM_MBC5] = "ROM_MBC5",
	[ROM_MBC5_RAM] = "ROM_MBC5_RAM",
	[ROM_MBC5_RAM_BATT] = "ROM_MBC5_RAM_BATT",
	[ROM_MBC5_RUMBLE] = "ROM_MBC5_RUMBLE",
	[ROM_MBC5_RUMBLE_SRAM] = "ROM_MBC5_RUMBLE_SRAM",
	[ROM_MBC5_RUMBLE_SRAM_BATT] = "ROM_MBC5_RUMBLE_SRAM_BATT",
	[ROM_POCKET_CAMERA] = "ROM_POCKET_CAMERA",
	[ROM_BANDAI_TAMA5] = "ROM_BANDAI_TAMA5",
	[ROM_HUDSON_HUC3] = "ROM_HUDSON_HUC3",
	[ROM_HUDSON_HUC1] = "ROM_HUDSON_HUC1",
};

unsigned char loadROM(char *filename) {
	FILE *f;
	size_t length;
	
	unsigned char header[0x180];
	char name[17];
	enum romType type;
	int romSize;
	int ramSize;
	
	int i;
	
	f = fopen(filename, "rb");
	if(!f) return 0;
	
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	if(length < 0x180) {
		printf("ROM is too small!\n");
		fclose(f);
		return 0;
	}
	
	rewind(f);
	fread(header, 0x180, 1, f);
	
	memset(name, '\0', 17);
	for(i = 0; i < 16; i++) {
		if(header[i + ROM_OFFSET_NAME] == 0x80 || header[i + ROM_OFFSET_NAME] == 0xc0) name[i] = '\0';
		else name[i] = header[i + ROM_OFFSET_NAME];
	}
	
	printf("Internal ROM name: %s\n", name);
	
	type = header[ROM_OFFSET_TYPE];
	
	if(!romTypeString[type]) {
		printf("Unknown ROM type: %#02x\n", type);
		fclose(f);
		return 0;
	}
	
	printf("ROM type: %s\n", romTypeString[type]);
	
	romSize = header[ROM_OFFSET_ROM_SIZE];
	
	if((romSize & 0xF0) == 0x50) romSize = (int)pow(2, ((0x52) & 0xF) + 1) + 64;
	else romSize = (int)pow(2, romSize + 1);
	
	printf("ROM size: %dKB\n", romSize * 16);
	
	if(length != romSize * 16 * 1024) {
		printf("ROM filesize does not equal ROM size!\n");
		fclose(f);
		return 0;
	}
	
	ramSize = header[ROM_OFFSET_RAM_SIZE];
	
	ramSize = (int)pow(4, ramSize) / 2;
	printf("RAM size: %dKB\n", ramSize);
	
	ramSize = ceil(ramSize / 8.0f);
	
	cart = malloc(length);
	if(!cart) {
		printf("Could not allocate memory!\n");
		fclose(f);
		return 0;
	}
	
	rewind(f);
	fread(cart, length, 1, f);
	
	fclose(f);
	
	return 1;
}

void unloadROM(void) {
	free(cart);
}
