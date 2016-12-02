#include "Color.h"

Color::Color(const int r, const int g, const int b) __GPU
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

Color::Color(const int r, const int g, const int b, const int a) __GPU
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

Color Color::Add(const Color * c0, const Color * c1) __GPU
{
	return Color(c0->GetR() + c1->GetR(), c0->GetG() + c1->GetG(), c0->GetB() + c1->GetB(), c0->GetA() + c1->GetA());
}

Color Color::Divide(const Color * c, float divider) __GPU
{
	return Color(c->GetR() / divider, c->GetG() / divider, c->GetB() / divider, c->GetA() / divider);
}

bool Color::Equals(const Color * c1, const Color * c2) __GPU
{
	return c1->packed == c2->packed;
}

Color Color::FromNonPremultiplied(int r, int g, int b, int a) __GPU
{
	return Color(r * a / 255, g * a / 255, b * a / 255, a);
}

void Color::FromNonPremultiplied(Color * c, float a) __GPU
{
	*c = FromNonPremultiplied(c->GetR(), c->GetG(), c->GetB(), ipart(a * 255));
}

Color Color::Lerp(const Color mi, const Color ma, float a) __GPU
{
	a = clamp(0, 1, a);
	float r = lerp(mi.GetR(), ma.GetR(), a);
	float g = lerp(mi.GetG(), ma.GetG(), a);
	float b = lerp(mi.GetB(), ma.GetB(), a);
	float alpha = lerp(mi.GetA(), ma.GetA(), a);

	return Color(r, g, b, alpha);
}

Color Color::Multiply(const Color * c, float multiplier) __GPU
{
	return Color(c->GetR() * multiplier, c->GetG() * multiplier, c->GetB() * multiplier, c->GetA() * multiplier);
}

Color Color::Subtract(const Color * c0, const Color * c1) __GPU
{
	return Color(c0->GetR() - c1->GetR(), c0->GetG() - c1->GetG(), c0->GetB() - c1->GetB(), c0->GetA() - c1->GetA());
}