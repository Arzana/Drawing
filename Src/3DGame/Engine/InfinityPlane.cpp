#define _VECT_CONV

#include "InfinityPlane.h"
#include "Utils.h"
#include "Mtrx4.h"

InfinityPlane::InfinityPlane(vect3 a, vect3 b, vect3 c)
	: p(a)
{
	n = vect3::Normalize(&vect3::Cross(&(b - a), &(c - a)));
}

bool InfinityPlane::IntersectSegment(seg s)
{
	mtrx4 m = mtrx4::CreateRotationQ(&(n - VECT3_RIGHT));
	s.a = V4ToV3(m * s.a) - p;
	s.b = V4ToV3(m * s.b) - p;
	return (s.a.X < 0 && s.b.X > 0) || (s.a.X > 0 && s.b.X < 0);
}
