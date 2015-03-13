#pragma once

#define argc __argc
#define argv __argv

void quit(void);

#ifdef WIN
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
