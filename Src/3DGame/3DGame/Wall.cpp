#define _VECT_CONV

#include <Utils.h>
#include "Wall.h"

Wall::Wall(vrtx v0, vrtx v1, vrtx v2, vrtx v3)
	: model(v0, v1, v2, v3)
	, collider(V4ToV3(v0.v), V4ToV3(v1.v), V4ToV3(v3.v))
{ }

bool Wall::CheckCollision(const plr * plr) const
{
	return collider.IntersectVCylinder(plr->GetCollider());
}
