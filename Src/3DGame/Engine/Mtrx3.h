#pragma once
#include "Vect2.h"
#include "Vect3.h"

#define MTRX3_IDENTITY		Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1)

typedef struct Matrix3
{
	float M11, M12, M13;
	float M21, M22, M23;
	float M31, M32, M33;

	Matrix3(void);
	Matrix3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);

	Matrix3 operator *(const Matrix3 &r) const;
	Vector3 operator *(const Vector2 &r) const;
	Matrix3 operator *=(const Matrix3 &r);

	bool operator ==(const Matrix3 &r) const;
	bool operator !=(const Matrix3 &r) const;

	static bool Equals(const Matrix3 *m1, const Matrix3 *m2);
	static Matrix3 CreateRotation(float rads);
	static Matrix3 CreateScale(float scale);
	static Matrix3 CreateScale(const Vector2 *scale);
	static Matrix3 CreateTranslation(const Vector2 *pos);
	static Matrix3 Multiply(const Matrix3 *m1, const Matrix3 *m2);
	static Vector3 Transform(const Matrix3 *m, const Vector2 *v);
	static Vector3* Transform(const Matrix3 *m, const Vector2 *v, int length);
	Vector2 Translation(void) const;
	static Matrix3 Transpose(const Matrix3 *m);
} Mtrx3;