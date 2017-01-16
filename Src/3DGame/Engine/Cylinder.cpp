#include <cmath>
#include <cfloat>
#include "MathEx.h"
#include "Cylinder.h"

bool Cylinder::IntersectSegment(seg s, float * t)
{
	vect3 d = q - p;
	vect3 m = s.a - p;
	vect3 n = s.b - s.a;
	float md = vect3::Dot(&m, &d);
	float nd = vect3::Dot(&n, &d);
	float dd = d.LengthSquared();

	// Test if segment fully outside either of cylinder
	if (md < 0.0f && md + nd < 0.0f) return false;	// Segment outside 'p' side of cylinder
	if (md > dd && md + nd > dd) return false;		// Segment outside 'q' side of cylinder

	float nn = n.LengthSquared();
	float mn = vect3::Dot(&m, &n);
	float a = dd * nn - sqr(nd);
	float k = m.LengthSquared() - sqr(r);
	float c = dd * k - sqr(md);

	if (abs(a) < FLT_EPSILON)						// Segment runs parallel to cylinder axis
	{
		if (c > 0.0f) return false;					// 'a' and thus the segment lie outside cylinder
		// Now known that the segment intersects cylinder; figure out how it intersectes
		if (md < 0.0f) *t = -mn / nn;				// Intersect segment against 'p' endcap
		else if (md > dd) *t = (nd - mn) / nn;		// Intersect segment against 'q' endcap
		else *t = 0.0f;								// 'a' lies inside cylinder
		return true;
	}

	float b = dd * mn - nd * md;
	float discr = sqr(b) - a * c;
	if (discr < 0.0f) return false;					// No real roots; no intersection

	*t = (-b - sqrtf(discr)) / a;
	if (*t < 0.0f || *t > 1.0f) return false;		// Intersection lies outside segment
	if (md + *t * nd < 0.0f)						// Intersection outsode cylinder on 'p' side
	{
		if (nd <= 0.0f) return false;				// Segment pointing away from endcap
		*t = -md / nd;
		// Keep intersection if Dot(S(t) - p, S(t) - p) <= r^2
		return k + 2 * *t * (mn + *t * nn) <= 0.0f;
	}
	else if (md + *t * nd > dd)						// Segment lies outside cylinder on 'q' side
	{
		if (nd >= 0.0f) return false;				// Segment pointing away from endcap
		*t = (dd - md) / nd;
		// Keep intersection if Dot(S(t) - q, S(t) - q) <= r^2
		return k + dd - 2 * md + *t * (2 * (mn - nd) + *t * nn) <= 0.0f;
	}

	// Segment intersects cylinder between the endcaps; t is correct
	return true;
}
