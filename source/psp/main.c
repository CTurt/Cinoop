#include <stdlib.h>
#include <math.h>

#include "platform.h"

PSP_MODULE_INFO("cinoop", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void quit(void) {
	exit(0);
}

int main(int argc, char **argv) {
	pspDebugScreenInit();
	
	printf("Cinoop");
	
	return 0;
}
