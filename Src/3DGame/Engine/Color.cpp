#include "Color.h"

Color::Color(void)
	: packed(0)
{ }

Color::Color(const uint packed)
	: packed(packed)
{ }

Color::Color(const int r, const int g, const int b)
{
	packed = 0xFF000000;	// Set A to 255

	if (((r | g | b) & 0xFFFFFF00) != 0)
	{
		int clpR = clamp(0, 255, r);
		int clpG = clamp(0, 255, g);
		int clpB = clamp(0, 255, b);

		packed |= (clpR << 16) | (clpG << 8) | (clpB);
	}
	else packed |= (r << 16) | (g << 8) | (b);
}

Color::Color(const int r, const int g, const int b, const int a)
{
	if (((r | g | b | a) & 0xFFFFFF00) != 0)
	{
		int clpR = clamp(0, 255, r);
		int clpG = clamp(0, 255, g);
		int clpB = clamp(0, 255, b);
		int clpA = clamp(0, 255, a);

		packed = (clpA << 24) | (clpR << 16) | (clpG << 8) | (clpB);
	}
	else packed = (a << 24) | (r << 16) | (g << 8) | (b);
}

Color::Color(const float r, const float g, const float b)
	: Color(ipart(r * 255), ipart(g * 255), ipart(b * 255))
{ }

Color::Color(const float r, const float g, const float b, const float a)
	: Color(ipart(r * 255), ipart(g * 255), ipart(b * 255), ipart(a * 255))
{ }

Color::Color(const vect3 * v)
	: Color(v->X, v->Y, v->Z)
{ }

Color::Color(const vect4 * v)
	: Color(v->X, v->Y, v->Z, v->W)
{ }

Color Color::operator+(const Color & r) const
{
	return Add(this, &r);
}

Color Color::operator-(const Color & r) const
{
	return Subtract(this, &r);
}

Color Color::operator*(float r) const
{
	return Multiply(this, r);
}

Color Color::operator/(float r) const
{
	return Divide(this, r);
}

bool Color::operator==(const Color & r) const
{
	return Equals(this, &r);
}

bool Color::operator!=(const Color & r) const
{
	return !Equals(this, &r);
}

Color Color::Add(const Color * c1, const Color * c2)
{
	return Color(c1->GetR() + c2->GetR(), c1->GetG() + c2->GetG(), c1->GetB() + c2->GetB());
}

Color Color::Divide(const Color * c, float divider)
{
	return Color(c->GetR() / divider, c->GetG() / divider, c->GetB() / divider, c->GetA() / divider);
}

bool Color::Equals(const Color * c1, const Color * c2)
{
	return c1->packed == c2->packed;
}

Color Color::FromNonPremultiplied(int r, int g, int b, int a)
{
	return Color(r * a / 255, g * a / 255, b * a / 255, a);
}

void Color::FromNonPremultiplied(Color * c, float a)
{
	*c = FromNonPremultiplied(c->GetR(), c->GetG(), c->GetB(), ipart(a * 255));
}

inline octet Color::GetA(void) const
{
	return octet(packed >> 24);
}

inline octet Color::GetR(void) const
{
	return octet(packed >> 16);
}

inline octet Color::GetG(void) const
{
	return octet(packed >> 8);
}

inline octet Color::GetB(void) const
{
	return octet(packed);
}

Color Color::Lerp(const Color mi, const Color ma, float a)
{
	a = clamp(0, 1, a);
	float r = lerp(mi.GetR(), ma.GetR(), a);
	float g = lerp(mi.GetG(), ma.GetG(), a);
	float b = lerp(mi.GetB(), ma.GetB(), a);
	float alpha = lerp(mi.GetA(), ma.GetA(), a);

	return Color(r, g, b, alpha);
}

Color Color::Multiply(const Color * c, float multiplier)
{
	return Color(c->GetR() * multiplier, c->GetG() * multiplier, c->GetB() * multiplier, c->GetA() * multiplier);
}

Color Color::Subtract(const Color * c1, const Color * c2)
{
	return Color(c1->GetR() - c2->GetR(), c1->GetG() - c2->GetG(), c1->GetB() - c2->GetB());
}

vect3 Color::ToVect3(void) const
{
	return vect3(GetA() / 255.0f, GetG() / 255.0f, GetB() / 255.0f);
}

vect4 Color::ToVect4(void) const
{
	return vect4(GetR() / 255.0f, GetG() / 255.0f, GetB() / 255.0f, GetA() / 255.0f);
}
