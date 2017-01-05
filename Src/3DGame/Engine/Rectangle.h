#pragma once

#ifndef __GPU
#define __GPU				restrict(cpu, amp)
#endif
#ifndef __GPU_ONLY
#define __GPU_ONLY			restrict(amp)
#endif
#ifndef __CPU_ONLY
#define __CPU_ONLY
#endif

#ifdef far
#undef far
#endif

#ifdef near
#undef near
#endif

typedef struct Rectangle
{
	int x, y, w, h;

	Rectangle(void) __GPU : x(0), y(0), w(0), h(0) { }
	Rectangle(int w, int h) __GPU : x(0), y(0), w(w), h(h) { }
	Rectangle(int x, int y, int w, int h) __GPU : x(x), y(y), w(w), h(h) { }

	inline bool operator ==(const Rectangle& r) __GPU { return r.x == x && r.y == y && r.w == w && r.h == h; }
	inline bool operator !=(const Rectangle& r) __GPU { return r.x != x || r.y != y || r.w != w || r.h != h; }

	inline int Left(void) const __GPU { return w > 0 ? x : x + w; }
	inline int Right(void) const __GPU { return w > 0 ? x + w : x; }
	inline int Top(void) const __GPU { return h > 0 ? y : y + h; }
	inline int Bottom(void) const __GPU { return h > 0 ? y + h : y; }
	bool Contains(int x, int y) const __GPU { return x >= Left() && x <= Right() && y >= Top() && y <= Bottom(); }
} rect;

typedef struct ViewPort
{
	rect screen;
	float far, near;

	ViewPort(void) __GPU : screen(rect()), far(0), near(0) { }
	ViewPort(int w, int h) __GPU : screen(rect(w, h)), far(0), near(0) { }
	ViewPort(rect screen) __GPU : screen(screen), far(0), near(0) { }
	ViewPort(int w, int h, float far, float near) __GPU : screen(rect(w, h)), far(far), near(near) { }
	ViewPort(rect screen, float far, float near) __GPU : screen(screen), far(far), near(near) { }
} vPort;