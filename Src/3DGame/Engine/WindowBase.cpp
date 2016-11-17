#define _USE_WINDOW_BASE_INTERNAL

#include <cstdio>
#include "Utils.h"
#include "WindowBase.h"

WindowBase::WindowBase(const char * title, const uint width, const uint height)
{
	this->title = title;
	this->width = width;
	this->height = height;

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
		this->WndProc,
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
		24,
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

LRESULT WindowBase::WndProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DeleteObject(hbmp);
		DestroyWindow(hwnd);
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