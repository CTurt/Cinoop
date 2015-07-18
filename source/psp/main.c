#include <stdlib.h>
#include <math.h>

#include "platform.h"

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"

#include "main.h"

PSP_MODULE_INFO("cinoop", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void quit(void) {
	exit(0);
}

int main(int argc, char **argv) {
	pspDebugScreenInit();
	sceDisplaySetFrameBuf((void *)framebuffer, 512, 1, 0);
	
	printf("Cinoop");
	
	framebuffer[100 + 100 * 512] = palette[0];
	framebuffer[100 + 101 * 512] = palette[1];
	framebuffer[100 + 102 * 512] = palette[2];
	framebuffer[100 + 103 * 512] = palette[3];
	
	return 0;
}
