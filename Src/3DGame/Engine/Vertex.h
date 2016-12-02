#pragma once

#include "Color.h"

typedef struct Vertex
{
	vect3 v;
	Color c;

	Vertex(void) __GPU : v(VECT3_ZERO), c(CLR_BLACK) { }
	Vertex(const vect3 v, const clr c) __GPU : v(v), c(c) { }
	Vertex(const float x, const float y, const float z) __GPU : v(vect3(x, y, z)), c(CLR_BLACK) { }
	Vertex(const float x, const float y, const float z, const clr c) __GPU : v(vect3(x, y, z)), c(c) { }

	inline bool operator ==(const Vertex& r) __GPU { return r.v == v && r.c == c; }
	inline bool operator !=(const Vertex& r) __GPU { return r.v != v || r.c != c; }
} vrtx;