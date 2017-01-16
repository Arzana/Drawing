#include <amp_math.h>
#include "Vect3.h"
#include "MathEx.h"

using namespace concurrency;

Vector3 Vector3::Abs(const Vector3 * v) __GPU
{
	return Vector3(abs(v->X), abs(v->Y), abs(v->Z));
}

void Vector3::Abs(void) __GPU
{
	X = abs(X);
	Y = abs(Y);
	Z = abs(Z);
}

Vector3 Vector3::Add(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(v1->X + v2->X, v1->Y + v2->Y, v1->Z + v2->Z);
}

float Vector3::Area(void) const __GPU
{
	return X * Z;
}

Vector3 Vector3::Barycentric(const Vector3 * v1, const Vector3 * v2, const Vector3 * v3, float b2, float b3) __GPU
{
	float x = barycentric(v1->X, v2->X, v3->X, b2, b3);
	float y = barycentric(v1->Y, v2->Y, v3->Y, b2, b3);
	float z = barycentric(v1->Z, v2->Z, v3->Z, b2, b3);

	return Vector3(x, y, z);
}

Vector3 Vector3::CatmullRom(const Vector3 * v1, const Vector3 * v2, const Vector3 * v3, const Vector3 * v4, float a) __GPU
{
	float x = catmullRom(v1->X, v2->X, v3->X, v4->X, a);
	float y = catmullRom(v1->Y, v2->Y, v3->Y, v4->Y, a);
	float z = catmullRom(v1->Z, v2->Z, v3->Z, v4->Z, a);

	return Vector3(x, y, z);
}

Vector3 Vector3::Clamp(const Vector3 * mi, const Vector3 * ma, const Vector3 * v) __GPU
{
	return Vector3(
		clamp(mi->X, ma->X, v->X),
		clamp(mi->Y, ma->Y, v->Y),
		clamp(mi->Z, ma->Z, v->Z));
}

void Vector3::Clamp(const Vector3 * mi, const Vector3 * ma) __GPU
{
	X = clamp(mi->X, ma->X, X);
	Y = clamp(mi->Y, ma->Y, Y);
	Z = clamp(mi->Z, ma->Z, Z);
}

Vector3 Vector3::Cross(const Vector3 * v1, const Vector3 * v2) __GPU
{
	float x = (v1->Y * v2->Z) - (v1->Z * v2->Y);
	float y = (v1->Z * v2->X) - (v1->X * v2->Z);
	float z = (v1->X * v2->Y) - (v1->Y * v2->X);

	return Vector3(x, y, z);
}

float Vector3::Distance(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return (*v2 - *v1).Length();
}

float Vector3::DistanceSquared(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return (*v2 - *v1).LengthSquared();
}

Vector3 Vector3::Divide(const Vector3 * v1, float v2) __GPU
{
	return Vector3(v1->X / v2, v1->Y / v2, v1->Z / v2);
}

Vector3 Vector3::Divide(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(v1->X / v2->X, v1->Y / v2->Y, v1->Z / v2->Z);
}

float Vector3::Dot(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return v1->X * v2->X + v1->Y * v2->Y + v1->Z * v2->Z;
}

bool Vector3::Equals(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return v1->X == v2->X && v1->Y == v2->Y && v1->Z == v2->Z;
}

Vector3 Vector3::Hermite(const Vector3 * v1, const Vector3 * t1, const Vector3 * v2, const Vector3 * t2, float a) __GPU
{
	float num = sqr(a);
	float num2 = cube(a);
	float num3 = num2 - num;
	float num4 = (num2 - (2.0f * num) + a);
	float num5 = (-2.0f * num2) + (3.0f * num);
	float num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;

	float x = (((v1->X * num6) + (v2->X * num5)) + (t1->X * num4)) + (t2->X * num3);
	float y = (((v1->Y * num6) + (v2->Y * num5)) + (t1->Y * num4)) + (t2->Y * num3);
	float z = (((v1->Z * num6) + (v2->Z * num5)) + (t1->Z * num4)) + (t2->Z * num3);

	return Vector3(x, y, z);
}

float Vector3::Length(void) const __CPU_ONLY
{
	return sqrtf(LengthSquared());
}

float Vector3::Length(void) const __GPU_ONLY
{
	return fast_math::sqrtf(LengthSquared());
}

float Vector3::LengthSquared(void) const __GPU
{
	return Dot(this, this);
}

Vector3 Vector3::Lerp(const Vector3 * mi, const Vector3 * ma, float a) __GPU
{
	float x = lerp(mi->X, ma->X, a);
	float y = lerp(mi->Y, ma->Y, a);
	float z = lerp(mi->Z, ma->Z, a);

	return Vector3(x, y, z);
}

Vector3 Vector3::Lerp(const Vector3 * mi, const Vector3 * ma, const Vector3 * a) __GPU
{
	float x = lerp(mi->X, ma->X, a->X);
	float y = lerp(mi->Y, ma->Y, a->Y);
	float z = lerp(mi->Z, ma->Z, a->Z);

	return Vector3(x, y, z);
}

Vector3 Vector3::InvLerp(const Vector3 * mi, const Vector3 * ma, float v) __GPU
{
	float x = invLerp(mi->X, ma->X, v);
	float y = invLerp(mi->Y, ma->Y, v);
	float z = invLerp(mi->Z, ma->Z, v);
	
	return Vector3(x, y, z);
}

Vector3 Vector3::InvLerp(const Vector3 * mi, const Vector3 * ma, const Vector3 * v) __GPU
{
	float x = invLerp(mi->X, ma->X, v->X);
	float y = invLerp(mi->Y, ma->Y, v->Y);
	float z = invLerp(mi->Z, ma->Z, v->Z);

	return Vector3(x, y, z);
}

Vector3 Vector3::Max(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(max(v1->X, v2->X), max(v1->Y, v2->Y), max(v1->Z, v2->Z));
}

Vector3 Vector3::Min(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(min(v1->X, v2->X), min(v1->Y, v2->Y), min(v1->Z, v2->Z));
}

Vector3 Vector3::Multiply(const Vector3 * v1, float v2) __GPU
{
	return Vector3(v1->X * v2, v1->Y * v2, v1->Z * v2);
}

Vector3 Vector3::Multiply(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(v1->X * v2->X, v1->Y * v2->Y, v1->Z * v2->Z);
}

Vector3 Vector3::Negate(const Vector3 * v) __GPU
{
	return Vector3(-v->X, -v->Y, -v->Z);
}

void Vector3::Normalize(void) __GPU
{
	float l = Length();
	operator/=(l);
}

Vector3 Vector3::Normalize(const Vector3 * v) __GPU
{
	float l = v->Length();
	return Divide(v, l);
}

Vector3 Vector3::Reflect(const Vector3 * v, const Vector3 * n) __GPU
{
	return *v - vect3(2) * (*v * *n) * *n;
}

Vector3 Vector3::SmoothStep(const Vector3 * mi, const Vector3 * ma, float a) __GPU
{
	a = sqr(a) * (3.0f - (2.0f * a));
	return Lerp(mi, ma, a);
}

Vector3 Vector3::Subtract(const Vector3 * v1, const Vector3 * v2) __GPU
{
	return Vector3(v1->X - v2->X, v1->Y - v2->Y, v1->Z - v2->Z);
}

float Vector3::Volume(void) const __GPU
{
	return X * Y * Z;
}
