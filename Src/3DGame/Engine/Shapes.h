#pragma once
#include "Color.h"
#include "Vect4.h"

#define MAX_VRTX_PRE_POLY	10
#define W_CLIPPING_PLANE	0.00001f

typedef struct polygon
{
	octet vrtxCount;
	struct clrvect4
	{
		vect4 v;
		clr c;

		static clrvect4 Lerp(const clrvect4 *min, const clrvect4 *max, float a)
		{
			return{ vect4::Lerp(&min->v, &max->v, a), clr::Lerp(min->c, max->c, a) };
		}
	} vertexes[MAX_VRTX_PRE_POLY];

	inline clrvect4* First(void) { return &vertexes[0]; }
	inline clrvect4* End(void) { return &vertexes[vrtxCount]; }
	inline clrvect4* Last(void) { return &vertexes[vrtxCount - 1]; }
	inline size_t TrglLen(void) { return vrtxCount < 3 ? 0 : vrtxCount - 2; }
} poly;

void ClipPoly(poly *face);