#define _USE_WINDOW_BASE_INTERNAL

#include <cstdio>
#include "Utils.h"
#include "WindowBase.h"
#include "Color.h"

WNDPROC WndProc;
HBITMAP hbmp;

WindowBase::WindowBase(const char * title, const uint width, const uint height, WNDPROC wndProc)
{
	this->title = title;
	this->width = width;
	this->height = height;
	WndProc = wndProc;

	if (WndInit("WindowBase", title)) exit(0);
	if (BmpInit()) exit(0);
}

WindowBase::~WindowBase(void)
{
	pixels = NULL;
	free_s(bits);
}

const HWND WindowBase::GetHWND(void) const
{
	return hwnd;
}

int WindowBase::WndInit(const char g_szClassName[], const char title[])
{
	printf("Starting windows window creation.\n");

	WNDCLASSEX wndc = 
	{
		sizeof(WNDCLASSEX),
		CS_DBLCLKS,
		WndProc,
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
		printf("Window registration failed!\n");
		return 1;
	}
	else printf("Window registration successfull.\n");

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(0), NULL);

	if (!hwnd)
	{
		printf("Window creation failed!\n");
		return 1;
	}
	else printf("Window creation successfull.\n");

	return 0;
}

int WindowBase::BmpInit(void)
{
	const uint depth = 3;
	printf("Starting bitmap creation.\n");

	hbmp = NULL;
	bits = malloc_s(octet, width * height * depth);

	for (size_t i = 0; i < width * height * depth; i++)
	{
		bits[i] = 0;
	}

	BITMAPINFOHEADER bmih
	{
		sizeof(BITMAPINFOHEADER),
		width, -(int)height,
		1,
		sizeof(Color) << 3,
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
	else printf("Bitmap creation successfull.\n");

	memcpy(pixels, bits, sizeof(pixels));
	ReleaseDC(hwnd, hdc);
	return 0;
}