#pragma once

#include "Quaternion.h"

#define MTRX4_IDENTITY		mtrx4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

typedef struct Matrix4
{
	float M11, M12, M13, M14;
	float M21, M22, M23, M24;
	float M31, M32, M33, M34;
	float M41, M42, M43, M44;

	Matrix4(void) __GPU
		: M11(0), M12(0), M13(0), M14(0)
		, M21(0), M22(0), M23(0), M24(0)
		, M31(0), M32(0), M33(0), M34(0)
		, M41(0), M42(0), M43(0), M44(0) { }
	Matrix4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) __GPU
		: M11(m11), M12(m12), M13(m13), M14(m14)
		, M21(m21), M22(m22), M23(m23), M24(m24)
		, M31(m31), M32(m32), M33(m33), M34(m34)
		, M41(m41), M42(m42), M43(m43), M44(m44) { }
	
	inline Matrix4 operator *(const Matrix4 &r) const __GPU
	{
		float m11 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31 + M14 * r.M41;
		float m12 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32 + M14 * r.M42;
		float m13 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33 + M14 * r.M43;
		float m14 = M11 * r.M14 + M12 * r.M24 + M13 * r.M34 + M14 * r.M44;
		float m21 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31 + M24 * r.M41;
		float m22 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32 + M24 * r.M42;
		float m23 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33 + M24 * r.M43;
		float m24 = M21 * r.M14 + M22 * r.M24 + M23 * r.M34 + M24 * r.M44;
		float m31 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31 + M34 * r.M41;
		float m32 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32 + M34 * r.M42;
		float m33 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33 + M34 * r.M43;
		float m34 = M31 * r.M14 + M32 * r.M24 + M33 * r.M34 + M34 * r.M44;
		float m41 = M41 * r.M11 + M42 * r.M21 + M43 * r.M31 + M44 * r.M41;
		float m42 = M41 * r.M12 + M42 * r.M22 + M43 * r.M32 + M44 * r.M42;
		float m43 = M41 * r.M13 + M42 * r.M23 + M43 * r.M33 + M44 * r.M43;
		float m44 = M41 * r.M14 + M42 * r.M24 + M43 * r.M34 + M44 * r.M44;
		return Matrix4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44);
	}
	inline Vector4 operator *(const Vector3 &r) const __GPU
	{
		float x = r.X * M11 + r.Y * M12 + r.Z * M13 + M14;
		float y = r.X * M21 + r.Y * M22 + r.Z * M23 + M24;
		float z = r.X * M31 + r.Y * M32 + r.Z * M33 + M34;
		float w = r.X * M41 + r.Y * M42 + r.Z * M43 + M44;
		return Vector4(x, y, z, w);
	}
	inline Matrix4 operator *=(const Matrix4 &r) __GPU
	{
		float m11 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31 + M14 * r.M41;
		float m12 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32 + M14 * r.M42;
		float m13 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33 + M14 * r.M43;
		float m14 = M11 * r.M14 + M12 * r.M24 + M13 * r.M34 + M14 * r.M44;
		float m21 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31 + M24 * r.M41;
		float m22 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32 + M24 * r.M42;
		float m23 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33 + M24 * r.M43;
		float m24 = M21 * r.M14 + M22 * r.M24 + M23 * r.M34 + M24 * r.M44;
		float m31 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31 + M34 * r.M41;
		float m32 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32 + M34 * r.M42;
		float m33 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33 + M34 * r.M43;
		float m34 = M31 * r.M14 + M32 * r.M24 + M33 * r.M34 + M34 * r.M44;
		float m41 = M41 * r.M11 + M42 * r.M21 + M43 * r.M31 + M44 * r.M41;
		float m42 = M41 * r.M12 + M42 * r.M22 + M43 * r.M32 + M44 * r.M42;
		float m43 = M41 * r.M13 + M42 * r.M23 + M43 * r.M33 + M44 * r.M43;
		float m44 = M41 * r.M14 + M42 * r.M24 + M43 * r.M34 + M44 * r.M44;
		return *this = Matrix4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44);
	}

	inline bool operator ==(const Matrix4 &r) const __GPU
	{
		return r.M11 == M11 && r.M12 == M12 && r.M13 == M13 && r.M14 == M14
			&& r.M21 == M21 && r.M22 == M22 && r.M23 == M23 && r.M24 == M24
			&& r.M31 == M31 && r.M32 == M32 && r.M33 == M33 && r.M34 == M34
			&& r.M41 == M41 && r.M42 == M42 && r.M43 == M43 && r.M44 == M44;
	}
	inline bool operator !=(const Matrix4 &r) const __GPU
	{
		return r.M11 != M11 || r.M12 != M12 || r.M13 != M13 || r.M14 != M14
			|| r.M21 != M21 || r.M22 != M22 || r.M23 != M23 || r.M24 != M24
			|| r.M31 != M31 || r.M32 != M32 || r.M33 != M33 || r.M34 != M34
			|| r.M41 != M41 || r.M42 != M42 || r.M43 != M43 || r.M44 != M44;
	}

	inline Vector3 Backwards(void) const __GPU { return Vector3(-M13, -M23, -M33); }
	inline Vector3 Down(void) const __GPU { return Vector3(-M12, -M22, -M32); }
	inline Vector3 Forwards(void) const __GPU { return Vector3(M13, M23, M33); }
	inline Vector3 Left(void) const __GPU { return Vector3(M11, M21, M31); }
	inline Vector3 Right(void) const __GPU { return Vector3(-M11, -M21, -M31); }
	inline Vector3 Up(void) const __GPU { return Vector3(M12, M22, M32); }
	inline Vector3 Translation(void) const __GPU { return Vector3(M14, M24, M34); }

	static bool Equals(const Matrix4 *m1, const Matrix4 *m2) __GPU;
	static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll) __CPU_ONLY;
	static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll) __GPU_ONLY;
	static Matrix4 CreateFrustrum(float fovY, float aspr, float front, float back) __CPU_ONLY;
	static Matrix4 CreateFrustrum(float fovY, float aspr, float front, float back) __GPU_ONLY;
	static Matrix4 CreateOrthographic(float width, float height, float front, float back) __GPU;
	static Matrix4 CreateRotation(const Vector3 *axis, float rads) __CPU_ONLY;
	static Matrix4 CreateRotation(const Vector3 *axis, float rads) __GPU_ONLY;
	static Matrix4 CreateRotationX(float rads) __CPU_ONLY;
	static Matrix4 CreateRotationX(float rads) __GPU_ONLY;
	static Matrix4 CreateRotationY(float rads) __CPU_ONLY;
	static Matrix4 CreateRotationY(float rads) __GPU_ONLY;
	static Matrix4 CreateRotationZ(float rads) __CPU_ONLY;
	static Matrix4 CreateRotationZ(float rads) __GPU_ONLY;
	static Matrix4 CreateRotationQ(const Quaternion *q) __GPU;
	inline static Matrix4 CreateRotationQ(float yaw, float pitch, float roll) __GPU { return CreateRotationQ(&Quaternion::CreateYawPitchRoll(yaw, pitch, roll)); }
	static Matrix4 CreateScale(float scale) __GPU;
	static Matrix4 CreateScale(const Vector3 *scale) __GPU;
	static Matrix4 CreateTranslation(const Vector3 *pos) __GPU;
	inline static Matrix4 CreateTranslation(float x, float y, float z) __GPU { return CreateTranslation(&vect3(x, y, z)); }
	static Matrix4 CreateView(const Vector3 *camPos, const Vector3 *camRot) __GPU;
	static Matrix4 Multiply(const Matrix4 *m1, const Matrix4 *m2) __GPU;
	static Vector4 Transform(const Matrix4 *m, const Vector3 *v) __GPU;
	static void Transform(const Matrix4 *m, const Vector3 *src, Vector4 *dest, const size_t length) __CPU_ONLY;
	static Matrix4 Transpose(const Matrix4 *m) __GPU;
} mtrx4;