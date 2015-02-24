#include <stdio.h>
#include <string.h>

#include "rom.h"

unsigned char loadROM(char *filename) {
	FILE *f;
	size_t length;
	
	unsigned char ROM[0x180];
	char name[17];
	
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
	
	memset(name, '\0', 17);
	for(i = 0; i < 16; i++) {
		if(ROM[i + 0x134] == 0x80 || ROM[i + 0x134] == 0xc0) name[i] = '\0';
		else name[i] = ROM[i + 0x134];
	}
	
	printf("Internal ROM name: %s\n", name);
	
	fclose(f);
	
	return 1;
}
