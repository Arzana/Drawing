#include "Vect3.h"
#include "MathEx.h"

Vector3::Vector3(void)
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

Vector3 Vector3::operator-(void) const
{
	return Negate(this);
}

Vector3 Vector3::operator-(const Vector3 & r) const
{
	return Subtract(this, &r);
}

Vector3 Vector3::operator+(const Vector3 & r) const
{
	return Add(this, &r);
}

Vector3 Vector3::operator/(float r) const
{
	return Divide(this, r);
}

Vector3 Vector3::operator/(const Vector3 & r) const
{
	return Divide(this, &r);
}

Vector3 Vector3::operator*(float r) const
{
	return Multiply(this, r);
}

Vector3 Vector3::operator*(const Vector3 & r) const
{
	return Multiply(this, &r);
}

Vector3 Vector3::operator-=(const Vector3 & r)
{
	X -= r.X;
	Y -= r.Y;
	Z -= r.Z;
	return *this;
}

Vector3 Vector3::operator+=(const Vector3 & r)
{
	X += r.X;
	Y += r.Y;
	return *this;
}

Vector3 Vector3::operator/=(float r)
{
	X /= r;
	Y /= r;
	Z /= r;
	return *this;
}

Vector3 Vector3::operator/=(const Vector3 & r)
{
	X /= r.X;
	Y /= r.Y;
	Z /= r.Z;
	return *this;
}

Vector3 Vector3::operator*=(float r)
{
	X *= r;
	Y *= r;
	Z *= r;
	return *this;
}

Vector3 Vector3::operator*=(const Vector3 & r)
{
	X *= r.X;
	Y *= r.Y;
	Z *= r.Z;
	return *this;
}

bool Vector3::operator==(const Vector3 & r) const
{
	return Equals(this, &r);
}

bool Vector3::operator!=(const Vector3 & r) const
{
	return !Equals(this, &r);
}

Vector3 Vector3::Abs(const Vector3 * v)
{
	return Vector3(abs(v->X), abs(v->Y), abs(v->Z));
}

void Vector3::Abs(void)
{
	X = abs(X);
	Y = abs(Y);
	Z = abs(Z);
}

Vector3 Vector3::Add(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(v1->X + v2->X, v1->Y + v2->Y, v1->Z + v2->Z);
}

float Vector3::Area(void) const
{
	return X * Z;
}

Vector3 Vector3::Barycentric(const Vector3 * v1, const Vector3 * v2, const Vector3 * v3, float b2, float b3)
{
	float x = barycentric(v1->X, v2->X, v3->X, b2, b3);
	float y = barycentric(v1->Y, v2->Y, v3->Y, b2, b3);
	float z = barycentric(v1->Z, v2->Z, v3->Z, b2, b3);
	return Vector3(x, y, z);
}

Vector3 Vector3::CatmullRom(const Vector3 * v1, const Vector3 * v2, const Vector3 * v3, const Vector3 * v4, float a)
{
	float x = catmullRom(v1->X, v2->X, v3->X, v4->X, a);
	float y = catmullRom(v1->Y, v2->Y, v3->Y, v4->Y, a);
	float z = catmullRom(v1->Z, v2->Z, v3->Z, v4->Z, a);

	return Vector3(x, y, z);
}

Vector3 Vector3::Clamp(const Vector3 * mi, const Vector3 * ma, const Vector3 * v)
{
	return Vector3(
		clamp(mi->X, ma->X, v->X),
		clamp(mi->Y, ma->Y, v->Y),
		clamp(mi->Z, ma->Z, v->Z));
}

void Vector3::Clamp(const Vector3 * mi, const Vector3 * ma)
{
	X = clamp(mi->X, ma->X, X);
	Y = clamp(mi->Y, ma->Y, Y);
	Z = clamp(mi->Z, ma->Z, Z);
}

Vector3 Vector3::Cross(const Vector3 * v1, const Vector3 * v2)
{
	float x = (v1->Y * v2->Z) - (v1->Z * v2->Y);
	float y = (v1->Z * v2->X) - (v1->X * v2->Z);
	float z = (v1->X * v2->Y) - (v1->Y * v2->X);

	return Vector3(x, y, z);
}

float Vector3::Distance(const Vector3 * v1, const Vector3 * v2)
{
	return sqrtf(DistanceSquared(v1, v2));
}

