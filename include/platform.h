#pragma once

#ifdef PS4
	#include "ps4.h"
	#define LITTLE_E
#endif

#ifdef WIN
	#include <windows.h>
	#include <gl/gl.h>
	#include <LDFS.h>
	#define LITTLE_E
#endif

#ifndef WIN
	#define MessageBox(a, b, c, d)
#endif

#ifdef DS
	#include <nds.h>
	#define LITTLE_E
#endif

#ifdef GC
	#include <gccore.h>
	#define BIG_E
#endif

#ifdef DS3
	#include <3ds.h>
	#define LITTLE_E
#endif

#if defined LIN || defined __APPLE__
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glu.h>
	#define LITTLE_E
#endif

#ifdef PSP
	#include <pspkernel.h>
	#include <pspdebug.h>
	#include <pspdisplay.h>
	#include <pspctrl.h>
	#define printf pspDebugScreenPrintf
	#define LITTLE_E
#endif
