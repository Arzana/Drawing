#pragma once

#include <InfinityPlane.h>
#include <Shapes.h>
#include "Player.h"

typedef struct Wall
{
	const plane model;
	const InfinityPlane collider;

	Wall(vrtx v0, vrtx v1, vrtx v2, vrtx v3);

	bool CheckCollision(const plr *plr) const;
} wall;