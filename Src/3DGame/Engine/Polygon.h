#pragma once
#include "Vertex.h"

#define MAX_VRTX_POLY		7
#define W_CLIPPING_PLANE	0.00001f

typedef struct polygon
{
	octet vrtxCount;
	Vertex vertexes[MAX_VRTX_POLY];

	inline Vertex* First(void) { return &vertexes[0]; }
	inline Vertex* End(void) { return &vertexes[vrtxCount]; }
	inline Vertex* Last(void) { return &vertexes[vrtxCount - 1]; }
} poly;

void ClipPoly(poly *face);