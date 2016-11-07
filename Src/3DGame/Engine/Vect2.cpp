#include "Vect2.h"
#include "MathEx.h"

Vector2::Vector2(void)
	: X(0), Y(0)
{ }

Vector2::Vector2(float v)
	: X(v), Y(v)
{ }

Vector2::Vector2(float x, float y)
	: X(x), Y(y)
{ }

Vector2 Vector2::operator-(void) const
{
	return Negate(this);
}

Vector2 Vector2::operator-(const Vector2 & r) const
{
	return Subtract(this, &r);
}

Vector2 Vector2::operator+(const Vector2 & r) const
{
	return Add(this, &r);
}

Vector2 Vector2::operator/(float r) const
{
	return Divide(this, r);
}

Vector2 Vector2::operator/(const Vector2 & r) const
{
	return Divide(this, &r);
}

Vector2 Vector2::operator*(float r) const
{
	return Multiply(this, r);
}

Vector2 Vector2::operator*(const Vector2 & right) const
{
	return Multiply(this, &right);
}

Vector2 Vector2::operator-=(const Vector2 & r)
{
	X -= r.X;
	Y -= r.Y;
	return *this;
}

Vector2 Vector2::operator+=(const Vector2 & r)
{
	X += r.X;
	Y += r.Y;
	return *this;
}

Vector2 Vector2::operator/=(float r)
{
	X /= r;
	Y /= r;
	return *this;
}

Vector2 Vector2::operator/=(const Vector2 & r)
{
	X /= r.X;
	Y /= r.Y;
	return *this;
}

Vector2 Vector2::operator*=(float r)
{
	X *= r;
	Y *= r;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2 & r)
{
	X *= r.X;
	Y *= r.Y;
	return *this;
}

bool Vector2::operator==(const Vector2 & r) const
{
	return Equals(this, &r);
}

bool Vector2::operator!=(const Vector2 & r) const
{
	return !Equals(this, &r);
}

Vector2 Vector2::Abs(const Vector2 * v)
{
	return Vect2(abs(v->X), abs(v->Y));
}

void Vector2::Abs()
{
	X = abs(X);
	Y = abs(Y);
}

float Vector2::Area(void) const
{
	return X * Y;
}

Vector2 Vector2::Add(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(v1->X + v2->X, v1->Y + v2->Y);
}

float Vector2::Angle(const Vector2 * v1, const Vector2 * v2)
{
	return atan2f(v2->Y - v1->Y, v2->X - v1->X);
}

Vector2 Vector2::Barycentric(const Vector2 * v1, const Vector2 * v2, const Vector2 * v3, float b2, float b3)
{
	float x = barycentric(v1->X, v2->X, v3->X, b2, b3);
	float y = barycentric(v1->Y, v2->Y, v3->Y, b2, b3);
	return Vect2(x, y);
}

Vector2 Vector2::CatmullRom(const Vector2 * v1, const Vector2 * v2, const Vector2 * v3, const Vector2 * v4, float a)
{
	float x = catmullRom(v1->X, v2->X, v3->X, v4->X, a);
	float y = catmullRom(v1->Y, v2->Y, v3->Y, v4->Y, a);
	return Vect2(x, y);
}

Vector2 Vector2::Clamp(const Vector2 * mi, const Vector2 * ma, const Vector2 * v)
{
	return Vect2(clamp(mi->X, ma->X, v->X), clamp(mi->Y, ma->Y, v->Y));
}

void Vector2::Clamp(const Vector2 * mi, const Vector2 * ma)
{
	X = clamp(mi->X, ma->X, X);
	Y = clamp(mi->Y, ma->Y, Y);
}

float Vector2::Distance(const Vector2 * v1, const Vector2 * v2)
{
	return sqrtf(DistanceSquared(v1, v2));
}

float Vector2::DistanceSquared(const Vector2 * v1, const Vector2 * v2)
{
	return square(abs(v2->X - v1->X)) + square(abs(v2->Y - v1->Y));
}

Vector2 Vector2::Divide(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(v1->X / v2->X, v1->Y / v2->Y);
}

