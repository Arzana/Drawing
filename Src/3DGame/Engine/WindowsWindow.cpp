#include <stdio.h>
#include <vector>
#include "WindowsWindow.h"
#include "Utils.h"
#include "Color.h"

using namespace std;

vector<winWnd*> wnds;

WindowsWindow::WindowsWindow(const char * t, const uint w, const uint h)
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
	printf("Starting windows window creation.\n");

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
		printf("Window registration failed(%d)!\n", GetLastError());
		return 1;
	}
	
	printf("Window registration successfull.\n");
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(0), NULL);

	if (!hwnd)
	{
		printf("Window creation failed(%d)!\n", GetLastError());
		return 1;
	}

	printf("Window creation successfull.\n");
	return 0;
}

int WindowsWindow::BmpInit(void)
{
	const uint depth = 3;
	printf("Starting bitmap creation.\n");

	hbmp = NULL;
	bits = malloc_s(octet, width * height * depth);
	memset(bits, 0, width * height * depth);

	BITMAPINFOHEADER bmih
	{
		sizeof(BITMAPINFOHEADER),
		width, -(int)height,
		1,
		sizeof(clr) << 3,
		BI_RGB,
		0,
		10, 10,
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
		printf("Bitmap creation failed!\n");
		PostQuitMessage(0);
		return 1;
	}

	printf("Bitmap creation successfull.\n");
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
		printf("WndProc(%d) called from unknown window!\n", msg);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	else return wnd->WndProc(msg, wParam, lParam);
}