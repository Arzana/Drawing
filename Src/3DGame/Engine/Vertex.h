#pragma once

#include "Color.h"
#include "Rectangle.h"

#ifdef FAR
#undef FAR
#endif

#ifdef NEAR
#undef NEAR
#endif

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
	vect4 v;
	Color c;

	Vertex(void) __GPU : v(VECT3_ZERO), c(CLR_BLACK) { }
	Vertex(vect4 v, clr c) __GPU : v(v), c(c) { }
	Vertex(float x, float y, float z) __GPU : v(x, y, z, 1), c(CLR_BLACK) { }
	Vertex(float x, float y, float z, clr c) __GPU : v(x, y, z, 1), c(c) { }

	inline bool operator ==(const Vertex& r) __GPU { return r.v == v && r.c == c; }
	inline bool operator !=(const Vertex& r) __GPU { return r.v != v || r.c != c; }

	static Vertex Lerp(const Vertex *min, const Vertex *max, float a)
	{
		return Vertex(vect4::Lerp(&min->v, &max->v, a), clr::Lerp(min->c, max->c, a));
	}

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