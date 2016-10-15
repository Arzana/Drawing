#include "Mtrx4.h"
#include "Mtrx4.h"
#include "Utils.h"
#include "MathEx.h"

Matrix4::Matrix4(void)
{
	M11 = 0, M12 = 0, M13 = 0, M14 = 0;
	M21 = 0, M22 = 0, M23 = 0, M24 = 0;
	M31 = 0, M32 = 0, M33 = 0, M34 = 0;
	M41 = 0, M42 = 0, M43 = 0, M44 = 0;
}

Matrix4::Matrix4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	M11 = m11, M12 = m12, M13 = m13, M14 = m14;
	M21 = m21, M22 = m22, M23 = m23, M24 = m24;
	M31 = m31, M32 = m32, M33 = m33, M34 = m34;
	M41 = m41, M42 = m42, M43 = m43, M44 = m44;
}

Matrix4 Matrix4::operator*(const Matrix4 & r) const
{
	return Multiply(this, &r);
}

Vector4 Matrix4::operator*(const Vector3 & r) const
{
	return Transform(this, &r);
}

Matrix4 Matrix4::operator*=(const Matrix4 & r)
{
	*this = Multiply(this, &r);
	return *this;
}

bool Matrix4::operator==(const Matrix4 & r) const
{
	return Equals(this, &r);
}

bool Matrix4::operator!=(const Matrix4 & r) const
{
	return !Equals(this, &r);
}

Vector3 Matrix4::Backwards(void) const
{
	return Vector3::Negate(&Forwards());
}

Vector3 Matrix4::Down(void) const
{
	return Vector3::Negate(&Up());
}

Vector3 Matrix4::Forwards(void) const
{
	return Vector3(M13, M23, M33);
}

Vector3 Matrix4::Left(void) const
{
	return Vector3(M11, M21, M31);
}

Vector3 Matrix4::Right(void) const
{
	return Vector3::Negate(&Left());
}

Vector3 Matrix4::Up(void) const
{
	return Vector3(M12, M22, M32);
}

Vector3 Matrix4::Translation(void) const
{
	return Vector3(M14, M24, M34);
}

bool Matrix4::Equals(const Matrix4 * m1, const Matrix4 * m2)
{
	return m1->M11 == m2->M11 && m1->M12 == m2->M12 && m1->M13 == m2->M13 && m1->M14 == m2->M14
		&& m1->M21 == m2->M21 && m1->M22 == m2->M22 && m1->M23 == m2->M23 && m1->M24 == m2->M24
		&& m1->M31 == m2->M31 && m1->M32 == m2->M32 && m1->M33 == m2->M33 && m1->M34 == m2->M34
		&& m1->M41 == m2->M41 && m1->M42 == m2->M42 && m1->M43 == m2->M43 && m1->M44 == m2->M44;
}

Matrix4 Matrix4::CreateYawPitchRoll(float yaw, float pitch, float roll)
{
	Matrix4 result = CreateRotationX(pitch);
	result *= CreateRotation(&result.Up(), yaw);
	return result * CreateRotation(&result.Forwards(), roll);
}

Matrix4 Matrix4::CreateView(const Vector3 * pos, const Vector3 * tar, const Vector3 * up)
{
	Vector3 *zAxis = &Vector3::Normalize(&Vector3::Subtract(tar, pos));
	Vector3 *xAxis = &Vector3::Normalize(&Vector3::Cross(up, zAxis));
	Vector3 *yAxis = &Vector3::Cross(zAxis, xAxis);

	float xDot = Vector3::Dot(xAxis, pos);
	float yDot = Vector3::Dot(yAxis, pos);
	float zDot = Vector3::Dot(zAxis, pos);

	return Matrix4(
		xAxis->X, yAxis->X, zAxis->X, 0,
		xAxis->Y, yAxis->Y, zAxis->Y, 0,
		xAxis->Z, yAxis->Z, zAxis->Z, 0,
		xDot, yDot, zDot, 1);
}

