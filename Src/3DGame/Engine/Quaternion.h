#pragma once

#include "Vect4.h"

#define QUAT_IDENTITY	quat(0, 0, 0, 1)

typedef struct Quaternion
{
	float X, Y, Z, W;

	Quaternion(void);
	Quaternion(float x, float y, float z, float w);
	Quaternion(vect3 vect, float w);
	Quaternion(vect4 vect);

	Quaternion operator +(const Quaternion &r) const;
	Quaternion operator -(const Quaternion &r) const;

	bool operator ==(const Quaternion &r) const;
	bool operator !=(const Quaternion &r) const;

	static Quaternion Add(const Quaternion *q1, const Quaternion *q2);
	static Quaternion Concat(const Quaternion *q1, const Quaternion *q2);
	static Quaternion CreateRotation(const vect3 *axis, float rads);
	static Quaternion CreateYawPitchRoll(float yaw, float pitch, float roll);
	static bool Equals(const Quaternion *q1, const Quaternion *q2);
	float Length(void) const;
	float LengthSquared(void) const;
	static Quaternion Lerp(const Quaternion *min, const Quaternion *max, float a);
	static Quaternion SLerp(const Quaternion *min, const Quaternion *max, float a);
	static Quaternion Subtract(const Quaternion *q1, const Quaternion *q2);
} quat;