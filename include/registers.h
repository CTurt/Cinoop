#pragma once

struct registers {
	struct {
		union {
			struct {
				unsigned char a;
				unsigned char f;
			};
			unsigned short af;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char b;
				unsigned char c;
			};
			unsigned short bc;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char d;
				unsigned char e;
			};
			unsigned short de;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char h;
				unsigned char l;
			};
			unsigned short hl;
		};
	};
	
	unsigned short sp;
	unsigned short pc;
	
	unsigned char ime;
} extern registers;
