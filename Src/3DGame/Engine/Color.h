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
	0        8        16       24
	|--------|--------|--------|--------
	A        R        G        B
	*/
	uint packed;

	Color(void) __GPU : packed(0) { }
	Color(const uint packed) __GPU : packed(packed) { }
	Color(const int r, const int g, const int b) __GPU;
	Color(const int r, const int g, const int b, const int a) __GPU;
	Color(const float r, const float g, const float b) __GPU : Color(ipart(r * 255), ipart(g * 255), ipart(b * 255)) { }
	Color(const float r, const float g, const float b, const float a) __GPU : Color(ipart(r * 255), ipart(g * 255), ipart(b * 255), ipart(a * 255)) { }
	Color(const vect3 *v) __GPU : Color(v->X, v->Y, v->Z) { }
	Color(const vect4 *v) __GPU : Color(v->X, v->Y, v->Z, v->W) { }

	inline Color operator +(const Color& r) const __GPU { return Color(GetR() + r.GetR(), GetG() + r.GetG(), GetB() + r.GetB(), GetA() + r.GetA()); }
	inline Color operator -(const Color& r) const __GPU { return Color(GetR() - r.GetR(), GetG() - r.GetG(), GetB() - r.GetB(), GetA() - r.GetA()); }
	inline Color operator *(float r) const __GPU { return Color(GetR() * r, GetG() * r, GetB() * r, GetA() * r); }
	inline Color operator /(float r) const __GPU { return Color(GetR() / r, GetG() / r, GetB() / r, GetA() / r); }
	inline bool operator ==(const Color &r) const __GPU { return r.packed == packed; }
	inline bool operator !=(const Color &r) const __GPU { return r.packed != packed; }

	static Color Add(const Color *c0, const Color *c1) __GPU;
	static Color Divide(const Color *c, float divider) __GPU;
	static bool Equals(const Color *c1, const Color *c2) __GPU;
	static Color FromNonPremultiplied(int r, int g, int b, int a) __GPU;
	static void FromNonPremultiplied(Color *c, float a) __GPU;
	inline int GetA(void) const __CPU_ONLY { return octet(packed >> 24); }
	inline int GetA(void) const __GPU_ONLY { return packed >> 24; }
	inline int GetR(void) const __CPU_ONLY { return octet(packed >> 16); }
	inline int GetR(void) const __GPU_ONLY { return packed >> 16; }
	inline int GetG(void) const __CPU_ONLY { return octet(packed >> 8); }
	inline int GetG(void) const __GPU_ONLY { return packed >> 8; }
	inline int GetB(void) const __CPU_ONLY { return octet(packed); }
	inline int GetB(void) const __GPU_ONLY { return packed; }
	static Color Lerp(const Color min, const Color max, float a) __GPU;
	static Color Multiply(const Color *c, float multiplier) __GPU;
	static Color Subtract(const Color *c0, const Color *c1) __GPU;
	inline vect3 ToVect3(void) const __GPU { return vect3(GetR() / 255.0f, GetG() / 255.0f, GetB() / 255.0f); }
	inline vect4 ToVect4(void) const __GPU { return vect4(GetR() / 255.0f, GetG() / 255.0f, GetB() / 255.0f, GetA() / 255.0f); }
} clr;