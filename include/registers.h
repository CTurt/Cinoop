#pragma once

#include "platform.h"

struct registers {
	struct {
		union {
			struct {
				#ifdef LITTLE_E
					unsigned char f;
					unsigned char a;
				#else
					unsigned char a;
					unsigned char f;
				#endif
			};
			unsigned short af;
		};
	};
	
	struct {
		union {
			struct {
				#ifdef LITTLE_E
					unsigned char c;
					unsigned char b;
				#else
					unsigned char b;
					unsigned char c;
				#endif
			};
			unsigned short bc;
		};
	};
	
	struct {
		union {
			struct {
				#ifdef LITTLE_E
					unsigned char e;
					unsigned char d;
				#else
					unsigned char d;
					unsigned char e;
				#endif
			};
			unsigned short de;
		};
	};
	
	struct {
		union {
			struct {
				#ifdef LITTLE_E
					unsigned char l;
					unsigned char h;
				#else
					unsigned char h;
					unsigned char l;
				#endif
			};
			unsigned short hl;
		};
	};
	
	unsigned short sp;
	unsigned short pc;
} extern registers;
