#include "ps4.h"

#include "memory.h"
#include "rom.h"
#include "registers.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"
#include "display.h"

void quit(void) { while(1); }

void getFramebuffer(void) {
	struct memoryRegionInfo info;
	struct otherMemoryRegionInfo otherInfo;
	
	void *m = NULL;
	
	int i;
	for(i = 0; i < 16; i++) {
		// Find base of next mapping
		getOtherMemoryInfo(m, 1, &otherInfo);
		
		// Get more info about this mapping
		getMemoryInfo(otherInfo.base, &info);
		
		// Search mappings 14 and 15 if they are readable
		if((i == 14 || i == 15) && (info.flags & PROT_CPU_READ)) {
			framebuffer = info.base;
			
			// Search for the colour we filled the framebuffer with
			while(framebuffer < (unsigned int *)info.end) {
				if(
					framebuffer[0] == 0xffc2c2c2 &&
					framebuffer[1] == 0xffc2c2c2
				) break;
				
				framebuffer++;
			}
		}
		
		m = info.end;
	}
}

int sock;

void *cinoop(void *arg) {
	// srand
	
	initController();
	
	reset();
	
	while(1) {
		cpuStep();
		gpuStep();
		interruptStep();
	}
	
	deinitController();
	
	return NULL;
}

int _main(void) {
	// Load modules
	initKernel();
	
	initLibc();
	initNetwork();
	initUsb();
	initPthread();
	
	
	// Init debug
	/*struct sockaddr_in server;
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));*/
	
	ps4loadROM();
	//memcpy(DATA, cart, 512);
	//memcpy(DATA, cart + 32 * 1024 - 512, 512);
	
	
	getFramebuffer();
	
	// Create Cinoop thread
	ScePthread thread;
	scePthreadCreate(&thread, NULL, cinoop, NULL, "cinoop");
	
	//sceNetSocketClose(sock);
	
	return 0;
}
