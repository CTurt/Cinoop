#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <LDFS.h>

#include "rom.h"
#include "cpu.h"
#include "opengl.h"
#include "debug.h"

#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	char *filename = NULL;
	
	printf("Starting...\n");
	
	printf("LDFS_Init\n");
	LDFS_Init(WndProc);
	
	printf("LDFS_CreateWindow\n");
	LDFS_CreateWindow("Cinoop", 0, 160, 144);
	
	printf("argc = %d\n", argc);
	int i;
	for(i = 1; i < argc; i++) {
		filename = argv[i];
	}
	
	if(filename == NULL) {
		printf("No ROM input\n");
		
		LDFS_DestroyWindow();
		return 1;
	}
	
	printf("Loading file \"%s\"...\n", filename);
	
	if(!loadROM(filename)) {
		printf("Failed!\n");
		
		LDFS_DestroyWindow();
		return 1;
	}
	
	printf("Passed!\n");
	
	reset();
	
	while(1) {
		if(LDFS_NoFramerateUpdate()) {
			cpuStep();
		}
		else {
			unloadROM();
			LDFS_DestroyWindow();
			return 0;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_CREATE:
			return 0;
			
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			
		case WM_DESTROY:
			return 0;
			
		case WM_KEYDOWN:
			switch(wParam) {
				case VK_ESCAPE:
					printf("ESC pressed, quitting!\n");
					printRegisters();
					PostQuitMessage(0);
					return 0;
					break;
					
				case VK_SPACE:
					if(!(lParam & 0x40000000)) realtimeDebugEnable = 1;
					break;
			}
			return 0;
			
		case WM_KEYUP:
			switch(wParam) {
				case VK_SPACE:
					//holdingSpace = 0;
					break;
			}
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
