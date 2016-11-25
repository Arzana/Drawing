#pragma once

#include "Vect3.h"

#define VECT4_ONE		vect4(1)
#define VECT4_UNITX		vect4(1, 0, 0, 0)
#define VECT4_UNITY		vect4(0, 1, 0, 0)
#define VECT4_UNITZ		vect4(0, 0, 1, 0)
#define VECT4_UNITW		vect4(0, 0, 0, 1)
#define VECT4_ZERO		vect4()
#define VECT4_NEGATIVE	vect4(-1)

typedef struct Vector4
{
	float X, Y, Z, W;

	Vector4(void);
	Vector4(float v);
	Vector4(float x, float y, float z, float w);

	Vector4 operator -(void) const;
	Vector4 operator -(const Vector4 &r) const;
	Vector4 operator +(const Vector4 &r) const;
	Vector4 operator /(float r) const;
	Vector4 operator /(const Vector4 &r) const;
	Vector4 operator *(float r) const;
	Vector4 operator *(const Vector4 &r) const;

	Vector4 operator -=(const Vector4 &r);
	Vector4 operator +=(const Vector4 &r);
	Vector4 operator /=(float r);
	Vector4 operator /=(const Vector4 &r);
	Vector4 operator *=(float r);
	Vector4 operator *=(const Vector4 &r);

	bool operator ==(const Vector4 &r) const;
	bool operator !=(const Vector4 &r) const;

	static Vector4 Abs(const Vector4 *v);
	void Abs(void);
	static Vector4 Add(const Vector4 *v1, const Vector4 *v2);
	float Area(void) const;
	static Vector4 Barycentric(const Vector4 *v1, const Vector4 *v2, const Vector4 *v4, float b2, float b4);
	static Vector4 CatmullRom(const Vector4 *v1, const Vector4 *v2, const Vector4 *v3, const Vector4 *v4, float a);
	static Vector4 Clamp(const Vector4 *min, const Vector4 *max, const Vector4 *v);
	void Clamp(const Vector4 *min, const Vector4 *max);
	bool Clip(void) const;
	static float Distance(const Vector4 *v1, const Vector4 *v2);
	static float DistanceSquared(const Vector4 *v1, const Vector4 *v2);
	static Vector4 Divide(const Vector4 *v1, float v2);
	static Vector4 Divide(const Vector4 *v1, const Vector4 *v2);
	static float Dot(const Vector4 *v1, const Vector4 *v2);
	static bool Equals(const Vector4 *v1, const Vector4 *v2);
	static Vector4 Hermite(const Vector4 *v1, const Vector4 *t1, const Vector4 *v2, const Vector4 *t2, float a);
	float Length(void) const;
	float LengthSquared(void) const;
	static Vector4 Lerp(const Vector4 *min, const Vector4 *max, float a);
	static Vector4 Lerp(const Vector4 *min, const Vector4 *max, const Vector4 *a);
	static Vector4 InvLerp(const Vector4 *min, const Vector4 *max, float v);
	static Vector4 InvLerp(const Vector4 *min, const Vector4 *max, const Vector4 *v);
	static Vector4 Max(const Vector4 *v1, const Vector4 *v2);
	static Vector4 Min(const Vector4 *v1, const Vector4 *v2);
	static Vector4 Multiply(const Vector4 *v1, float v2);
	static Vector4 Multiply(const Vector4 *v1, const Vector4 *v2);
	static Vector4 Negate(const Vector4 *v);
	void Normalize(void);
	static Vector4 Normalize(const Vector4 *v);
	static Vector4 Reflect(const Vector4 *v, const Vector4 *n);
	static Vector4 SmoothStep(const Vector4 *v1, const Vector4 *v2, float a);
	static Vector4 Subtract(const Vector4 *v1, const Vector4 *v2);
	Vector3 ToNDC(void) const;
	float Volume(void) const;
	float Volume4D(void) const;
} vect4;