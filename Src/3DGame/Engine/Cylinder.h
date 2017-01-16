#pragma once

#include "Segment.h"

typedef struct Cylinder
{
	vect3 p, q;
	float r;

	Cylinder(void) : p(VECT3_ZERO), q(VECT3_ZERO), r(0) { }
	Cylinder(vect3 p, vect3 q, float r) : p(p), q(q), r(r) { }

	bool IntersectSegment(seg s, float *t);
} cyldr;