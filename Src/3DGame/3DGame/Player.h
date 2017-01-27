#pragma once

#include <Mtrx4.h>
#include <Cylinder.h>

typedef struct Player
{
	vect3 pos, rot;

	Player(void) : pos(VECT3_ZERO), rot(VECT3_ZERO) { }
	Player(vect3 pos) : pos(pos), rot(VECT3_ZERO) { }

	cyldr GetCollider(void) const { return cyldr(pos, vect3(pos.X, 0, pos.Y), 0.25f); }
	void Move(vect3 dir) { pos += vect4::ToNDC(mtrx4::CreateRotationQ(rot.X, 0, rot.Z) * dir); }
	void Look(float dx, float dy) { rot += vect3(dx, dy, 0); }
} plr;