float Vector3::DistanceSquared(const Vector3 * v1, const Vector3 * v2)
{
	float diffX = abs(v2->X - v1->X);
	float diffY = abs(v2->Y - v1->Y);
	float diffZ = abs(v2->Z - v1->Z);

	return square(diffX) + square(diffY) + square(diffZ);
}

Vector3 Vector3::Divide(const Vector3 * v1, float v2)
{
	return Vector3(v1->X / v2, v1->Y / v2, v1->Z / v2);
}

Vector3 Vector3::Divide(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(v1->X / v2->X, v1->Y / v2->Y, v1->Z / v2->Z);
}

float Vector3::Dot(const Vector3 * v1, const Vector3 * v2)
{
	return v1->X * v2->X + v1->Y * v2->Y + v1->Z * v2->Z;
}

bool Vector3::Equals(const Vector3 * v1, const Vector3 * v2)
{
	return v1->X == v2->X && v1->Y == v2->Y && v1->Z == v2->Z;
}

Vector3 Vector3::Hermite(const Vector3 * v1, const Vector3 * t1, const Vector3 * v2, const Vector3 * t2, float a)
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

	return Vector3(x, y, z);
}

float Vector3::Length(void) const
{
	return sqrtf(LengthSquared());
}

float Vector3::LengthSquared(void) const
{
	return square(X) + square(Y) + square(Z);
}

Vector3 Vector3::Lerp(const Vector3 * mi, const Vector3 * ma, float a)
{
	float x = lerp(mi->X, ma->X, a);
	float y = lerp(mi->Y, ma->Y, a);
	float z = lerp(mi->Z, ma->Z, a);

	return Vector3(x, y, z);
}

Vector3 Vector3::Lerp(const Vector3 * mi, const Vector3 * ma, const Vector3 * a)
{
	float x = lerp(mi->X, ma->X, a->X);
	float y = lerp(mi->Y, ma->Y, a->Y);
	float z = lerp(mi->Z, ma->Z, a->Z);

	return Vector3(x, y, z);
}

Vector3 Vector3::InvLerp(const Vector3 * mi, const Vector3 * ma, float v)
{
	float x = invLerp(mi->X, ma->X, v);
	float y = invLerp(mi->Y, ma->Y, v);
	float z = invLerp(mi->Z, ma->Z, v);
	
	return Vector3(x, y, z);
}

Vector3 Vector3::InvLerp(const Vector3 * mi, const Vector3 * ma, const Vector3 * v)
{
	float x = invLerp(mi->X, ma->X, v->X);
	float y = invLerp(mi->Y, ma->Y, v->Y);
	float z = invLerp(mi->Z, ma->Z, v->Z);

	return Vector3(x, y, z);
}

Vector3 Vector3::Max(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(max(v1->X, v2->X), max(v1->Y, v2->Y), max(v1->Z, v2->Z));
}

Vector3 Vector3::Min(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(min(v1->X, v2->X), min(v1->Y, v2->Y), min(v1->Z, v2->Z));
}

Vector3 Vector3::Multiply(const Vector3 * v1, float v2)
{
	return Vector3(v1->X * v2, v1->Y * v2, v1->Z * v2);
}

Vector3 Vector3::Multiply(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(v1->X * v2->X, v1->Y * v2->Y, v1->Z * v2->Z);
}

Vector3 Vector3::Negate(const Vector3 * v)
{
	return Vector3(-v->X, -v->Y, -v->Z);
}

void Vector3::Normalize(void)
{
	float l = Length();
	X /= l;
	Y /= l;
	Z /= l;
}

Vector3 Vector3::Normalize(const Vector3 * v)
{
	float l = v->Length();
	return Vector3(v->X / l, v->Y / l, v->Z / l);
}

Vector3 Vector3::Reflect(const Vector3 * v, const Vector3 * n)
{
	return *v - Vect3(2) * (*v * *n) * *n;
}

Vector3 Vector3::SmoothStep(const Vector3 * v1, const Vector3 * v2, float a)
{
	a = clamp(0.0f, 1.0f, a);
	a = square(a) * (3.0f - (2.0f * a));

	float x = v1->X + ((v2->X - v1->X) * a);
	float y = v1->Y + ((v2->Y - v1->Y) * a);
	float z = v1->Z + ((v2->Z - v1->Z) * a);

	return Vector3(x, y, z);
}

Vector3 Vector3::Subtract(const Vector3 * v1, const Vector3 * v2)
{
	return Vector3(v1->X - v2->X, v1->Y - v2->Y, v1->Z - v2->Z);
}

float Vector3::Volume(void) const
{
	return X * Y * Z;
}
