#include <amp_math.h>
#include "vect2.h"
#include "MathEx.h"

using namespace concurrency;

Vector2 Vector2::Abs(const Vector2 * v) __GPU
{
	return vect2(abs(v->X), abs(v->Y));
}

void Vector2::Abs() __GPU
{
	X = abs(X);
	Y = abs(Y);
}

float Vector2::Area(void) const __GPU
{
	return X * Y;
}

Vector2 Vector2::Add(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(v1->X + v2->X, v1->Y + v2->Y);
}

float Vector2::Angle(const Vector2 * v1, const Vector2 * v2) __CPU_ONLY
{
	return atan2f(v2->Y - v1->Y, v2->X - v1->X);
}

float Vector2::Angle(const Vector2 * v1, const Vector2 * v2) __GPU_ONLY
{
	return fast_math::atan2f(v2->Y - v1->Y, v2->X - v1->X);
}

Vector2 Vector2::Barycentric(const Vector2 * v1, const Vector2 * v2, const Vector2 * v3, float b2, float b3) __GPU
{
	float x = barycentric(v1->X, v2->X, v3->X, b2, b3);
	float y = barycentric(v1->Y, v2->Y, v3->Y, b2, b3);
	return vect2(x, y);
}

Vector2 Vector2::CatmullRom(const Vector2 * v1, const Vector2 * v2, const Vector2 * v3, const Vector2 * v4, float a) __GPU
{
	float x = catmullRom(v1->X, v2->X, v3->X, v4->X, a);
	float y = catmullRom(v1->Y, v2->Y, v3->Y, v4->Y, a);
	return vect2(x, y);
}

Vector2 Vector2::Clamp(const Vector2 * mi, const Vector2 * ma, const Vector2 * v) __GPU
{
	return vect2(clamp(mi->X, ma->X, v->X), clamp(mi->Y, ma->Y, v->Y));
}

void Vector2::Clamp(const Vector2 * mi, const Vector2 * ma) __GPU
{
	X = clamp(mi->X, ma->X, X);
	Y = clamp(mi->Y, ma->Y, Y);
}

float Vector2::Distance(const Vector2 * v1, const Vector2 * v2) __CPU_ONLY
{
	return sqrtf(DistanceSquared(v1, v2));
}

float Vector2::Distance(const Vector2 * v1, const Vector2 * v2) __GPU_ONLY
{
	return fast_math::sqrtf(DistanceSquared(v1, v2));
}

float Vector2::DistanceSquared(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return square(abs(v2->X - v1->X)) + square(abs(v2->Y - v1->Y));
}

Vector2 Vector2::Divide(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(v1->X / v2->X, v1->Y / v2->Y);
}

Vector2 Vector2::Divide(const Vector2 * v1, float v2) __GPU
{
	return vect2(v1->X / v2, v1->Y / v2);
}

float Vector2::Dot(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return v1->X * v2->X + v1->Y * v2->Y;
}

bool Vector2::Equals(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return v1->X == v2->X && v1->Y == v2->Y;
}

Vector2 Vector2::Hermite(const Vector2 * v1, const Vector2 * t1, const Vector2 * v2, const Vector2 * t2, float a) __GPU
{
	float num = square(a);
	float num2 = cube(a);
	float num3 = num2 - num;
	float num4 = (num2 - (2.0f * num) + a);
	float num5 = (-2.0f * num2) + (3.0f * num);
	float num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;

	float x = (((v1->X * num6) + (v2->X * num5)) + (t1->X * num4)) + (t2->X * num3);
	float y = (((v1->Y * num6) + (v2->Y * num5)) + (t1->Y * num4)) + (t2->Y * num3);

	return vect2(x, y);
}

float Vector2::Length(void) const __CPU_ONLY
{
	return sqrtf(LengthSquared());
}

float Vector2::Length(void) const __GPU_ONLY
{
	return fast_math::sqrtf(LengthSquared());
}

float Vector2::LengthSquared(void) const __GPU
{
	return square(X) + square(Y);
}

Vector2 Vector2::Lerp(const Vector2 * mi, const Vector2 * ma, float a) __GPU
{
	a = clamp(0.0f, 1.0f, a);
	return vect2(lerp(mi->X, ma->X, a), lerp(mi->Y, ma->Y, a));
}

Vector2 Vector2::Lerp(const Vector2 * mi, const Vector2 * ma, const Vector2 * a) __GPU
{
	a = &vect2::Clamp(&VECT2_ZERO, &VECT2_ONE, a);
	return vect2(lerp(mi->X, ma->X, a->X), lerp(mi->Y, ma->Y, a->Y));
}

Vector2 Vector2::InvLerp(const Vector2 * mi, const Vector2 * ma, float v) __GPU
{
	return vect2(invLerp(mi->X, ma->X, v), invLerp(mi->Y, ma->Y, v));
}

Vector2 Vector2::InvLerp(const Vector2 * mi, const Vector2 * ma, const Vector2 * v) __GPU
{
	return vect2(invLerp(mi->X, ma->X, v->X), invLerp(mi->Y, ma->Y, v->Y));
}

Vector2 Vector2::Max(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(max(v1->X, v2->X), max(v1->Y, v2->Y));
}

Vector2 Vector2::Min(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(min(v1->X, v2->X), min(v1->Y, v2->Y));
}

Vector2 Vector2::Multiply(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(v1->X * v2->X, v1->Y * v2->Y);
}

Vector2 Vector2::Multiply(const Vector2 * v1, float v2) __GPU
{
	return vect2(v1->X * v2, v1->Y * v2);
}

Vector2 Vector2::Negate(const Vector2 * v) __GPU
{
	return vect2(-v->X, -v->Y);
}

void Vector2::Normalize(void) __GPU
{
	float l = Length();
	operator/=(l);
}

Vector2 Vector2::Normalize(const Vector2 * v) __GPU
{
	float l = v->Length();
	return Divide(v, l);
}

Vector2 Vector2::Reflect(const Vector2 * v, const Vector2 * n) __GPU
{
	return *v - vect2(2) * (*v * *n) * *n;
}

Vector2 Vector2::SmoothStep(const Vector2 * v1, const Vector2 * v2, float a) __GPU
{
	a = clamp(0.0f, 1.0f, a);
	a = square(a) * (3.0f - (2.0f * a));

	float x = v1->X + ((v2->X - v1->X) * a);
	float y = v1->Y + ((v2->Y - v1->Y) * a);

	return vect2(x, y);
}

Vector2 Vector2::Subtract(const Vector2 * v1, const Vector2 * v2) __GPU
{
	return vect2(v1->X - v2->X, v1->Y - v2->Y);
}