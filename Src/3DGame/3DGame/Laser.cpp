#include "Laser.h"

bool Laser::CheckCollision(const wall * walls, size_t len) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (walls[i].collider.IntersectSegment(seg(V4ToV3(a.v), V4ToV3(b.v)))) return true;
	}

	return false;
}