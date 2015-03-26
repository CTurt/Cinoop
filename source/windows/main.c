#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <direct.h>
#include <gl/gl.h>
#include <LDFS.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "opengl.h"
#include "debug.h"
#include "keys.h"

#include "main.h"

void quit(void) {
	exit(1);
}

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
	
	srand(time(NULL));
	reset();
	
	while(1) {
		if(LDFS_NoFramerateUpdate()) {
			cpuStep();
			gpuStep();
			interruptStep();
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
				
				case 'X':
					if(!(lParam & 0x40000000)) {
						keys.a = 0;
						stopped = 0;
					}
					break;
				
				case 'Z':
					if(!(lParam & 0x40000000)) {
						keys.b = 0;
						stopped = 0;
					}
					break;
				
				case VK_BACK:
					if(!(lParam & 0x40000000)) {
						keys.select = 0;
						stopped = 0;
					}
					break;
				
				case VK_RETURN:
					if(!(lParam & 0x40000000)) {
						keys.start = 0;
						stopped = 0;
					}
					break;
				
				case VK_UP:
					if(!(lParam & 0x40000000)) {
						keys.up = 0;
						stopped = 0;
					}
					break;
				
				case VK_DOWN:
					if(!(lParam & 0x40000000)) {
						keys.down = 0;
						stopped = 0;
					}
					break;
				
				case VK_LEFT:
					if(!(lParam & 0x40000000)) {
						keys.left = 0;
						stopped = 0;
					}
					break;
				
				case VK_RIGHT:
					if(!(lParam & 0x40000000)) {
						keys.right = 0;
						stopped = 0;
					}
					break;
				
				case VK_MULTIPLY:
					if(!(lParam & 0x40000000)) {
						//reset();
						
						char *path = malloc(1024);
						char *exe = malloc(1024);
						if(!path) break;
						if(!exe) break;
						memset(path, 0, 1024);
						memset(exe, 0, 1024);
						GetModuleFileName(0, exe, 1023);
						_getcwd(path, 1023);
						ShellExecute(NULL, NULL, exe, "tetris.gb", path, SW_SHOWDEFAULT);
						free(path);
						free(exe);
						exit(0);
					}
					break;
				
				case VK_SPACE:
					if(!(lParam & 0x40000000)) realtimeDebugEnable = 1;
					break;
			}
			return 0;
			
		case WM_KEYUP:
			switch(wParam) {
				case 'X':
					keys.a = 1;
					stopped = 0;
					break;
				
				case 'Z':
					keys.b = 1;
					stopped = 0;
					break;
				
				case VK_BACK:
					keys.select = 1;
					stopped = 0;
					break;
				
				case VK_RETURN:
					keys.start = 1;
					stopped = 0;
					break;
				
				case VK_UP:
					keys.up = 1;
					stopped = 0;
					break;
				
				case VK_DOWN:
					keys.down = 1;
					stopped = 0;
					break;
				
				case VK_LEFT:
					keys.left = 1;
					stopped = 0;
					break;
				
				case VK_RIGHT:
					keys.right = 1;
					stopped = 0;
					break;
			}
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
