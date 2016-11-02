#include "Mtrx3.h"
#include "Utils.h"
#include <math.h>

Matrix3::Matrix3(void)
	: M11(0), M12(0), M13(0)
	, M21(0), M22(0), M23(0)
	, M31(0), M32(0), M33(0)
{ }

Matrix3::Matrix3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	: M11(m11), M12(m12), M13(m13)
	, M21(m21), M22(m22), M23(m23)
	, M31(m31), M32(m32), M33(m33)
{ }

Matrix3 Matrix3::operator*(const Matrix3 & r) const
{
	return Multiply(this, &r);
}

Vector3 Matrix3::operator*(const Vector2 & r) const
{
	return Transform(this, &r);
}

Matrix3 Matrix3::operator*=(const Matrix3 & r)
{
	*this = Multiply(this, &r);
	return *this;
}

bool Matrix3::operator==(const Matrix3 & r) const
{
	return Equals(this, &r);
}

bool Matrix3::operator!=(const Matrix3 & r) const
{
	return !Equals(this, &r);
}

bool Matrix3::Equals(const Matrix3 * m1, const Matrix3 * m2)
{
	return m1->M11 == m2->M11 && m1->M12 == m2->M12 && m1->M13 == m2->M13
		&& m1->M21 == m2->M21 && m1->M22 == m2->M22 && m1->M23 == m2->M23
		&& m1->M31 == m2->M31 && m1->M32 == m2->M32 && m1->M33 == m2->M33;
}

Matrix3 Matrix3::CreateRotation(float rads)
{
	float cos = cosf(rads);
	float sin = sinf(rads);
	return Matrix3(cos, -sin, 0, sin, cos, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateScale(float scale)
{
	return Matrix3(scale, 0, 0, 0, scale, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateScale(const Vector2 * scale)
{
	return Matrix3(scale->X, 0, 0, 0, scale->Y, 0, 0, 0, 1);
}

Matrix3 Matrix3::CreateTranslation(const Vector2 * pos)
{
	return Matrix3(1, 0, pos->X, 0, 1, pos->Y, 0, 0, 1);
}

Matrix3 Matrix3::Multiply(const Matrix3 * m1, const Matrix3 * m2)
{
	Matrix3 result = Matrix3();

	result.M11 = m1->M11 * m2->M11 + m1->M12 * m2->M21 + m1->M13 * m2->M31;
	result.M12 = m1->M11 * m2->M12 + m1->M12 * m2->M22 + m1->M13 * m2->M32;
	result.M13 = m1->M11 * m2->M13 + m1->M12 * m2->M23 + m1->M13 * m2->M33;
	result.M21 = m1->M21 * m2->M11 + m1->M22 * m2->M21 + m1->M23 * m2->M31;
	result.M22 = m1->M21 * m2->M12 + m1->M22 * m2->M22 + m1->M23 * m2->M32;
	result.M23 = m1->M21 * m2->M13 + m1->M22 * m2->M23 + m1->M23 * m2->M33;
	result.M31 = m1->M31 * m2->M11 + m1->M32 * m2->M21 + m1->M33 * m2->M31;
	result.M32 = m1->M31 * m2->M12 + m1->M32 * m2->M22 + m1->M33 * m2->M32;
	result.M33 = m1->M31 * m2->M13 + m1->M32 * m2->M23 + m1->M33 * m2->M33;

	return result;
}

Vector3 Matrix3::Transform(const Matrix3 * m, const Vector2 * v)
{
	float x = (v->X * m->M11) + (v->Y * m->M12) + m->M13;
	float y = (v->X * m->M21) + (v->Y * m->M22) + m->M23;
	float z = (v->X * m->M31) + (v->Y * m->M32) + m->M33;

	return Vector3(x, y, z);
}

Vector3 * Matrix3::Transform(const Matrix3 * m, const Vector2 * v, int length)
{
	Vector3 *result = malloc_s(Vect3, length);

	for (size_t i = 0; i < length; i++)
	{
		result[i] = *m * v[i];
	}

	return result;
}

Vector2 Matrix3::Translation(void) const
{
	return Vector2(M13, M23);
}

Matrix3 Matrix3::Transpose(const Matrix3 * m)
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
