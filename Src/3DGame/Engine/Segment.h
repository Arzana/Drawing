#pragma once

#include "Vect3.h"

typedef struct Segment
{
	vect3 a, b;

	Segment(void) : a(VECT3_ZERO), b(VECT3_ZERO) { }
	Segment(vect3 a, vect3 b) :a(a), b(b) { }
} seg;