#pragma once

#define _VECT_CONV

#include "Vect2.h"
#include "Vertex.h"
#include "Utils.h"

typedef struct Triangle
{
	vrtx v0, v1, v2;

	Triangle(void) __GPU : v0(vrtx()), v1(vrtx()), v2(vrtx()) { }
	Triangle(const vrtx v0, const vrtx v1, const vrtx v2) __GPU : v0(v0), v1(v1), v2(v2) { }
	Triangle(const vrtx *v0, const vrtx *v1, const vrtx *v2) __GPU : v0(*v0), v1(*v1), v2(*v2) { }

	bool IsInside(vrtx *v) const __GPU
	{
		vect2 *vs1 = &V3ToV2(v1.v - v0.v);
		vect2 *vs2 = &V3ToV2(v2.v - v0.v);
		vect2 *vs3 = &V3ToV2(v->v - v0.v);

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