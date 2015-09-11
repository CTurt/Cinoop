#pragma once

#define argc __argc
#define argv __argv

void quit(void);

#ifdef WIN
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

#if defined LIN || defined __APPLE__
	extern Display *dpy;
	extern Window root;
	extern GLint att[];
	extern XVisualInfo *vi;
	extern Colormap cmap;
	extern XSetWindowAttributes swa;
	extern Window win;
	extern GLXContext glc;
	extern XWindowAttributes gwa;
	extern XEvent xev;
#endif
