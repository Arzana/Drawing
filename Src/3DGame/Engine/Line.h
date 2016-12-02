#pragma once

#include "Vertex.h"

typedef struct Line
{
	vrtx v0, v1;

	Line(void) __GPU : v0(vrtx()), v1(vrtx()) { }
	Line(const vrtx v0, const vrtx v1) __GPU : v0(v0), v1(v1) { }
	Line(const vrtx *v0, const vrtx *v1) __GPU : v0(*v0), v1(*v1) { }
} line;