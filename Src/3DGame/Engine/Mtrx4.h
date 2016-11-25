#pragma once

#include "Quaternion.h"

#define MTRX4_IDENTITY		mtrx4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

typedef struct Matrix4
{
	float M11, M12, M13, M14;
	float M21, M22, M23, M24;
	float M31, M32, M33, M34;
	float M41, M42, M43, M44;

	Matrix4(void);
	Matrix4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);

	Matrix4 operator *(const Matrix4 &r) const;
	Vector4 operator *(const Vector3 &r) const;
	Matrix4 operator *=(const Matrix4 &r);

	bool operator ==(const Matrix4 &r) const;
	bool operator !=(const Matrix4 &r) const;

	Vector3 Backwards(void) const;
	Vector3 Down(void) const;
	Vector3 Forwards(void) const;
	Vector3 Left(void) const;
	Vector3 Right(void) const;
	Vector3 Up(void) const;
	Vector3 Translation(void) const;

	static bool Equals(const Matrix4 *m1, const Matrix4 *m2);
	static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll);
	static Matrix4 CreateFrustrum(float fovY, float aspr, float front, float back);
	static Matrix4 CreateOrthographic(float width, float height, float front, float back);
	static Matrix4 CreateRotation(const Vector3 *axis, float rads);
	static Matrix4 CreateRotationX(float rads);
	static Matrix4 CreateRotationY(float rads);
	static Matrix4 CreateRotationZ(float rads);
	static Matrix4 CreateRotationQ(const Quaternion *q);
	static Matrix4 CreateScale(float scale);
	static Matrix4 CreateScale(const Vector3 *scale);
	static Matrix4 CreateTranslation(const Vector3 *pos);
	static Matrix4 Multiply(const Matrix4 *m1, const Matrix4 *m2);
	static Vector4 Transform(const Matrix4 *m, const Vector3 *v);
	static Vector4* Transform(const Matrix4 *m, const Vector3 *v, int length);
	static Matrix4 Transpose(const Matrix4 *m);
} mtrx4;