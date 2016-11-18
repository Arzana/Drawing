#pragma once

#define _WIN32_WINNT	0x0501

#include <Windows.h>
#include "MathEx.h"

#undef far
#undef near

#ifdef _USE_WINDOW_BASE_INTERNAL
extern HBITMAP hbmp;
#endif

class WindowBase
{
public:
	uint width, height;
	void *pixels;

	WindowBase(const char *title, const uint width, const uint height, WNDPROC wndProc);
	~WindowBase(void);
	const HWND GetHWND(void) const;
private:
	const char *title;
	HWND hwnd;
	BITMAPINFO bmpinf;
	octet *bits;

	int WndInit(const char g_szClassName[], const char title[]);
	int BmpInit(void);
};