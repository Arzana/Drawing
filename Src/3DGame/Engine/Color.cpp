#include "Color.h"

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

void Color::FromNonPremultiplied(Color * c, float a) __GPU
{
	*c = FromNonPremultiplied(c->GetR(), c->GetG(), c->GetB(), ipart(a * 255));
}

Color Color::Multiply(const Color * c, float multiplier) __GPU
{
	return Color(c->GetR() * multiplier, c->GetG() * multiplier, c->GetB() * multiplier, c->GetA() * multiplier);
}

Color Color::Subtract(const Color * c0, const Color * c1) __GPU
{
	return Color(c0->GetR() - c1->GetR(), c0->GetG() - c1->GetG(), c0->GetB() - c1->GetB(), c0->GetA() - c1->GetA());
}