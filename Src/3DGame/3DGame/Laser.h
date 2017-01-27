#pragma once

#include <Vertex.h>
#include "Wall.h"

typedef struct Laser
{
	vrtx a, b;
	vect4 vel;

	Laser(void) : a(), b(), vel() { }
	Laser(vrtx a, vrtx b, vect3 vel) : a(a), b(b), vel(vel) { }

	bool CheckCollision(const wall *walls, size_t len) const;
} lsr;