#pragma once
#include "Vect2.h"
#include "Vect3.h"

#define MTRX3_IDENTITY		Mtrx3(1, 0, 0, 0, 1, 0, 0, 0, 1)

typedef struct Matrix3
{
	float M11, M12, M13;
	float M21, M22, M23;
	float M31, M32, M33;

	Matrix3(void) __GPU	
		: M11(0), M12(0), M13(0)
		, M21(0), M22(0), M23(0)
		, M31(0), M32(0), M33(0) { }
	Matrix3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) __GPU 
		: M11(m11), M12(m12), M13(m13)
		, M21(m21), M22(m22), M23(m23)
		, M31(m31), M32(m32), M33(m33) { }

	inline Matrix3 operator *(const Matrix3 &r) const __GPU
	{
		float m11 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31;
		float m12 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32;
		float m13 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33;
		float m21 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31;
		float m22 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32;
		float m23 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33;
		float m31 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31;
		float m32 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32;
		float m33 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33;
		return Matrix3(
			m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33);
	}
	inline Vector3 operator *(const Vector2 &r) const __GPU
	{
		float x = r.X * M11 + r.Y * M12 + M13;
		float y = r.X * M21 + r.Y * M22 + M23;
		float z = r.X * M31 + r.Y * M32 + M33;
		return Vector3(x, y, z);
	}
	inline Matrix3 operator *=(const Matrix3 &r) __GPU
	{
		float m11 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31;
		float m12 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32;
		float m13 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33;
		float m21 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31;
		float m22 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32;
		float m23 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33;
		float m31 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31;
		float m32 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32;
		float m33 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33;
		return *this = Matrix3(
			m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33);
	}

	inline bool operator ==(const Matrix3 &r) const __GPU 
	{
		return r.M11 == M11 && r.M12 == M12 && r.M13 == M13 
			&& r.M21 == M21 && r.M22 == M22 && r.M23 == M23 
			&& r.M31 == M31 && r.M32 == M32 && r.M33 == M33; 
	}
	inline bool operator !=(const Matrix3 &r) const __GPU
	{
		return r.M11 != M11 || r.M12 != M12 || r.M13 != M13
			|| r.M21 != M21 || r.M22 != M22 || r.M23 != M23
			|| r.M31 != M31 || r.M32 != M32 || r.M33 != M33;
	}

	static bool Equals(const Matrix3 *m1, const Matrix3 *m2) __GPU;
	static Matrix3 CreateRotation(float rads) __CPU_ONLY;
	static Matrix3 CreateRotation(float rads) __GPU_ONLY;
	static Matrix3 CreateScale(float scale) __GPU;
	static Matrix3 CreateScale(const Vector2 *scale) __GPU;
	static Matrix3 CreateTranslation(const Vector2 *pos) __GPU;
	static Matrix3 Multiply(const Matrix3 *m1, const Matrix3 *m2) __GPU;
	static Vector3 Transform(const Matrix3 *m, const Vector2 *v) __GPU;
	static void Transform(const Matrix3 *m, const Vector2 *src, Vector3 *dest, const size_t length) __CPU_ONLY;
	Vector2 Translation(void) const __GPU;
	static Matrix3 Transpose(const Matrix3 *m) __GPU;
} mtrx3;