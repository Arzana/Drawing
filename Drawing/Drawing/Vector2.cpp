#include <math.h>
#include "UtilDefs.h"
#include "Vector2.h"

Vector2::Vector2()
{
	X = 0;
	Y = 0;
}

Vector2::Vector2(float v)
{
	X = v;
	Y = v;
}

Vector2::Vector2(float x, float y)
{
	X = y;
	Y = y;
}

Vector2 Vector2::Clamp(const Vector2& mi, const Vector2& ma, const Vector2& v)
{
	float x = clamp(mi.X, ma.X, v.X);
	float y = clamp(mi.Y, ma.Y, v.Y);

	return Vector2(x, y);
}

void Vector2::Clamp(const Vector2& mi, const Vector2& ma, Vector2 *v)
{
	v->X = clamp(mi.X, ma.X, v->X);
	v->Y = clamp(mi.Y, ma.Y, v->Y);
}

float Vector2::GetLength() const
{
	return sqrtf(square(X) + square(Y));
}

float Vector2::GetLenghtSquared() const
{
	return square(X) + square(Y);
}

Vector2 Vector2::Lerp(const Vector2& l, const Vector2& r, float a)
{
	Vector2 adder = r - l;
	return (l + adder) * a;
}

Vector2 Vector2::Max(const Vector2& l, const Vector2& r)
{
	float x = fmax(l.X, r.X);
	float y = fmax(l.Y, r.Y);

	return Vector2(x, y);
}

Vector2 Vector2::Min(const Vector2& l, const Vector2& r)
{
	float x = fmin(l.X, r.X);
	float y = fmin(l.Y, r.Y);

	return Vector2(x, y);
}

void Vector2::Normalize()
{
	float length = GetLength();

	X = X / length;
	Y = Y / length;
}

Vector2 Vector2::Normalize(const Vector2& v)
{
	float length = v.GetLength();

	float x = v.X / length;
	float y = v.Y / length;

	return Vector2(x, y);
}

Vector2 Vector2::operator*(float s) const
{
	float x = X * s;
	float y = Y * s;

	return Vector2(x, y);
}

Vector2 Vector2::operator*(const Vector2& s) const
{
	float x = X * s.X;
	float y = Y * s.Y;

	return Vector2(x, y);
}

Vector2 Vector2::operator+(const Vector2& r) const
{
	float x = X + r.X;
	float y = Y + r.Y;

	return Vector2(x, y);
}

void Vector2::operator+=(const Vector2& r)
{
	X += r.X;
	Y += r.Y;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-X, -Y);
}

Vector2 Vector2::operator-(const Vector2& r) const
{
	float x = X - r.X;
	float y = Y - r.Y;

	return Vector2(x, y);
}

Vector2 Vector2::operator/(const Vector2& r) const
{
	float x = X / r.X;
	float y = Y / r.Y;

	return Vector2(x, y);
}

bool Vector2::operator==(const Vector2& o) const
{
	return	X == o.X && Y == o.Y;
}

bool Vector2::operator!=(const Vector2& o) const
{
	return X != o.X && Y != o.Y;
}