#pragma once

#include "Cylinder.h"

typedef struct InfinityPlane
{
	vect3 p, n;

	InfinityPlane(void) : p(VECT3_ZERO), n(VECT3_ZERO) { }
	InfinityPlane(vect3 a, vect3 b, vect3 c);

	bool IntersectSegment(seg s) const;
	bool IntersectVCylinder(cyldr c) const;
} infplane;