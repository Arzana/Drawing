#pragma once

#define VECT3_ONE		vect3(1)
#define VECT3_RIGHT		vect3(1, 0, 0)
#define VECT3_LEFT		vect3(-1, 0, 0)
#define VECT3_UP		vect3(0, 1, 0)
#define VECT3_DOWN		vect3(0, -1, 0)
#define VECT3_BACK		vect3(0, 0, 1)
#define VECT3_FORWARD	vect3(0, 0, -1)
#define VECT3_ZERO		vect3()
#define VECT3_NEGATIVE	vect3(-1)

#ifndef _CXXAMP
#define __GPU				restrict(cpu, amp)
#define __GPU_ONLY			restrict(amp)
#define __CPU_ONLY
#endif

typedef struct Vector3
{
	float X, Y, Z;

	Vector3(void) __GPU : X(0), Y(0), Z(0) { }
	Vector3(float v) __GPU : X(v), Y(v), Z(v) { }
	Vector3(float x, float y, float z) __GPU : X(x), Y(y), Z(z) { }

	inline Vector3 operator -(void) const __GPU { return Vector3(-X, -Y, -Z); }
	inline Vector3 operator -(const Vector3 &r) const __GPU { return Vector3(X - r.X, Y - r.Y, Z - r.Z); }
	inline Vector3 operator +(const Vector3 &r) const __GPU { return Vector3(X + r.X, Y + r.Y, Z + r.Z); }
	inline Vector3 operator /(const Vector3 &r) const __GPU { return Vector3(X / r.X, Y / r.Y, Z / r.Z); }
	inline Vector3 operator *(const Vector3 &r) const __GPU { return Vector3(X * r.X, Y * r.Y, Z * r.Z); }
	inline Vector3 operator /(float r) const __GPU { return Vector3(X / r, Y / r, Z / r); }
	inline Vector3 operator *(float r) const __GPU { return Vector3(X * r, Y * r, Z * r); }

	inline Vector3 operator -=(const Vector3 &r) __GPU { X -= r.X; Y -= r.Y; Z -= r.Z; return *this; }
	inline Vector3 operator +=(const Vector3 &r) __GPU { X += r.X; Y += r.Y; Z += r.Z; return *this; }
	inline Vector3 operator /=(const Vector3 &r) __GPU { X /= r.X; Y /= r.Y; Z /= r.Z; return *this; }
	inline Vector3 operator *=(const Vector3 &r) __GPU { X *= r.X; Y *= r.Y; Z *= r.Z; return *this; }
	inline Vector3 operator /=(float r) __GPU { X /= r; Y /= r; Z /= r; return *this; }
	inline Vector3 operator *=(float r) __GPU { X *= r; Y *= r; Z *= r; return *this; }

	inline bool operator ==(const Vector3 &r) const __GPU { return r.X == X && r.Y == Y && r.Z == Z; }
	inline bool operator !=(const Vector3 &r) const __GPU { return r.X != X || r.Y != Y || r.Z != Z; }

	static Vector3 Abs(const Vector3 *v) __GPU;
	void Abs(void) __GPU;
	static Vector3 Add(const Vector3 *v1, const Vector3 *v2) __GPU;
	float Area(void) const __GPU;
	static Vector3 Barycentric(const Vector3 *v1, const Vector3 *v2, const Vector3 *v3, float b2, float b3) __GPU;
	static Vector3 CatmullRom(const Vector3 *v1, const Vector3 *v2, const Vector3 *v3, const Vector3 *v4, float a) __GPU;
	static Vector3 Clamp(const Vector3 *min, const Vector3 *max, const Vector3 *v) __GPU;
	void Clamp(const Vector3 *min, const Vector3 *max) __GPU;
	static Vector3 Cross(const Vector3 *v1, const Vector3 *v2) __GPU;
	static float Distance(const Vector3 *v1, const Vector3 *v2) __CPU_ONLY;
	static float Distance(const Vector3 *v1, const Vector3 *v2) __GPU_ONLY;
	static float DistanceSquared(const Vector3 *v1, const Vector3 *v2) __GPU;
	static Vector3 Divide(const Vector3 *v1, float v2) __GPU;
	static Vector3 Divide(const Vector3 *v1, const Vector3 *v2) __GPU;
	static float Dot(const Vector3 *v1, const Vector3 *v2) __GPU;
	static bool Equals(const Vector3 *v1, const Vector3 *v2) __GPU;
	static Vector3 Hermite(const Vector3 *v1, const Vector3 *t1, const Vector3 *v2, const Vector3 *t2, float a) __GPU;
	float Length(void) const __CPU_ONLY;
	float Length(void) const __GPU_ONLY;
	float LengthSquared(void) const __GPU;
	static Vector3 Lerp(const Vector3 *min, const Vector3 *max, float a) __GPU;
	static Vector3 Lerp(const Vector3 *min, const Vector3 *max, const Vector3 *a) __GPU;
	static Vector3 InvLerp(const Vector3 *min, const Vector3 *max, float v) __GPU;
	static Vector3 InvLerp(const Vector3 *min, const Vector3 *max, const Vector3 *v) __GPU;
	static Vector3 Max(const Vector3 *v1, const Vector3 *v2) __GPU;
	static Vector3 Min(const Vector3 *v1, const Vector3 *v2) __GPU;
	static Vector3 Multiply(const Vector3 *v1, float v2) __GPU;
	static Vector3 Multiply(const Vector3 *v1, const Vector3 *v2) __GPU;
	static Vector3 Negate(const Vector3 *v) __GPU;
	void Normalize(void) __GPU;
	static Vector3 Normalize(const Vector3 *v) __GPU;
	static Vector3 Reflect(const Vector3 *v, const Vector3 *n) __GPU;
	static Vector3 SmoothStep(const Vector3 *v1, const Vector3 *v2, float a) __GPU;
	static Vector3 Subtract(const Vector3 *v1, const Vector3 *v2) __GPU;
	float Volume(void) const __GPU;
} vect3;