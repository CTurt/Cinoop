#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "debug.h"
#include "keys.h"

#include "main.h"

Display *dpy;
Window root;
GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;

void quit(void) {
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	exit(0);
}

int main(int argc, char **argv) {
	char *filename = NULL;
	
	printf("Starting...\n");
	
	dpy = XOpenDisplay(NULL);
	
	if(dpy == NULL) {
		printf("Cannot connect to X server!\n");
		exit(0);
	}
	
	root = DefaultRootWindow(dpy);
	
	vi = glXChooseVisual(dpy, 0, att);
	
	if(vi == NULL) {
		printf("No appropriate visual found!\n");
		exit(0);
	}
	
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;
	
	win = XCreateWindow(dpy, root, 0, 0, 160, 144, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Cinoop");
	
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	
	printf("argc = %d\n", argc);
	int i;
	for(i = 1; i < argc; i++) {
		filename = argv[i];
	}
	
	if(filename == NULL) {
		printf("No ROM input\n");
		
		quit();
		return 1;
	}
	
	printf("Loading file \"%s\"...\n", filename);
	
	if(!loadROM(filename)) {
		printf("Failed!\n");
		
		quit();
		return 1;
	}
	
	printf("Passed!\n");
	
	srand(time(NULL));
	reset();
	
	while(1) {
		if(XPending(dpy)) {
			XNextEvent(dpy, &xev);
			
			if(xev.type == KeyPress || xev.type == KeyRelease) {
				//printf("%d (%d, %d) \n", xev.xkey.keycode, KeyPress == xev.type ? 1 : 0, KeyRelease == xev.type ? 1 : 0);
				switch(xev.xkey.keycode) {
					//case XK_BackSpace:
					case 59:
						keys.select = (xev.type == KeyPress ? 1 : 0);
                        printf("%d\n", keys.select);
						break;
					
					//case XK_Return:
					case 44:
						keys.start = (xev.type == KeyPress ? 1 : 0);
						break;
					
					//case XK_z:
					case 14:
						keys.b = (xev.type == KeyPress);
                        printf("%d\n", keys.b);
						break;
					
					//case XK_x:
					case 15:
						keys.a = (xev.type == KeyPress);
						break;
					
					//case XK_Left:
					case 131:
						keys.left = (xev.type == KeyPress);
						break;
					
					//case XK_Right:
					case 132:
						keys.right = (xev.type == KeyPress);
						break;
					
					//case XK_Up:
					case 134:
						keys.up = (xev.type == KeyPress);
						break;
					
					//case XK_Down:
					case 133:
						keys.down = (xev.type == KeyPress);
						break;
				}
			}
		}
		
		cpuStep();
		gpuStep();
		interruptStep();
	}
	
	quit();
	
	return 0;
}
