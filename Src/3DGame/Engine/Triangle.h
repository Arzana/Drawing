#pragma once

#include "Line.h"

typedef struct Triangle
{
	vrtx v0, v1, v2;

	Triangle(void) __GPU : v0(vrtx()), v1(vrtx()), v2(vrtx()) { }
	Triangle(const vrtx v0, const vrtx v1, const vrtx v2) __GPU : v0(v0), v1(v1), v2(v2) { }
	Triangle(const vrtx *v0, const vrtx *v1, const vrtx *v2) __GPU : v0(*v0), v1(*v1), v2(*v2) { }

	line GetLine(const int l) const __GPU;
	bool IsInside(vrtx *v) const __GPU;
} trgl;