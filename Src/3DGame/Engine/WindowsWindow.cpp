#include <stdio.h>
#include <vector>
#include "WindowsWindow.h"
#include "Utils.h"
#include "Color.h"
#include "WinLogger.h"

using namespace std;

vector<winWnd*> wnds;

WindowsWindow::WindowsWindow(const char * t, uint w, uint h)
	: width(w), height(h), title(t), isRunning(new bool(true))
{
	wnds.push_back(this);
	if (WndInit("WindowsWindow", title)) exit(1);
	if (BmpInit()) exit(1);
}

WindowsWindow::~WindowsWindow(void)
{
	free_s(bits);
	pixels = NULL;
	delete isRunning;

	DeleteObject(hbmp);
	DestroyWindow(hwnd);

	for (size_t i = 0; i < wnds.size(); i++)
	{
		if (wnds.at(i)->hwnd == hwnd)
		{
			wnds.erase(wnds.begin() + i);
			break;
		}
	}
}

LRESULT CALLBACK WindowsWindow::WndProc(uint msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		*isRunning = false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		PAINTSTRUCT		ps;
		HDC				hdc;
		BITMAP			bmp;
		HDC				hdcMem;
		HGDIOBJ			oldBmp;

		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		oldBmp = SelectObject(hdcMem, hbmp);

		GetObject(hbmp, sizeof(BITMAP), &bmp);
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBmp);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WindowsWindow::WndInit(const char g_szClassName[], const char title[])
{
	LogMsg_Eng("Starting windows window registration.");

	WNDCLASSEX wndc =
	{
		sizeof(WNDCLASSEX),
		CS_DBLCLKS,
		f_WndProc,
		0, 0,
		GetModuleHandle(0),
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		HBRUSH(COLOR_WINDOW + 1),
		NULL,
		g_szClassName,
		LoadIcon(NULL, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wndc))
	{
		LogErr_Eng("Window registration failed(%d)!", GetLastError());
		return 1;
	}
	
	LogVer_Eng("Window registration successfull.");
	LogMsg_Eng("Starting window creation.");
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(0), NULL);

	if (!hwnd)
	{
		LogErr_Eng("Window creation failed(%d)!", GetLastError());
		return 1;
	}

	LogVer_Eng("Window creation successfull.");
	return 0;
}

int WindowsWindow::BmpInit(void)
{
	const uint depth = 3;
	LogMsg_Eng("Starting bitmap creation.");

	hbmp = NULL;
	bits = malloc_s(octet, width * height * depth);
	memset(bits, 0, width * height * depth);

	BITMAPINFOHEADER bmih
	{
		sizeof(BITMAPINFOHEADER),
		(long)width, -(long)height,
		1,
		sizeof(clr) << 3,
		BI_RGB,
		0,
		10L, 10L,
		0, 0
	};

	ZeroMemory(&bmpinf, sizeof(BITMAPINFO));
	bmpinf.bmiHeader = bmih;
	bmpinf.bmiColors->rgbBlue = 0;
	bmpinf.bmiColors->rgbGreen = 0;
	bmpinf.bmiColors->rgbRed = 0;
	bmpinf.bmiColors->rgbReserved = 0;

	pixels = (void*)&(bits[0]);

	HDC hdc = GetDC(hwnd);
	hbmp = CreateDIBSection(hdc, &bmpinf, DIB_RGB_COLORS, &pixels, NULL, 0);
	if (!hbmp)
	{
		LogErr_Eng("Bitmap creation failed(%d)!", GetLastError());
		PostQuitMessage(0);
		return 1;
	}

	LogVer_Eng("Bitmap creation successfull.");
	memcpy(pixels, bits, sizeof(pixels));
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT CALLBACK f_WndProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam)
{
	winWnd *wnd = NULL;
	for (size_t i = 0; i < wnds.size(); i++)
	{
		if (wnds.at(i)->hwnd == hwnd)
		{
			wnd = wnds.at(i);
			break;
		}
	}

	if (!wnd)
	{
		LogWar_Eng("WndProc(%d) called from unknown window!", msg);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	else return wnd->WndProc(msg, wParam, lParam);
}