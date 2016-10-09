#pragma once

#include "MathEx.h"
#include "Vect3.h"
#include "Vect4.h"

typedef struct Color
{
public:
	/*
	|--------|--------|--------|--------
	A        R        G        B
	*/
	uint packed;

	Color(const uint packed);
	Color(const int r, const int g, const int b);
	Color(const int r, const int g, const int b, const int a);
	Color(const float r, const float g, const float b);
	Color(const float r, const float g, const float b, const float a);
	Color(const Vect3 *v);
	Color(const Vect4 *v);

	Color operator *(float r) const;
	bool operator ==(const Color &r) const;
	bool operator !=(const Color &r) const;

	static bool Equals(const Color *c1, const Color *c2);
	inline octet GetA(void) const;
	inline octet GetR(void) const;
	inline octet GetG(void) const;
	inline octet GetB(void) const;
	static Color Lerp(const Color *min, const Color *max, float a);
	static Color Multiply(const Color *c, float multiplier);
	Vect3 ToVect3(void) const;
	Vect4 ToVect4(void) const;
} Color;