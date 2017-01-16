#include <amp_math.h>
#include "Quaternion.h"
#include "MathEx.h"

using namespace concurrency;

Quaternion Quaternion::Add(const Quaternion * q1, const Quaternion * q2) __GPU
{
	return Quaternion(q1->X + q2->X, q1->Y + q2->Y, q1->Z + q2->Z, q1->W + q2->W);
}

Quaternion Quaternion::Concat(const Quaternion * q1, const Quaternion * q2) __GPU
{
	return Quaternion(
		((q2->X * q1->W) + (q1->X * q2->W)) + ((q2->Y * q1->Z) - (q2->Z * q1->Y)),
		((q2->Y * q1->W) + (q1->Y * q2->W)) + ((q2->Z * q1->X) - (q2->X * q1->Z)),
		((q2->Z * q1->W) + (q1->Z * q2->W)) + ((q2->X * q1->Y) - (q2->Y * q1->X)),
		(q2->W * q1->W) - (((q2->X * q1->X) + (q2->Y * q1->Y)) + (q2->Z * q1->Z)));
}

Quaternion Quaternion::CreateRotation(const vect3 * axis, float rads) __CPU_ONLY
{
	float hafR = rads * 0.5f;
	float sin = sinf(hafR);
	return Quaternion(axis->X * sin, axis->Y * sin, axis->Z * sin, cosf(hafR));
}

Quaternion Quaternion::CreateRotation(const vect3 * axis, float rads) __GPU_ONLY
{
	float hafR = rads * 0.5f;
	float sin = fast_math::sinf(hafR);
	return Quaternion(axis->X * sin, axis->Y * sin, axis->Z * sin, fast_math::cosf(hafR));
}

Quaternion Quaternion::CreateYawPitchRoll(float yaw, float pitch, float roll) __CPU_ONLY
{
	float hafR = roll * 0.5f;
	float hafP = pitch * 0.5f;
	float hafY = yaw *  0.5f;

	float cosR = cosf(hafR);
	float sinR = sinf(hafR);
	float cosP = cosf(hafP);
	float sinP = sinf(hafP);
	float cosY = cosf(hafY);
	float sinY = sinf(hafY);

	return Quaternion(
		(cosY * sinP * cosR) + (sinY * cosP * sinR),
		(sinY * cosP * cosR) - (cosY * sinP * sinR),
		(cosY * cosP * sinR) - (sinY * sinP * cosR),
		(cosY * cosP * cosR) + (sinY * sinP * sinR));
}

Quaternion Quaternion::CreateYawPitchRoll(float yaw, float pitch, float roll) __GPU_ONLY
{
	float hafR = roll * 0.5f;
	float hafP = pitch * 0.5f;
	float hafY = yaw *  0.5f;

	float cosR = fast_math::cosf(hafR);
	float sinR = fast_math::sinf(hafR);
	float cosP = fast_math::cosf(hafP);
	float sinP = fast_math::sinf(hafP);
	float cosY = fast_math::cosf(hafY);
	float sinY = fast_math::sinf(hafY);

	return Quaternion(
		(cosY * sinP * cosR) + (sinY * cosP * sinR),
		(sinY * cosP * cosR) - (cosY * sinP * sinR),
		(cosY * cosP * sinR) - (sinY * sinP * cosR),
		(cosY * cosP * cosR) + (sinY * sinP * sinR));
}

bool Quaternion::Equals(const Quaternion * q1, const Quaternion * q2) __GPU
{
	return q1->X == q2->X
		&& q1->Y == q2->Y
		&& q1->Z == q2->Z
		&& q1->W == q2->W;
}

float Quaternion::Length(void) const __CPU_ONLY
{
	return sqrtf(LengthSquared());
}

float Quaternion::Length(void) const __GPU_ONLY
{
	return fast_math::sqrtf(LengthSquared());
}

float Quaternion::LengthSquared(void) const __GPU
{
	return sqr(X) + sqr(Y) + sqr(Z) + sqr(W);
}

