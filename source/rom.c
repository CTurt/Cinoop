#include <stdio.h>
#include <string.h>
#include <windows.h>

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
	
	unsigned char ROM[0x180];
	char name[17];
	enum romType type;
	
	int i;
	
	f = fopen(filename, "rb");
	if(!f) return 0;
	
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	if(length < 0x180) {
		fclose(f);
		return 0;
	}
	
	rewind(f);
	fread(ROM, 0x180, 1, f);
	
	fclose(f);
	
	memset(name, '\0', 17);
	for(i = 0; i < 16; i++) {
		if(ROM[i + ROM_OFFSET_NAME] == 0x80 || ROM[i + ROM_OFFSET_NAME] == 0xc0) name[i] = '\0';
		else name[i] = ROM[i + ROM_OFFSET_NAME];
	}
	
	printf("Internal ROM name: %s\n", name);
	
	type = ROM[ROM_OFFSET_TYPE];
	
	if(!romTypeString[type]) {
		printf("Unknown ROM type: %#02x\n", type);
		return 0;
	}
	
	printf("ROM type: %s\n", romTypeString[type]);
	
	return 1;
}
