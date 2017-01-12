#pragma once

#include "Geon.h"

typedef struct Plane
	: public Geon
{
	Plane(vrtx v0, vrtx v1, vrtx v2, vrtx v3) : Geon(2, trgl(v0, v1, v2), trgl(v0, v2, v3)) { }
} plane;