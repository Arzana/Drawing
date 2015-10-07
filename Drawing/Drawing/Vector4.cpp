#include <math.h>
#include "Vector4.h"
#include "UtilDefs.h"

Vector4::Vector4()
{
	X = 0;
	Y = 0;
	Z = 0;
	W = 0;
}

Vector4::Vector4(float v)
{
	X = v;
	Y = v;
	Z = v;
	W = v;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Vector4 Vector4::Clamp(const Vector4& mi, const Vector4& ma, const Vector4& v)
{
	float x = clamp(mi.X, ma.X, v.X);
	float y = clamp(mi.Y, ma.Y, v.Y);
	float z = clamp(mi.Z, ma.Z, v.Z);
	float w = clamp(mi.W, ma.W, v.W);

	return Vector4(x, y, z, w);
}

void Vector4::Clamp(const Vector4& mi, const Vector4& ma, Vector4 *v)
{
	v->X = clamp(mi.X, ma.X, v->X);
	v->Y = clamp(mi.Y, ma.Y, v->Y);
	v->Z = clamp(mi.Z, ma.Z, v->Z);
	v->W = clamp(mi.W, ma.W, v->W);
}

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
}

float Vector4::GetLength() const
{
	return sqrtf(square(X) + square(Y) + square(Z) + square(W));
}

float Vector4::GetLenghtSquared() const
{
	return square(X) + square(Y) + square(Z) + square(W);
}

Vector4 Vector4::Lerp(const Vector4& l, const Vector4& r, float a)
{
	Vector4 adder = r - l;
	return (l + adder) * a;
}

Vector4 Vector4::Max(const Vector4& l, const Vector4& r)
{
	float x = fmax(l.X, r.X);
	float y = fmax(l.Y, r.Y);
	float z = fmax(l.Z, r.Z);
	float w = fmax(l.W, r.W);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::Min(const Vector4& l, const Vector4& r)
{
	float x = fmin(l.X, r.X);
	float y = fmin(l.Y, r.Y);
	float z = fmin(l.Z, r.Z);
	float w = fmin(l.W, r.W);

	return Vector4(x, y, z, w);
}

void Vector4::Normalize()
{
	float length = GetLength();

	X = X / length;
	Y = Y / length;
	Z = Z / length;
	W = W / length;
}

Vector4 Vector4::Normalize(const Vector4& v)
{
	float length = v.GetLength();

	float x = v.X / length;
	float y = v.Y / length;
	float z = v.Z / length;
	float w = v.W / length;

	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator*(float s) const
{
	float x = X * s;
	float y = Y * s;
	float z = Z * s;
	float w = W * s;

	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator*(const Vector4& s) const
{
	float x = X * s.X;
	float y = Y * s.Y;
	float z = Z * s.Z;
	float w = W * s.W;

	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator+(const Vector4& r) const
{
	float x = X + r.X;
	float y = Y + r.Y;
	float z = Z + r.Z;
	float w = W + r.W;

	return Vector4(x, y, z, w);
}

void Vector4::operator+=(const Vector4& r)
{
	X += r.X;
	Y += r.Y;
	Z += r.Z;
	W += r.W;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-X, -Y, -Z, -W);
}

Vector4 Vector4::operator-(const Vector4& r) const
{
	float x = X - r.X;
	float y = Y - r.Y;
	float z = Z - r.Z;
	float w = W - r.W;

	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator/(float s) const
{
	float x = X / s;
	float y = Y / s;
	float z = Z / s;
	float w = W / s;

	return Vector4(x, y, z, w);
}


Vector4 Vector4::operator/(const Vector4& r) const
{
	float x = X / r.X;
	float y = Y / r.Y;
	float z = Z / r.Z;
	float w = W / r.W;

	return Vector4(x, y, z, w);
}

bool Vector4::operator==(const Vector4& o) const
{
	return	X == o.X && Y == o.Y && Z == o.Z && W == o.W;
}

bool Vector4::operator!=(const Vector4& o) const
{
	return X != o.X && Y != o.Y && Z != o.Z && W != o.W;
}

Vector3 Vector4::ToCartesian() const
{
	float x = X / W;
	float y = Y / W;
	float z = Z / W;

	return Vector3(x, y, z);
}

void Vector4::ToCartesian(Vector4 *s, Vector3 *d, int length)
{
	for (int i = 0; i < length; i++)
	{
		Vector4 *cur = s + i;
		float x = cur->X / cur->W;
		float y = cur->Y / cur->W;
		float z = cur->Z / cur->W;

		d[i] = Vector3(x, y, z);
	}
}