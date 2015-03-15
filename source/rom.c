#include <stdio.h>
#include <string.h>
#include <math.h>

#include "platform.h"

#include "memory.h"
#include "debug.h"

#include "rom.h"

#ifdef DS3
	Handle fileHandle;
#endif

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
	char name[17];
	enum romType type;
	int romSize;
	int ramSize;
	
	int i;
	
	#ifdef DS3
	u64 length;
	u32 bytesRead;
	
	FS_archive sdmcArchive = (FS_archive){ARCH_SDMC, (FS_path){PATH_EMPTY, 1, (u8*)""}};
	FS_path filePath = FS_makePath(PATH_CHAR, filename); // /filename
	
	Result ret = FSUSER_OpenFileDirectly(NULL, &fileHandle, sdmcArchive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if(ret) return false;
	
	ret = FSFILE_GetSize(fileHandle, &length);
	if(ret) return false;
	
	ret = FSFILE_Read(fileHandle, &bytesRead, 0x0, cart, length);
	if(ret || length != bytesRead) return false;
	
	memset(name, '\0', 17);
	for(i = 0; i < 16; i++) {
		if(cart[i + ROM_OFFSET_NAME] == 0x80 || cart[i + ROM_OFFSET_NAME] == 0xc0) name[i] = '\0';
		else name[i] = cart[i + ROM_OFFSET_NAME];
	}
	
	printf("Internal ROM name: %s\n", name);
	
	if(strcmp(name, "TETRIS") == 0) tetrisPatch = 1;
	
	type = cart[ROM_OFFSET_TYPE];
	
	if(!romTypeString[type]) {
		printf("Unknown ROM type: %#02x\n", type);
		return false;
	}
	
	printf("ROM type: %s\n", romTypeString[type]);
	
	if(type != ROM_PLAIN) {
		printf("Only 32KB games with no mappers are supported!\n");
		return false;
	}
	
	romSize = cart[ROM_OFFSET_ROM_SIZE];
	
	if((romSize & 0xF0) == 0x50) romSize = (int)pow(2, ((0x52) & 0xF) + 1) + 64;
	else romSize = (int)pow(2, romSize + 1);
	
	printf("ROM size: %dKB\n", romSize * 16);
	
	if(romSize * 16 != 32) {
		printf("Only 32KB games with no mappers are supported!\n");
		return false;
	}
	
	if(length != romSize * 16 * 1024) {
		printf("ROM filesize does not equal ROM size!\n");
		return false;
	}
	
	ramSize = cart[ROM_OFFSET_RAM_SIZE];
	
	ramSize = (int)pow(4, ramSize) / 2;
	printf("RAM size: %dKB\n", ramSize);
	
	ramSize = ceil(ramSize / 8.0f);
	
	ret = FSFILE_Close(fileHandle);
	if(ret) return false;
	
	return 1;
#else
	FILE *f;
	size_t length;
	
	unsigned char header[0x180];
	
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
	
	if(strcmp(name, "TETRIS") == 0) tetrisPatch = 1;
	
	type = header[ROM_OFFSET_TYPE];
	
	if(!romTypeString[type]) {
		printf("Unknown ROM type: %#02x\n", type);
		fclose(f);
		return 0;
	}
	
	printf("ROM type: %s\n", romTypeString[type]);
	
	if(type != ROM_PLAIN) {
		printf("Only 32KB games with no mappers are supported!\n");
		fclose(f);
		return 0;
	}
	
	romSize = header[ROM_OFFSET_ROM_SIZE];
	
	if((romSize & 0xF0) == 0x50) romSize = (int)pow(2, ((0x52) & 0xF) + 1) + 64;
	else romSize = (int)pow(2, romSize + 1);
	
	printf("ROM size: %dKB\n", romSize * 16);
	
	if(romSize * 16 != 32) {
		printf("Only 32KB games with no mappers are supported!\n");
		fclose(f);
		return 0;
	}
	
	if(length != romSize * 16 * 1024) {
		printf("ROM filesize does not equal ROM size!\n");
		//fclose(f);
		//return 0;
	}
	
	ramSize = header[ROM_OFFSET_RAM_SIZE];
	
	ramSize = (int)pow(4, ramSize) / 2;
	printf("RAM size: %dKB\n", ramSize);
	
	ramSize = ceil(ramSize / 8.0f);
	
	/*cart = malloc(length);
	if(!cart) {
		printf("Could not allocate memory!\n");
		fclose(f);
		return 0;
	}*/
	
	rewind(f);
	fread(cart, length, 1, f);
	
	fclose(f);
	
	return 1;
	#endif
}

void unloadROM(void) {
	//free(cart);
}
