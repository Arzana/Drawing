#include "Quaternion.h"
#include "MathEx.h"

Quaternion::Quaternion(void)
	: X(0), Y(0), Z(0), W(0)
{ }

Quaternion::Quaternion(float x, float y, float z, float w)
	: X(x), Y(y), Z(z), W(w)
{ }

Quaternion::Quaternion(Vect3 v, float w)
	: X(v.X), Y(v.Y), Z(v.Z), W(w)
{ }

Quaternion::Quaternion(Vect4 v)
	: X(v.X), Y(v.Y), Z(v.Z), W(v.W)
{ }

Quaternion Quaternion::operator+(const Quaternion & r) const
{
	return Add(this, &r);
}

Quaternion Quaternion::operator-(const Quaternion & r) const
{
	return Subtract(this, &r);
}

bool Quaternion::operator==(const Quaternion & r) const
{
	return Equals(this, &r);
}

bool Quaternion::operator!=(const Quaternion & r) const
{
	return !Equals(this, &r);
}

Quaternion Quaternion::Add(const Quaternion * q1, const Quaternion * q2)
{
	return Quaternion(q1->X + q2->X, q1->Y + q2->Y, q1->Z + q2->Z, q1->W + q2->W);
}

Quaternion Quaternion::Concat(const Quaternion * q1, const Quaternion * q2)
{
	return Quaternion(
		((q2->X * q1->W) + (q1->X * q2->W)) + ((q2->Y * q1->Z) - (q2->Z * q1->Y)),
		((q2->Y * q1->W) + (q1->Y * q2->W)) + ((q2->Z * q1->X) - (q2->X * q1->Z)),
		((q2->Z * q1->W) + (q1->Z * q2->W)) + ((q2->X * q1->Y) - (q2->Y * q1->X)),
		(q2->W * q1->W) - (((q2->X * q1->X) + (q2->Y * q1->Y)) + (q2->Z * q1->Z)));
}

Quaternion Quaternion::CreateRotation(const Vect3 * axis, float rads)
{
	float hafR = rads * 0.5f;
	float sin = sinf(hafR);
	return Quaternion(axis->X * sin, axis->Y * sin, axis->Z * sin, cosf(hafR));
}

Quaternion Quaternion::CreateYawPitchRoll(float yaw, float pitch, float roll)
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

bool Quaternion::Equals(const Quaternion * q1, const Quaternion * q2)
{
	return q1->X == q2->X
		&& q1->Y == q2->Y
		&& q1->Z == q2->Z
		&& q1->W == q2->W;
}

float Quaternion::Length(void) const
{
	return sqrtf(LengthSquared());
}

float Quaternion::LengthSquared(void) const
{
	return square(X) + square(Y) + square(Z) + square(W);
}

Quaternion Quaternion::Lerp(const Quaternion * min, const Quaternion * max, float a)
{
	Quat qr;
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

	float a3 = 1.0f / sqrtf(((square(qr.X) + square(qr.Y)) + square(qr.Z)) + square(qr.W));
	qr.X *= a3;
	qr.Y *= a3;
	qr.Z *= a3;
	qr.W *= a3;
	return qr;
}

Quaternion Quaternion::SLerp(const Quaternion * min, const Quaternion * max, float a)
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

Quaternion Quaternion::Subtract(const Quaternion * q1, const Quaternion * q2)
{
	return Quaternion(q1->X - q2->X, q1->Y - q2->Y, q1->Z - q2->Z, q1->W - q2->W);
}