Matrix4 Matrix4::CreateFrustrum(float fovY, float aspr, float n, float f)
{
	float tangent = tanf(fovY / 2 * deg2rad);
	float t = n * tangent;
	float r = t * aspr;
	float b = -t;
	float l = -r;

	float m11 = 2 * n / (r - l);
	float m13 = (r + l) / (r - l);
	float m22 = 2 * n / (t - b);
	float m23 = (t + b) / (t - b);
	float m33 = -(f + n) / (f - n);
	float m34 = -(2 * f * n) / (f - n);

	return Matrix4(
		m11, 0, m13, 0,
		0, m22, m23, 0,
		0, 0, m33, m34,
		0, 0, -1, 0);
}

Matrix4 Matrix4::CreateOrthographic(float width, float height, float n, float f)
{
	float t = height / 2;
	float r = width / 2;
	float b = -t;
	float l = -r;

	float m11 = 2 * n / (r - l);
	float m14 = -(r + l) / (r - l);
	float m22 = 2 / (t - b);
	float m24 = -(t + b) / (t - b);
	float m33 = -2 / (f - n);
	float m34 = -(f + n) / (f - n);

	return Matrix4(
		m11, 0, 0, m14,
		0, m22, 0, m24,
		0, 0, m33, m34,
		0 ,0, 0, 1);
}

