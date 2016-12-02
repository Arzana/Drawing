#pragma once

#include "Vect3.h"
#include "MathEx.h"

#define VECT4_ONE		vect4(1)
#define VECT4_UNITX		vect4(1, 0, 0, 0)
#define VECT4_UNITY		vect4(0, 1, 0, 0)
#define VECT4_UNITZ		vect4(0, 0, 1, 0)
#define VECT4_UNITW		vect4(0, 0, 0, 1)
#define VECT4_ZERO		vect4()
#define VECT4_NEGATIVE	vect4(-1)

#ifndef _CXXAMP
#define __GPU				restrict(cpu, amp)
#define __GPU_ONLY			restrict(amp)
#define __CPU_ONLY
#endif

typedef struct Vector4
{
	float X, Y, Z, W;

	Vector4(void) __GPU : X(0), Y(0), Z(0), W(0) { }
	Vector4(float v) __GPU : X(v), Y(v), Z(v), W(v) { }
	Vector4(float x, float y, float z, float w) __GPU : X(x), Y(y), Z(z), W(w) { }

	inline Vector4 operator -(void) const __GPU { return Vector4(-X, -Y, -Z, -W); }
	inline Vector4 operator -(const Vector4 &r) const __GPU { return Vector4(X - r.X, Y - r.Y, Z - r.Z, W - r.W); }
	inline Vector4 operator +(const Vector4 &r) const __GPU { return Vector4(X + r.X, Y + r.Y, Z + r.Z, W + r.W); }
	inline Vector4 operator /(const Vector4 &r) const __GPU { return Vector4(X / r.X, Y / r.Y, Z / r.Z, W / r.W); }
	inline Vector4 operator *(const Vector4 &r) const __GPU { return Vector4(X * r.X, Y * r.Y, Z * r.Z, W * r.W); }
	inline Vector4 operator /(float r) const __GPU { return Vector4(X / r, Y / r, Z / r, W / r); }
	inline Vector4 operator *(float r) const __GPU { return Vector4(X * r, Y * r, Z * r, W * r); }

	inline Vector4 operator -=(const Vector4 &r) __GPU { X -= r.X; Y -= r.Y; Z -= r.Z; W -= r.W; return *this; }
	inline Vector4 operator +=(const Vector4 &r) __GPU { X += r.X; Y += r.Y; Z += r.Z; W += r.W; return *this; }
	inline Vector4 operator /=(const Vector4 &r) __GPU { X /= r.X; Y /= r.Y; Z /= r.Z; W /= r.W; return *this; }
	inline Vector4 operator *=(const Vector4 &r) __GPU { X *= r.X; Y *= r.Y; Z *= r.Z; W *= r.W; return *this; }
	inline Vector4 operator /=(float r) __GPU { X /= r; Y /= r; Z /= r; W /= r; return *this; }
	inline Vector4 operator *=(float r) __GPU { X *= r; Y *= r; Z *= r; W *= r; return *this; }

	inline bool operator ==(const Vector4 &r) const __GPU { return r.X == X && r.Y == Y && r.Z == Z && r.W == W; }
	inline bool operator !=(const Vector4 &r) const __GPU { return r.X != X || r.Y != Y || r.Z != Z || r.W != W; }

	static Vector4 Abs(const Vector4 *v) __GPU;
	void Abs(void) __GPU;
	static Vector4 Add(const Vector4 *v1, const Vector4 *v2) __GPU;
	float Area(void) const __GPU;
	static Vector4 Barycentric(const Vector4 *v1, const Vector4 *v2, const Vector4 *v4, float b2, float b4) __GPU;
	static Vector4 CatmullRom(const Vector4 *v1, const Vector4 *v2, const Vector4 *v3, const Vector4 *v4, float a) __GPU;
	static Vector4 Clamp(const Vector4 *min, const Vector4 *max, const Vector4 *v) __GPU;
	void Clamp(const Vector4 *min, const Vector4 *max) __GPU;
	inline bool Clip(void) const __GPU
	{
		float pW = abs(W);
		float nW = -pW;

		return X < nW || Y < nW || Z < nW || X > pW || Y > pW || Z > pW;
	}
	static float Distance(const Vector4 *v1, const Vector4 *v2) __CPU_ONLY;
	static float Distance(const Vector4 *v1, const Vector4 *v2) __GPU_ONLY;
	static float DistanceSquared(const Vector4 *v1, const Vector4 *v2) __GPU;
	static Vector4 Divide(const Vector4 *v1, float v2) __GPU;
	static Vector4 Divide(const Vector4 *v1, const Vector4 *v2) __GPU;
	static float Dot(const Vector4 *v1, const Vector4 *v2) __GPU;
	static bool Equals(const Vector4 *v1, const Vector4 *v2) __GPU;
	static Vector4 Hermite(const Vector4 *v1, const Vector4 *t1, const Vector4 *v2, const Vector4 *t2, float a) __GPU;
	float Length(void) const __CPU_ONLY;
	float Length(void) const __GPU_ONLY;
	float LengthSquared(void) const __GPU;
	static Vector4 Lerp(const Vector4 *min, const Vector4 *max, float a) __GPU;
	static Vector4 Lerp(const Vector4 *min, const Vector4 *max, const Vector4 *a) __GPU;
	static Vector4 InvLerp(const Vector4 *min, const Vector4 *max, float v) __GPU;
	static Vector4 InvLerp(const Vector4 *min, const Vector4 *max, const Vector4 *v) __GPU;
	static Vector4 Max(const Vector4 *v1, const Vector4 *v2) __GPU;
	static Vector4 Min(const Vector4 *v1, const Vector4 *v2) __GPU;
	static Vector4 Multiply(const Vector4 *v1, float v2) __GPU;
	static Vector4 Multiply(const Vector4 *v1, const Vector4 *v2) __GPU;
	static Vector4 Negate(const Vector4 *v) __GPU;
	void Normalize(void) __GPU;
	static Vector4 Normalize(const Vector4 *v) __GPU;
	static Vector4 Reflect(const Vector4 *v, const Vector4 *n) __GPU;
	static Vector4 SmoothStep(const Vector4 *v1, const Vector4 *v2, float a) __GPU;
	static Vector4 Subtract(const Vector4 *v1, const Vector4 *v2) __GPU;
	inline Vector3 ToNDC(void) const __GPU { return vect3(X / W, Y / W, Z / W); }
	float Volume(void) const __GPU;
	float Volume4D(void) const __GPU;
} vect4;