#pragma once

#include "Color.h"
#include "Rectangle.h"

#define INSIDE		0b000000
#define LEFT		0b000001
#define RIGHT		0b000010
#define HORIZONTAL	0b000011
#define BOTTOM		0b000100
#define TOP			0b001000
#define VERTICAL	0b001100
#define NEAR		0b010000
#define FAR			0b100000
#define DEPTH		0b110000
#define OUTSIDE		0b111111

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

	int ComputeMask(const ViewPort vp) const __GPU
	{
		int code = INSIDE;

		if (v.X < vp.screen.x) code |= LEFT;
		else if (v.X > vp.screen.w) code |= RIGHT;
		if (v.Y < vp.screen.y) code |= BOTTOM;
		else if (v.Y > vp.screen.h) code |= TOP;
		if (v.Z < vp.near) code |= NEAR;
		else if (v.Z > vp.far) code |= FAR;

		return code;
	}
} vrtx;