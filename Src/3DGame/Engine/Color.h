#pragma once

#include "MathEx.h"
#include "Vect3.h"
#include "Vect4.h"

#define CLR_BLACK		((Color)0xFF000000)
#define CLR_RED			((Color)0xFFFF0000)
#define CLR_GREEN		((Color)0xFF00FF00)
#define CLR_BLUE		((Color)0xFF0000FF)
#define CLR_WHITE		((Color)0xFFFFFFFF)
#define CLR_CYAN		((Color)0xFF00FFFF)
#define CLR_MAGENTA		((Color)0xFFFF00FF)
#define CLR_YELLOW		((Color)0xFFFFFF00)

typedef struct Color
{
	/*
	|--------|--------|--------|--------
	A        R        G        B
	*/
	uint packed;

	Color(void);
	Color(const uint packed);
	Color(const int r, const int g, const int b);
	Color(const int r, const int g, const int b, const int a);
	Color(const float r, const float g, const float b);
	Color(const float r, const float g, const float b, const float a);
	Color(const Vect3 *v);
	Color(const Vect4 *v);

	Color operator +(const Color &r) const;
	Color operator -(const Color &r) const;
	Color operator *(float r) const;
	Color operator /(float r) const;
	bool operator ==(const Color &r) const;
	bool operator !=(const Color &r) const;

	static Color Add(const Color *c1, const Color *c2);
	static Color Divide(const Color *c, float divider);
	static bool Equals(const Color *c1, const Color *c2);
	inline octet GetA(void) const;
	inline octet GetR(void) const;
	inline octet GetG(void) const;
	inline octet GetB(void) const;
	static Color Lerp(const Color min, const Color max, float a);
	static Color Multiply(const Color *c, float multiplier);
	static Color Subtract(const Color *c1, const Color *c2);
	Vect3 ToVect3(void) const;
	Vect4 ToVect4(void) const;
} Color;