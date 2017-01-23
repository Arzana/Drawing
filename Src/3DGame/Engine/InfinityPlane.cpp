#include "InfinityPlane.h"
#include "Mtrx4.h"

InfinityPlane::InfinityPlane(vect3 a, vect3 b, vect3 c)
	: p(a)
{
	n = vect3::Normalize(&vect3::Cross(&(b - a), &(c - a)));
}

bool InfinityPlane::IntersectSegment(seg s) const
{
	mtrx4 m = mtrx4::CreateRotationQ(&(n - VECT3_RIGHT));
	s.a = vect4::ToNDC(m * s.a) - p;
	s.b = vect4::ToNDC(m * s.b) - p;
	return (s.a.X < 0 && s.b.X > 0) || (s.a.X > 0 && s.b.X < 0);
}

bool InfinityPlane::IntersectVCylinder(cyldr c) const
{
	mtrx4 m = mtrx4::CreateRotationQ(&(n - VECT3_RIGHT));
	c.p = vect4::ToNDC(m * c.p) - p;
	return c.p.X + c.r > 0 || c.p.X - c.r < 0;
}
