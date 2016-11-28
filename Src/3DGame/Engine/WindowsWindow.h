#pragma once

#include <Windows.h>
#include "MathEx.h"

typedef struct WindowsWindow
{
public: 
	const uint width, height;

	WindowsWindow(const char *title, const uint width, const uint height);
	~WindowsWindow(void);
protected:
	HWND hwnd;
	void *pixels;
	bool *isRunning;

	inline void ShowWnd(void) { ShowWindow(hwnd, SW_SHOWDEFAULT); }
	inline void HideWnd(void) { CloseWindow(hwnd); }
	inline void PostMsg(uint msg, WPARAM wParam, LPARAM lParam) { PostMessage(hwnd, msg, wParam, lParam); }
	virtual LRESULT CALLBACK WndProc(uint msg, WPARAM wParam, LPARAM lParam);
private:
	BITMAPINFO bmpinf;
	HBITMAP hbmp;
	const char *title;
	octet *bits;

	friend LRESULT CALLBACK f_WndProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam);
	int WndInit(const char g_szClassName[], const char title[]);
	int BmpInit(void);
} winWnd;