#pragma once

#include "MathEx.h"
#include "Vect3.h"
#include "Vect4.h"

#define CLR_BLACK		((clr)0xFF000000)
#define CLR_RED			((clr)0xFFFF0000)
#define CLR_GREEN		((clr)0xFF00FF00)
#define CLR_BLUE		((clr)0xFF0000FF)
#define CLR_WHITE		((clr)0xFFFFFFFF)
#define CLR_CYAN		((clr)0xFF00FFFF)
#define CLR_MAGENTA		((clr)0xFFFF00FF)
#define CLR_YELLOW		((clr)0xFFFFFF00)

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
	Color(const vect3 *v);
	Color(const vect4 *v);

	Color operator +(const Color &r) const;
	Color operator -(const Color &r) const;
	Color operator *(float r) const;
	Color operator /(float r) const;
	bool operator ==(const Color &r) const;
	bool operator !=(const Color &r) const;

	static Color Add(const Color *c1, const Color *c2);
	static Color Divide(const Color *c, float divider);
	static bool Equals(const Color *c1, const Color *c2);
	static Color FromNonPremultiplied(int r, int g, int b, int a);
	static void FromNonPremultiplied(Color *c, float a);
	inline octet GetA(void) const;
	inline octet GetR(void) const;
	inline octet GetG(void) const;
	inline octet GetB(void) const;
	static Color Lerp(const Color min, const Color max, float a);
	static Color Multiply(const Color *c, float multiplier);
	static Color Subtract(const Color *c1, const Color *c2);
	vect3 ToVect3(void) const;
	vect4 ToVect4(void) const;
} clr;