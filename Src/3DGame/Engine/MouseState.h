#pragma once

#include "MathEx.h"

#ifdef _WIN32
#include <Windows.h>
#endif

typedef struct MouseState
{
	bool leftDown, rightDown, middleDown, inBounds;
	int x, y, scrollWheel, dx, dy;

	MouseState(void)
		: leftDown(false), rightDown(false), middleDown(false), inBounds(true)
		, x(-1), y(0)
		, scrollWheel(0), dx(0), dy(0) { }

	void Update(int newX, int newY);
	void DeltaReset(void);

#ifdef _WIN32
	static void Hide(void) { while (ShowCursor(false) >= 0); }
	static void Show(void) { while (ShowCursor(true) >= 0); }
#endif
} mstate;