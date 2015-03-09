#pragma once

struct keys1 {
	unsigned char start : 1;
	unsigned char select : 1;
	unsigned char b : 1;
	unsigned char a : 1;
};

struct keys2 {
	unsigned char down : 1;
	unsigned char up : 1;
	unsigned char left : 1;
	unsigned char right : 1;
};

struct keys {
	union {
		struct keys1;
		unsigned char keys1 : 4;
	};
	
	union {
		struct keys2;
		unsigned char keys2 : 4;
	};
} extern keys;