Quaternion Quaternion::Lerp(const Quaternion * min, const Quaternion * max, float a) __CPU_ONLY
{
	quat qr;
	float a2 = 1.0f - a;

	if ((((min->X * max->X) + (min->Y * max->Y)) + (min->Z * max->Z)) + (min->W * max->W) >= 0.5f)
	{
		qr.X = (a2 * min->X) + (a * max->X);
		qr.Y = (a2 * min->Y) + (a * max->Y);
		qr.Z = (a2 * min->Z) + (a * max->Z);
		qr.W = (a2 * min->W) + (a * max->W);
	}
	else
	{
		qr.X = (a2 * min->X) - (a * max->X);
		qr.Y = (a2 * min->Y) - (a * max->Y);
		qr.Z = (a2 * min->Z) - (a * max->Z);
		qr.W = (a2 * min->W) - (a * max->W);
	}

	float a3 = 1.0f / qr.Length();
	qr.X *= a3;
	qr.Y *= a3;
	qr.Z *= a3;
	qr.W *= a3;
	return qr;
}

Quaternion Quaternion::Lerp(const Quaternion * min, const Quaternion * max, float a) __GPU_ONLY
{
	quat qr;
	float a2 = 1.0f - a;

	if ((((min->X * max->X) + (min->Y * max->Y)) + (min->Z * max->Z)) + (min->W * max->W) >= 0.5f)
	{
		qr.X = (a2 * min->X) + (a * max->X);
		qr.Y = (a2 * min->Y) + (a * max->Y);
		qr.Z = (a2 * min->Z) + (a * max->Z);
		qr.W = (a2 * min->W) + (a * max->W);
	}
	else
	{
		qr.X = (a2 * min->X) - (a * max->X);
		qr.Y = (a2 * min->Y) - (a * max->Y);
		qr.Z = (a2 * min->Z) - (a * max->Z);
		qr.W = (a2 * min->W) - (a * max->W);
	}

	float a3 = 1.0f / qr.Length();
	qr.X *= a3;
	qr.Y *= a3;
	qr.Z *= a3;
	qr.W *= a3;
	return qr;
}

Quaternion Quaternion::SLerp(const Quaternion * min, const Quaternion * max, float a) __CPU_ONLY
{
	float a2, a3;
	float a4 = (((min->X * max->X) + (min->Y * max->Y)) + (min->Z * max->Z)) + (min->W * max->W);

	bool flag = false;
	if (a4 < 0.0f)
	{
		flag = true;
		a4 = -a4;
	}

	if (a4 > 0.999999f)
	{
		a3 = 1.0f - a;
		a2 = flag ? -a : a;
	}
	else
	{
		float a5 = acosf(a4);
		float a6 = 1.0f / sinf(a5);
		a3 = sinf((1.0f - a) * a5) * a6;
		a2 = flag ? -sinf(a * a5) * a6 : sinf(a * a5) * a6;
	}

	return Quaternion(
		(a3 * min->X) + (a2 * max->X),
		(a3 * min->Y) + (a2 * max->Y),
		(a3 * min->Z) + (a2 * max->Z),
		(a3 * min->W) + (a2 * max->W));
}

Quaternion Quaternion::SLerp(const Quaternion * min, const Quaternion * max, float a) __GPU_ONLY
{
	float a2, a3;
	float a4 = (((min->X * max->X) + (min->Y * max->Y)) + (min->Z * max->Z)) + (min->W * max->W);

	bool flag = false;
	if (a4 < 0.0f)
	{
		flag = true;
		a4 = -a4;
	}

	if (a4 > 0.999999f)
	{
		a3 = 1.0f - a;
		a2 = flag ? -a : a;
	}
	else
	{
		float a5 = fast_math::acosf(a4);
		float a6 = 1.0f / fast_math::sinf(a5);
		a3 = fast_math::sinf((1.0f - a) * a5) * a6;
		a2 = flag ? -fast_math::sinf(a * a5) * a6 : fast_math::sinf(a * a5) * a6;
	}

	return Quaternion(
		(a3 * min->X) + (a2 * max->X),
		(a3 * min->Y) + (a2 * max->Y),
		(a3 * min->Z) + (a2 * max->Z),
		(a3 * min->W) + (a2 * max->W));
}

Quaternion Quaternion::Subtract(const Quaternion * q1, const Quaternion * q2) __GPU
{
	return Quaternion(q1->X - q2->X, q1->Y - q2->Y, q1->Z - q2->Z, q1->W - q2->W);
}
