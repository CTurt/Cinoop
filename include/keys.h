#pragma once

#include "platform.h"

struct keys1 {
	#ifdef LITTLE_E
		unsigned char a : 1;
		unsigned char b : 1;
		unsigned char select : 1;
		unsigned char start : 1;
	#else
		unsigned char start : 1;
		unsigned char select : 1;
		unsigned char b : 1;
		unsigned char a : 1;
	#endif
};

struct keys2 {
	#ifdef LITTLE_E
		unsigned char right : 1;
		unsigned char left : 1;
		unsigned char up : 1;
		unsigned char down : 1;
	#else
		unsigned char down : 1;
		unsigned char up : 1;
		unsigned char left : 1;
		unsigned char right : 1;
	#endif
};

struct keys {
	union {
		struct {
			union {
				struct keys1;
				unsigned char keys1 : 4;
			};
			
			union {
				struct keys2;
				unsigned char keys2 : 4;
			};
		};
		
		unsigned char c;
	};
} extern keys;