Vector2 Vector2::Divide(const Vector2 * v1, float v2)
{
	return Vect2(v1->X / v2, v1->Y / v2);
}

float Vector2::Dot(const Vector2 * v1, const Vector2 * v2)
{
	return v1->X * v2->X + v1->Y * v2->Y;
}

bool Vector2::Equals(const Vector2 * v1, const Vector2 * v2)
{
	return v1->X == v2->X && v1->Y == v2->Y;
}

Vector2 Vector2::Hermite(const Vector2 * v1, const Vector2 * t1, const Vector2 * v2, const Vector2 * t2, float a)
{
	float num = square(a);
	float num2 = cube(a);
	float num3 = num2 - num;
	float num4 = (num2 - (2.0f * num) + a);
	float num5 = (-2.0f * num2) + (3.0f * num);
	float num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;

	float x = (((v1->X * num6) + (v2->X * num5)) + (t1->X * num4)) + (t2->X * num3);
	float y = (((v1->Y * num6) + (v2->Y * num5)) + (t1->Y * num4)) + (t2->Y * num3);

	return Vect2(x, y);
}

float Vector2::Length(void) const
{
	return sqrtf(LengthSquared());
}

float Vector2::LengthSquared(void) const
{
	return square(X) + square(Y);
}

Vector2 Vector2::Lerp(const Vector2 * mi, const Vector2 * ma, float a)
{
	a = clamp(0.0f, 1.0f, a);
	return Vect2(lerp(mi->X, ma->X, a), lerp(mi->Y, ma->Y, a));
}

Vector2 Vector2::Lerp(const Vector2 * mi, const Vector2 * ma, const Vector2 * a)
{
	a = &Vect2::Clamp(&VECT2_ZERO, &VECT2_ONE, a);
	return Vect2(lerp(mi->X, ma->X, a->X), lerp(mi->Y, ma->Y, a->Y));
}

Vector2 Vector2::InvLerp(const Vector2 * mi, const Vector2 * ma, float v)
{
	return Vect2(invLerp(mi->X, ma->X, v), invLerp(mi->Y, ma->Y, v));
}

Vector2 Vector2::InvLerp(const Vector2 * mi, const Vector2 * ma, const Vector2 * v)
{
	return Vect2(invLerp(mi->X, ma->X, v->X), invLerp(mi->Y, ma->Y, v->Y));
}

Vector2 Vector2::Max(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(max(v1->X, v2->X), max(v1->Y, v2->Y));
}

Vector2 Vector2::Min(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(min(v1->X, v2->X), min(v1->Y, v2->Y));
}

Vector2 Vector2::Multiply(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(v1->X * v2->X, v1->Y * v2->Y);
}

Vector2 Vector2::Multiply(const Vector2 * v1, float v2)
{
	return Vect2(v1->X * v2, v1->Y * v2);
}

Vector2 Vector2::Negate(const Vector2 * v)
{
	return Vect2(-v->X, -v->Y);
}

void Vector2::Normalize(void)
{
	float l = Length();
	operator/=(l);
}

Vector2 Vector2::Normalize(const Vector2 * v)
{
	float l = v->Length();
	return Divide(v, l);
}

float Vector2::PrepDot(const Vector2 * v1, const Vector2 * v2)
{
	return v1->X * v2->Y - v1->Y * v2->X;
}

Vector2 Vector2::Reflect(const Vector2 * v, const Vector2 * n)
{
	return *v - Vect2(2) * (*v * *n) * *n;
}

Vector2 Vector2::SmoothStep(const Vector2 * v1, const Vector2 * v2, float a)
{
	a = clamp(0.0f, 1.0f, a);
	a = square(a) * (3.0f - (2.0f * a));

	float x = v1->X + ((v2->X - v1->X) * a);
	float y = v1->Y + ((v2->Y - v1->Y) * a);

	return Vect2(x, y);
}

Vector2 Vector2::Subtract(const Vector2 * v1, const Vector2 * v2)
{
	return Vect2(v1->X - v2->X, v1->Y - v2->Y);
}