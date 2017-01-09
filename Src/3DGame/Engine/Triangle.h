#pragma once

#define _VECT_CONV

#include "Vect2.h"
#include "Vertex.h"
#include "Utils.h"

typedef struct Triangle
{
	vrtx v0, v1, v2;

	Triangle(void) __GPU : v0(vrtx()), v1(vrtx()), v2(vrtx()) { }
	Triangle(vrtx v0, vrtx v1, vrtx v2) __GPU : v0(v0), v1(v1), v2(v2) { }

	bool IsInside(vrtx *v) const __GPU
	{
		vect2 *vs1 = &vect2(v1.v.X - v0.v.X, v1.v.Y - v0.v.Y);
		vect2 *vs2 = &vect2(v2.v.X - v0.v.X, v2.v.Y - v0.v.Y);
		vect2 *vs3 = &vect2(v->v.X - v0.v.X, v->v.Y - v0.v.Y);

		float r = vect2::PrepDot(vs1, vs2);
		float s = vect2::PrepDot(vs3, vs2) / r;
		float t = vect2::PrepDot(vs1, vs3) / r;

		if (s >= 0 && t >= 0 && s + t <= 1)
		{
			float u = 1.0f - s - t;
			v->v.Z = v0.v.Z * u + v1.v.Z * s + v2.v.Z * t;
			v->c = v0.c * u + v1.c * s + v2.c * t;
			return true;
		}

		return false;
	}
} trgl;