#include "Vect4.h"
#include "MathEx.h"

Vector4::Vector4(void)
	: X(0), Y(0), Z(0), W(0)
{ }

Vector4::Vector4(float v)
	: X(v), Y(v), Z(v), W(v)
{ }

Vector4::Vector4(float x, float y, float z, float w)
	: X(x), Y(y), Z(z), W(w)
{ }

Vector4 Vector4::operator-(void) const
{
	return Negate(this);
}

Vector4 Vector4::operator-(const Vector4 & r) const
{
	return Subtract(this, &r);
}

Vector4 Vector4::operator+(const Vector4 & r) const
{
	return Add(this, &r);
}

Vector4 Vector4::operator/(float r) const
{
	return Divide(this, r);
}

Vector4 Vector4::operator/(const Vector4 & r) const
{
	return Divide(this, &r);
}

Vector4 Vector4::operator*(float r) const
{
	return Multiply(this, r);
}

Vector4 Vector4::operator*(const Vector4 & r) const
{
	return Multiply(this, &r);
}

Vector4 Vector4::operator-=(const Vector4 & r)
{
	X -= r.X;
	Y -= r.Y;
	Z -= r.Z;
	W -= r.W;
	return *this;
}

Vector4 Vector4::operator+=(const Vector4 & r)
{
	X += r.X;
	Y += r.Y;
	Z += r.Z;
	W += r.W;
	return *this;
}

Vector4 Vector4::operator/=(float r)
{
	X /= r;
	Y /= r;
	Z /= r;
	W /= r;
	return *this;
}

Vector4 Vector4::operator/=(const Vector4 & r)
{
	X /= r.X;
	Y /= r.Y;
	Z /= r.Z;
	W /= r.W;
	return *this;
}

Vector4 Vector4::operator*=(float r)
{
	X *= r;
	Y *= r;
	Z *= r;
	W *= r;
	return *this;
}

Vector4 Vector4::operator*=(const Vector4 & r)
{
	X *= r.X;
	Y *= r.Y;
	Z *= r.Z;
	W *= r.W;
	return *this;
}

bool Vector4::operator==(const Vector4 & r) const
{
	return Equals(this, &r);
}

bool Vector4::operator!=(const Vector4 & r) const
{
	return !Equals(this, &r);
}

Vector4 Vector4::Abs(const Vector4 * v)
{
	return Vector4(abs(v->X), abs(v->Y), abs(v->Z), abs(v->W));
}

void Vector4::Abs(void)
{
	X = abs(X);
	Y = abs(Y);
	Z = abs(Z);
	W = abs(W);
}

Vector4 Vector4::Add(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(v1->X + v2->X, v1->Y + v2->Y, v1->Z + v2->Z, v1->W + v2->W);
}

float Vector4::Area(void) const
{
	return X * Z;
}

