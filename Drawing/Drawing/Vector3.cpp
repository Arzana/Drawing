#include <math.h>
#include "UtilDefs.h"
#include "Vector3.h"

Vector3::Vector3()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector3::Vector3(float v)
{
	X = v;
	Y = v;
	Z = v;
}

Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3 Vector3::Clamp(const Vector3& mi, const Vector3& ma, const Vector3& v)
{
	float x = clamp(mi.X, ma.X, v.X);
	float y = clamp(mi.Y, ma.Y, v.Y);
	float z = clamp(mi.Z, ma.Z, v.Z);

	return Vector3(x, y, z);
}

void Vector3::Clamp(const Vector3& mi, const Vector3& ma, Vector3* v)
{
	v->X = clamp(mi.X, ma.X, v->X);
	v->Y = clamp(mi.Y, ma.Y, v->Y);
	v->Z = clamp(mi.Z, ma.Z, v->Z);
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	float x = (a.Y * b.Z) - (a.Z * b.Y);
	float y = (a.Z * b.X) - (a.X * b.Z);
	float z = (a.X * b.Y) - (a.Y * b.X);

	return Vector3(x, y, z);
}

float Vector3::Distance(const Vector3& l, const Vector3& r)
{
	float diffX = r.X - l.X;
	float diffY = r.Y - l.Y;
	float diffZ = r.Z - l.Z;

	float distX = square(diffX);
	float distY = square(diffY);
	float distZ = square(diffZ);
	
	return sqrtf(distX + distY + distZ);
}

float Vector3::DistanceSquared(const Vector3& l, const Vector3& r)
{
	float diffX = r.X - l.X;
	float diffY = r.Y - l.Y;
	float diffZ = r.Z - l.Z;

	return square(diffX) + square(diffY) + square(diffZ);
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

float Vector3::GetLength() const
{
	return sqrtf(square(X) + square(Y) + square(Z));
}

float Vector3::GetLenghtSquared() const
{
	return square(X) + square(Y) + square(Z);
}

Vector3 Vector3::Lerp(const Vector3& l, const Vector3& r, float a)
{
	Vector3 adder = r - l;
	return (l + adder) * a;
}

Vector3 Vector3::Max(const Vector3& l, const Vector3& r)
{
	float x = fmax(l.X, r.X);
	float y = fmax(l.Y, r.Y);
	float z = fmax(l.Z, r.Z);

	return Vector3(x, y, z);
}

Vector3 Vector3::Min(const Vector3& l, const Vector3& r)
{
	float x = fmin(l.X, r.X);
	float y = fmin(l.Y, r.Y);
	float z = fmin(l.Z, r.Z);

	return Vector3(x, y, z);
}

void Vector3::Normalize()
{
	float length = GetLength();

	X = X / length;
	Y = Y / length;
	Z = Z / length;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	float length = v.GetLength();

	float x = v.X / length;
	float y = v.Y / length;
	float z = v.Z / length;

	return Vector3(x, y, z);
}

Vector3 Vector3::operator*(float s) const
{
	float x = X * s;
	float y = Y * s;
	float z = Z * s;

	return Vector3(x, y, z);
}

Vector3 Vector3::operator*(const Vector3& s) const
{
	float x = X * s.X;
	float y = Y * s.Y;
	float z = Z * s.Z;

	return Vector3(x, y, z);
}

Vector3 Vector3::operator+(const Vector3& r) const
{
	float x = X + r.X;
	float y = Y + r.Y;
	float z = Z + r.Z;

	return Vector3(x, y, z);
}

void Vector3::operator+=(const Vector3& r)
{
	X += r.X;
	Y += r.Y;
	Z += r.Z;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-X, -Y, -Z);
}

Vector3 Vector3::operator-(const Vector3& r) const
{
	float x = X - r.X;
	float y = Y - r.Y;
	float z = Z - r.Z;

	return Vector3(x, y, z);
}

Vector3 Vector3::operator/(float s) const
{
	float x = X / s;
	float y = Y / s;
	float z = Z / s;

	return Vector3(x, y, z);
}


Vector3 Vector3::operator/(const Vector3& r) const
{
	float x = X / r.X;
	float y = Y / r.Y;
	float z = Z / r.Z;

	return Vector3(x, y, z);
}

bool Vector3::operator==(const Vector3& o) const
{
	return	X == o.X && Y == o.Y && Z == o.Z;
}

bool Vector3::operator!=(const Vector3& o) const
{
	return X != o.X && Y != o.Y && Z != o.Z;
}