Matrix4 Matrix4::CreateRotation(const Vector3 * axis, float rads)
{
	float l = axis->Length();
	float x2 = square(axis->X);
	float y2 = square(axis->Y);
	float z2 = square(axis->Z);
	float L = sqrtf(l);
	float c = cosf(rads);
	float s = sinf(rads);

	float m11 = (x2 + (y2 + z2) * c) / l;
	float m12 = (axis->X * axis->Y * (1 - c) - axis->Z * L * s) / l;
	float m13 = (axis->X * axis->Z * (1 - c) + axis->Y * L * s) / l;
	float m21 = (axis->X * axis->Y * (1 - c) + axis->Z * L * s) / l;
	float m22 = (y2 + (x2 + z2) * c) / l;
	float m23 = (axis->Y * axis->Z * (1 - c) - axis->X * L * s) / l;
	float m31 = (axis->X * axis->Z * (1 - c) - axis->Y * L * s) / l;
	float m32 = (axis->Y * axis->Z * (1 - c) + axis->X * L * s) / l;
	float m33 = (z2 + (x2 + y2) * c) / l;

	return Matrix4(
		m11, m12, m13, 0,
		m21, m22, m23, 0,
		m31, m32, m33, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateRotationX(float rads)
{
	float cos = cosf(rads);
	float sin = sinf(rads);

	return Matrix4(
		1, 0, 0, 0,
		0, cos, -sin, 0,
		0, sin, cos, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateRotationY(float rads)
{
	float cos = cosf(rads);
	float sin = sinf(rads);

	return Matrix4(
		cos, 0, sin, 0,
		0, 1, 0, 0,
		-sin, 0, cos, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateRotationZ(float rads)
{
	float cos = cosf(rads);
	float sin = cosf(rads);

	return Matrix4(
		cos, -sin, 0, 0,
		sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(float scale)
{
	return Matrix4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(const Vector3 * scale)
{
	return Matrix4(scale->X, 0, 0, 0, 0, scale->Y, 0, 0, 0, 0, scale->Z, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::CreateTranslation(const Vector3 * pos)
{
	return Matrix4(1, 0, 0, pos->X, 0, 1, 0, pos->Y, 0, 0, 1, pos->Z, 0, 0, 0, 1);
}

Matrix4 Matrix4::Multiply(const Matrix4 * m1, const Matrix4 * m2)
{
	float m11 = (m1->M11 * m2->M11) + (m1->M12 * m2->M21) + (m1->M13 * m2->M31) + (m1->M14 * m2->M41);
	float m12 = (m1->M11 * m2->M12) + (m1->M12 * m2->M22) + (m1->M13 * m2->M32) + (m1->M14 * m2->M42);
	float m13 = (m1->M11 * m2->M13) + (m1->M12 * m2->M23) + (m1->M13 * m2->M33) + (m1->M14 * m2->M43);
	float m14 = (m1->M11 * m2->M14) + (m1->M12 * m2->M24) + (m1->M13 * m2->M34) + (m1->M14 * m2->M44);

	float m21 = (m1->M21 * m2->M11) + (m1->M22 * m2->M21) + (m1->M23 * m2->M31) + (m1->M24 * m2->M41);
	float m22 = (m1->M21 * m2->M12) + (m1->M22 * m2->M22) + (m1->M23 * m2->M32) + (m1->M24 * m2->M42);
	float m23 = (m1->M21 * m2->M13) + (m1->M22 * m2->M23) + (m1->M23 * m2->M33) + (m1->M24 * m2->M43);
	float m24 = (m1->M21 * m2->M14) + (m1->M22 * m2->M24) + (m1->M23 * m2->M34) + (m1->M24 * m2->M44);

	float m31 = (m1->M31 * m2->M11) + (m1->M32 * m2->M21) + (m1->M33 * m2->M31) + (m1->M34 * m2->M41);
	float m32 = (m1->M31 * m2->M12) + (m1->M32 * m2->M22) + (m1->M33 * m2->M32) + (m1->M34 * m2->M42);
	float m33 = (m1->M31 * m2->M13) + (m1->M32 * m2->M23) + (m1->M33 * m2->M33) + (m1->M34 * m2->M43);
	float m34 = (m1->M31 * m2->M14) + (m1->M32 * m2->M24) + (m1->M33 * m2->M34) + (m1->M34 * m2->M44);

	float m41 = (m1->M41 * m2->M11) + (m1->M42 * m2->M21) + (m1->M43 * m2->M31) + (m1->M44 * m2->M41);
	float m42 = (m1->M41 * m2->M12) + (m1->M42 * m2->M22) + (m1->M43 * m2->M32) + (m1->M44 * m2->M42);
	float m43 = (m1->M41 * m2->M13) + (m1->M42 * m2->M23) + (m1->M43 * m2->M33) + (m1->M44 * m2->M43);
	float m44 = (m1->M41 * m2->M14) + (m1->M42 * m2->M24) + (m1->M43 * m2->M34) + (m1->M44 * m2->M44);

	return Matrix4(
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44);
}

Vector4 Matrix4::Transform(const Matrix4 * m, const Vector3 * v)
{
	float x = (v->X * m->M11) + (v->Y * m->M12) + (v->Z * m->M13) + m->M14;
	float y = (v->X * m->M21) + (v->Y * m->M22) + (v->Z * m->M23) + m->M24;
	float z = (v->X * m->M31) + (v->Y * m->M32) + (v->Z * m->M33) + m->M34;
	float w = (v->X * m->M41) + (v->Y * m->M42) + (v->Z * m->M43) + m->M44;

	return Vector4(x, y, z, w);
}

Vector4* Matrix4::Transform(const Matrix4 * m, const Vector3 * v, int length)
{
	Vector4 *result = malloc_s(Vect4, length);

	for (size_t i = 0; i < length; i++)
	{
		result[i] = *m * v[i];
	}

	return result;
}

Matrix4 Matrix4::Transpose(const Matrix4 * m)
{
	Matrix4 result = Matrix4(*m);

	float b = m->M21;
	float c = m->M31;
	float d = m->M41;
	float g = m->M23;
	float h = m->M42;
	float l = m->M43;

	result.M21 = m->M12;
	result.M31 = m->M13;
	result.M41 = m->M14;
	result.M23 = m->M32;
	result.M42 = m->M24;
	result.M43 = m->M43;
	result.M12 = b;
	result.M13 = c;
	result.M14 = d;
	result.M32 = g;
	result.M24 = h;
	result.M34 = l;

	return result;
}
