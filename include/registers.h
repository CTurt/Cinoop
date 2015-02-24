#pragma once

struct registers {
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;
	unsigned char e;
	unsigned char h;
	unsigned char l;
	unsigned short pc;
	unsigned char flags;
	unsigned short sp;
	unsigned char ime;
} extern registers;
