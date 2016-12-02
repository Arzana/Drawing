#include <math.h>
#include <amp_math.h>
#include <amp.h>
#include "Mtrx3.h"

using namespace concurrency;

bool Matrix3::Equals(const Matrix3 * m1, const Matrix3 * m2) __GPU
{
	return m1->M11 == m2->M11 && m1->M12 == m2->M12 && m1->M13 == m2->M13
		&& m1->M21 == m2->M21 && m1->M22 == m2->M22 && m1->M23 == m2->M23
		&& m1->M31 == m2->M31 && m1->M32 == m2->M32 && m1->M33 == m2->M33;
}

Matrix3 Matrix3::CreateRotation(float rads) __CPU_ONLY
{
	float cos = cosf(rads);
	float sin = sinf(rads);
	return Matrix3(cos, -sin, 0, sin, cos, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateRotation(float rads) __GPU_ONLY
{
	float cos = fast_math::cosf(rads);
	float sin = fast_math::sinf(rads);
	return Matrix3(cos, -sin, 0, sin, cos, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateScale(float scale) __GPU
{
	return Matrix3(scale, 0, 0, 0, scale, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateScale(const Vector2 * scale) __GPU
{
	return Matrix3(scale->X, 0, 0, 0, scale->Y, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateTranslation(const Vector2 * pos) __GPU
{
	return Matrix3(1, 0, pos->X, 0, 1, pos->Y, 0, 0, 1);
}

Matrix3 Matrix3::Multiply(const Matrix3 * m1, const Matrix3 * m2) __GPU
{
	float m11 = m1->M11 * m2->M11 + m1->M12 * m2->M21 + m1->M13 * m2->M31;
	float m12 = m1->M11 * m2->M12 + m1->M12 * m2->M22 + m1->M13 * m2->M32;
	float m13 = m1->M11 * m2->M13 + m1->M12 * m2->M23 + m1->M13 * m2->M33;
	float m21 = m1->M21 * m2->M11 + m1->M22 * m2->M21 + m1->M23 * m2->M31;
	float m22 = m1->M21 * m2->M12 + m1->M22 * m2->M22 + m1->M23 * m2->M32;
	float m23 = m1->M21 * m2->M13 + m1->M22 * m2->M23 + m1->M23 * m2->M33;
	float m31 = m1->M31 * m2->M11 + m1->M32 * m2->M21 + m1->M33 * m2->M31;
	float m32 = m1->M31 * m2->M12 + m1->M32 * m2->M22 + m1->M33 * m2->M32;
	float m33 = m1->M31 * m2->M13 + m1->M32 * m2->M23 + m1->M33 * m2->M33;

	return Matrix3(
		m11, m12, m13,
		m21, m22, m23,
		m31, m32, m33);
}

Vector3 Matrix3::Transform(const Matrix3 * m, const Vector2 * v) __GPU
{
	float x = (v->X * m->M11) + (v->Y * m->M12) + m->M13;
	float y = (v->X * m->M21) + (v->Y * m->M22) + m->M23;
	float z = (v->X * m->M31) + (v->Y * m->M32) + m->M33;

	return Vector3(x, y, z);
}

void Matrix3::Transform(const Matrix3 * m, const Vector2 * src, Vector3 * dest, const size_t length) __CPU_ONLY
{
	array_view<const vect2, 1> arr_src(length, src);
	array_view<vect3, 1> arr_dest(length, dest);
	const mtrx3 mtrx = *m;

	parallel_for_each(
		arr_dest.extent,
		[=](index<1> i) __GPU
	{
		arr_dest[i] = mtrx * arr_src[i];
	});
}

Vector2 Matrix3::Translation(void) const __GPU
{
	return Vector2(M13, M23);
}

Matrix3 Matrix3::Transpose(const Matrix3 * m) __GPU
{
	Matrix3 result = Matrix3(*m);

	float b = m->M12;
	float c = m->M13;
	float f = m->M23;

	result.M12 = m->M21;
	result.M13 = m->M31;
	result.M23 = m->M32;
	result.M21 = b;
	result.M31 = c;
	result.M32 = f;

	return result;
}
