#pragma once

#ifdef WIN
	#include <windows.h>
	#define LITTLE_ENDIAN
#endif

#ifndef WIN
	#define MessageBox(a, b, c, d)
#endif

#ifdef DS
	#define LITTLE_E
#endif

#ifdef GC
	#define BIG_E
#endif

#ifdef DS3
	#include <3ds.h>
	#define LITTLE_E
#endif
