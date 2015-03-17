#include <stdio.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "rom.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupts.h"
#include "opengl.h"
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
	
	reset();
	
	while(1) {
		//XNextEvent(dpy, &xev);
		
		cpuStep();
		gpuStep();
		interruptStep();
		
		/*if(xev.type == Expose) {
				XGetWindowAttributes(dpy, win, &gwa);
				glViewport(0, 0, gwa.width, gwa.height);
				glXSwapBuffers(dpy, win);
		}*/
		
		//else 
		if(xev.type == KeyPress) {
			break;	
		}
	}
	
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	exit(0);
	
	return 0;
}
