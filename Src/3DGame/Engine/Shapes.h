#pragma once
#include "Vect4.h"

#define MAX_VRTX_PRE_POLY	10
#define W_CLIPPING_PLANE	0.00001

typedef struct polygon
{
	octet vrtxCount;
	vect4 vertices[MAX_VRTX_PRE_POLY];

	inline vect4* First(void) { return &vertices[0]; }
	inline vect4* Last(void) { return &vertices[vrtxCount]; }
	inline size_t TrglLen(void) { return vrtxCount > 2 ? vrtxCount - 2 : 0; }
} poly;

void ClipPoly(poly *face);