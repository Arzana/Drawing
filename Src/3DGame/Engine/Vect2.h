#pragma once

#define VECT2_ONE		vect2(1)
#define VECT2_UNITX		vect2(1, 0)
#define VECT2_UNITY		vect2(0, 1)
#define VECT2_ZERO		vect2()
#define VECT2_NEGATIVE	vect2(-1)

#ifndef __GPU
#define __GPU				restrict(cpu, amp)
#endif
#ifndef __GPU_ONLY
#define __GPU_ONLY			restrict(amp)
#endif
#ifndef __CPU_ONLY
#define __CPU_ONLY
#endif

typedef struct Vector2
{
	float X, Y;

	Vector2(void) __GPU : X(0), Y(0) { }
	Vector2(float v) __GPU : X(v), Y(v) { }
	Vector2(float x, float y) __GPU : X(x), Y(y) { }

	inline Vector2 operator -(void) const __GPU { return Vector2(-X, -Y); }
	inline Vector2 operator -(const Vector2 &r) const __GPU { return Vector2(X - r.X, Y - r.Y); }
	inline Vector2 operator +(const Vector2 &r) const __GPU { return Vector2(X + r.X, Y + r.Y); }
	inline Vector2 operator /(const Vector2 &r) const __GPU { return Vector2(X / r.X, Y / r.Y); }
	inline Vector2 operator *(const Vector2 &r) const __GPU { return Vector2(X * r.X, Y * r.Y); }
	inline Vector2 operator /(float r) const __GPU { return Vector2(X / r, Y / r); }
	inline Vector2 operator *(float r) const __GPU { return Vector2(X * r, Y * r); }

	inline Vector2 operator -=(const Vector2 &r) __GPU { X -= r.X; Y -= r.Y; return *this; }
	inline Vector2 operator +=(const Vector2 &r) __GPU { X += r.X; Y += r.Y; return *this; }
	inline Vector2 operator /=(const Vector2 &r) __GPU { X /= r.Y; Y /= r.Y; return *this; }
	inline Vector2 operator *=(const Vector2 &r) __GPU { X *= r.X; Y *= r.Y; return *this; }
	inline Vector2 operator /=(float r) __GPU { X /= r; Y /= r; return *this; }
	inline Vector2 operator *=(float r) __GPU { X *= r; Y *= r; return *this; }

	inline bool operator ==(const Vector2 &r) const __GPU { return r.X == X && r.Y == Y; }
	inline bool operator !=(const Vector2 &r) const __GPU { return r.X != X || r.Y != Y; }
	inline float operator [](int i) const __GPU { return ((const float*)this)[i]; }

	static Vector2 Abs(const Vector2 *v) __GPU;
	void Abs() __GPU;
	float Area(void) const __GPU;
	static Vector2 Add(const Vector2 *v1, const Vector2 *v2) __GPU;
	static float Angle(const Vector2 *v1, const Vector2 *v2) __CPU_ONLY;
	static float Angle(const Vector2 *v1, const Vector2 *v2) __GPU_ONLY;
	static Vector2 Barycentric(const Vector2 *v1, const Vector2 *v2, const Vector2 *v3, float b2, float b3) __GPU;
	static Vector2 CatmullRom(const Vector2 *v1, const Vector2 *v2, const Vector2 *v3, const Vector2 *v4, float a) __GPU;
	static Vector2 Clamp(const Vector2 *min, const Vector2 *max, const Vector2 *v) __GPU;
	void Clamp(const Vector2 *min, const Vector2 *max) __GPU;
	static float Distance(const Vector2 *v1, const Vector2 *v2) __GPU;
	static float DistanceSquared(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Divide(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Divide(const Vector2 *v1, float v2) __GPU;
	static float Dot(const Vector2 *v1, const Vector2 *v2) __GPU;
	static bool Equals(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Hermite(const Vector2 *v1, const Vector2 *t1, const Vector2 *v2, const Vector2 *t2, float a) __GPU;
	float Length(void) const __CPU_ONLY;
	float Length(void) const __GPU_ONLY;
	float LengthSquared(void) const __GPU;
	static Vector2 Lerp(const Vector2 *min, const Vector2 *max, float a) __GPU;
	static Vector2 Lerp(const Vector2 *min, const Vector2 *max, const Vector2 *a) __GPU;
	static Vector2 InvLerp(const Vector2 *min, const Vector2 *max, float v) __GPU;
	static Vector2 InvLerp(const Vector2 *min, const Vector2 *max, const Vector2 *v) __GPU;
	static Vector2 Max(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Min(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Multiply(const Vector2 *v1, const Vector2 *v2) __GPU;
	static Vector2 Multiply(const Vector2 *v1, float v2) __GPU;
	static Vector2 Negate(const Vector2 *v) __GPU;
	void Normalize(void) __GPU;
	static Vector2 Normalize(const Vector2 *v) __GPU;
	static float PrepDot(const Vector2 *v1, const Vector2 *v2) __GPU { return v1->X * v2->Y - v1->Y * v2->X; }
	static Vector2 Reflect(const Vector2 *v, const Vector2 *n) __GPU;
	static Vector2 SmoothStep(const Vector2 *min, const Vector2 *max, float a) __GPU;
	static Vector2 Subtract(const Vector2 *v1, const Vector2 *v2) __GPU;
} vect2;