Vector4 Vector4::Barycentric(const Vector4 * v1, const Vector4 * v2, const Vector4 * v3, float b2, float b3)
{
	float x = barycentric(v1->X, v2->X, v3->X, b2, b3);
	float y = barycentric(v1->Y, v2->Y, v3->Y, b2, b3);
	float z = barycentric(v1->Z, v2->Z, v3->Z, b2, b3);
	float w = barycentric(v1->W, v2->W, v3->W, b2, b3);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::CatmullRom(const Vector4 * v1, const Vector4 * v2, const Vector4 * v3, const Vector4 * v4, float a)
{
	float x = catmullRom(v1->X, v2->X, v3->X, v4->X, a);
	float y = catmullRom(v1->Y, v2->Y, v3->Y, v4->Y, a);
	float z = catmullRom(v1->Z, v2->Z, v3->Z, v4->Z, a);
	float w = catmullRom(v1->W, v2->W, v3->W, v4->W, a);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::Clamp(const Vector4 * mi, const Vector4 * ma, const Vector4 * v)
{
	return Vector4(
		clamp(mi->X, ma->X, v->X),
		clamp(mi->Y, ma->Y, v->Y),
		clamp(mi->Z, ma->Z, v->Z),
		clamp(mi->W, ma->W, v->W));
}

void Vector4::Clamp(const Vector4 * mi, const Vector4 * ma)
{
	X = clamp(mi->X, ma->X, X);
	Y = clamp(mi->Y, ma->Y, Y);
	Z = clamp(mi->Z, ma->Z, Z);
	W = clamp(mi->W, ma->W, W);
}

bool Vector4::Clip(void) const
{
	float pW = abs(W);
	float nW = -pW;

	return X < nW || Y < nW || Z < nW || X > pW || Y > pW || Z > pW;
}

float Vector4::Distance(const Vector4 * v1, const Vector4 * v2)
{
	return sqrtf(DistanceSquared(v1, v2));
}

float Vector4::DistanceSquared(const Vector4 * v1, const Vector4 * v2)
{
	float diffX = abs(v2->X - v1->X);
	float diffY = abs(v2->Y - v1->Y);
	float diffZ = abs(v2->Z - v1->Z);
	float diffW = abs(v2->W - v1->W);

	return square(diffX) + square(diffY) + square(diffZ) + square(diffW);
}

Vector4 Vector4::Divide(const Vector4 * v1, float v2)
{
	return Vector4(v1->X / v2, v1->Y / v2, v1->Z / v2, v1->W / v2);
}

Vector4 Vector4::Divide(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(v1->X / v2->X, v1->Y / v2->Y, v1->Z / v2->Z, v1->W / v2->W);
}

float Vector4::Dot(const Vector4 * v1, const Vector4 * v2)
{
	return v1->X * v2->X + v1->Y * v2->Y + v1->Z * v2->Z + v1->W * v2->W;
}

bool Vector4::Equals(const Vector4 * v1, const Vector4 * v2)
{
	return v1->X == v2->X && v1->Y == v2->Y && v1->Z == v2->Z && v1->W == v2->W;
}

Vector4 Vector4::Hermite(const Vector4 * v1, const Vector4 * t1, const Vector4 * v2, const Vector4 * t2, float a)
{
	float num = square(a);
	float num2 = cube(a);
	float num3 = num2 - num;
	float num4 = (num2 - (2.0f * num) + a);
	float num5 = (-2.0f * num2) + (3.0f * num);
	float num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;

	float x = (((v1->X * num6) + (v2->X * num5)) + (t1->X * num4)) + (t2->X * num3);
	float y = (((v1->Y * num6) + (v2->Y * num5)) + (t1->Y * num4)) + (t2->Y * num3);
	float z = (((v1->Z * num6) + (v2->Z * num5)) + (t1->Z * num4)) + (t2->Z * num3);
	float w = (((v1->W * num6) + (v2->W * num5)) + (t1->W * num4)) + (t2->W * num3);

	return Vector4(x, y, z, w);
}

float Vector4::Length(void) const
{
	return sqrtf(LengthSquared());
}

float Vector4::LengthSquared(void) const
{
	return square(X) + square(Y) + square(Z) + square(W);
}

Vector4 Vector4::Lerp(const Vector4 * mi, const Vector4 * ma, float a)
{
	float x = lerp(mi->X, ma->X, a);
	float y = lerp(mi->Y, ma->Y, a);
	float z = lerp(mi->Z, ma->Z, a);
	float w = lerp(mi->W, ma->W, a);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::Lerp(const Vector4 * mi, const Vector4 * ma, const Vector4 * a)
{
	float x = lerp(mi->X, ma->X, a->X);
	float y = lerp(mi->Y, ma->Y, a->Y);
	float z = lerp(mi->Z, ma->Z, a->Z);
	float w = lerp(mi->W, ma->W, a->W);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::InvLerp(const Vector4 * mi, const Vector4 * ma, float v)
{
	float x = invLerp(mi->X, ma->X, v);
	float y = invLerp(mi->Y, ma->Y, v);
	float z = invLerp(mi->Z, ma->Z, v);
	float w = invLerp(mi->W, ma->W, v);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::InvLerp(const Vector4 * mi, const Vector4 * ma, const Vector4 * v)
{
	float x = invLerp(mi->X, ma->X, v->X);
	float y = invLerp(mi->Y, ma->Y, v->Y);
	float z = invLerp(mi->Z, ma->Z, v->Z);
	float w = invLerp(mi->W, ma->W, v->W);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::Max(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(max(v1->X, v2->X), max(v1->Y, v2->Y), max(v1->Z, v2->Z), max(v1->W, v2->W));
}

Vector4 Vector4::Min(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(min(v1->X, v2->X), min(v1->Y, v2->Y), min(v1->Z, v2->Z), min(v1->W, v2->W));
}

Vector4 Vector4::Multiply(const Vector4 * v1, float v2)
{
	return Vector4(v1->X * v2, v1->Y * v2, v1->Z * v2, v1->W * v2);
}

Vector4 Vector4::Multiply(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(v1->X * v2->X, v1->Y * v2->Y, v1->Z * v2->Z, v1->W * v2->W);
}

Vector4 Vector4::Negate(const Vector4 * v)
{
	return Vector4(-v->X, -v->Y, -v->Z, -v->W);
}

void Vector4::Normalize(void)
{
	float l = Length();
	operator/=(l);
}

Vector4 Vector4::Normalize(const Vector4 * v)
{
	float l = v->Length();
	return Divide(v, l);
}

Vector4 Vector4::Reflect(const Vector4 * v, const Vector4 * n)
{
	return *v - Vect4(2) * (*v * *n) * *n;
}

Vector4 Vector4::SmoothStep(const Vector4 * v1, const Vector4 * v2, float a)
{
	a = clamp(0.0f, 1.0f, a);
	a = square(a) * (3.0f - (2.0f * a));

	float x = v1->X + ((v2->X - v1->X) * a);
	float y = v1->Y + ((v2->Y - v1->Y) * a);
	float z = v1->Z + ((v2->Z - v1->Z) * a);
	float w = v1->W + ((v2->W - v1->W) * a);

	return Vector4(x, y, z, w);
}

Vector4 Vector4::Subtract(const Vector4 * v1, const Vector4 * v2)
{
	return Vector4(v1->X - v2->X, v1->Y - v2->Y, v1->Z - v2->Z, v1->W - v2->W);
}

Vector3 Vector4::ToNDC(void) const
{
	return Vector3(X / W, Y / W, Z / W);
}

float Vector4::Volume(void) const
{
	return X * Y * Z;
}

float Vector4::Volume4D(void) const
{
	return X * Y * Z * W;
}
