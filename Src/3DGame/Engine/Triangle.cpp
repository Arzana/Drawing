#define _VECT_CONV

#include "Triangle.h"
#include "Vect2.h"
#include "Utils.h"

line Triangle::GetLine(const int l) const __GPU
{
	return !l ? line(v0, v1) : (l == 1 ? line(v1, v2) : line(v2, v0));
}

bool Triangle::IsInside(vrtx * v) const __GPU
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