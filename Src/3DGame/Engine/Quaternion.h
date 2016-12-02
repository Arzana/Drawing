#pragma once

#include "Vect4.h"

#define QUAT_IDENTITY	quat(0, 0, 0, 1)

#ifndef _CXXAMP
#define __GPU			restrict(cpu, amp)
#define __GPU_ONLY		restrict(amp)
#define __CPU_ONLY
#endif

typedef struct Quaternion
{
	float X, Y, Z, W;

	Quaternion(void) __GPU : X(0), Y(0), Z(0), W(0) { }
	Quaternion(float x, float y, float z, float w) __GPU : X(x), Y(y), Z(z), W(w) { }
	Quaternion(vect3 v, float w) __GPU : X(v.X), Y(v.Y), Z(v.Z), W(w) { }
	Quaternion(vect4 v) __GPU : X(v.X), Y(v.Y), Z(v.Z), W(v.W) { }

	inline Quaternion operator +(const Quaternion &r) const __GPU { return Quaternion(X + r.X, Y + r.Y, Z + r.Z, W + r.W); }
	inline Quaternion operator -(const Quaternion &r) const __GPU { return Quaternion(X - r.X, Y - r.Y, Z - r.Z, W - r.W); }

	inline bool operator ==(const Quaternion &r) const __GPU { return r.X == X && r.Y == Y && r.Z == Z && r.W == W; }
	inline bool operator !=(const Quaternion &r) const __GPU { return r.X != X || r.Y != Y || r.Z != Z || r.W != W; }

	static Quaternion Add(const Quaternion *q1, const Quaternion *q2) __GPU;
	static Quaternion Concat(const Quaternion *q1, const Quaternion *q2) __GPU;
	static Quaternion CreateRotation(const vect3 *axis, float rads) __CPU_ONLY;
	static Quaternion CreateRotation(const vect3 *axis, float rads) __GPU_ONLY;
	static Quaternion CreateYawPitchRoll(float yaw, float pitch, float roll) __CPU_ONLY;
	static Quaternion CreateYawPitchRoll(float yaw, float pitch, float roll) __GPU_ONLY;
	static bool Equals(const Quaternion *q1, const Quaternion *q2) __GPU;
	float Length(void) const __CPU_ONLY;
	float Length(void) const __GPU_ONLY;
	float LengthSquared(void) const __GPU;
	static Quaternion Lerp(const Quaternion *min, const Quaternion *max, float a) __CPU_ONLY;
	static Quaternion Lerp(const Quaternion *min, const Quaternion *max, float a) __GPU_ONLY;
	static Quaternion SLerp(const Quaternion *min, const Quaternion *max, float a) __CPU_ONLY;
	static Quaternion SLerp(const Quaternion *min, const Quaternion *max, float a) __GPU_ONLY;
	static Quaternion Subtract(const Quaternion *q1, const Quaternion *q2) __GPU;
} quat;