#include "ps4.h"

//#include "controller.h"

static int sock;

unsigned int buttons;

void initController(void) {
	struct sockaddr_in server;
	
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IN_ADDR_ANY;
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	sock = sceNetSocket("pongController", AF_INET, SOCK_DGRAM, 0);
	
	// Set non-blocking
	int nonBlocking = 1;
	sceNetSetsockopt(sock, SOL_SOCKET, SO_NBIO, &nonBlocking, sizeof(nonBlocking));
	
	sceNetBind(sock, (struct sockaddr *)&server, sizeof(server));
}

void readController(void) {
	while(sceNetRecv(sock, &buttons, sizeof(buttons), 0) > 0);
}

void deinitController(void) {
	sceNetSocketClose(sock);
}
