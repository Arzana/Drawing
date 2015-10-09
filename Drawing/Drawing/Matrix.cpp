#include <math.h>
#include "UtilDefs.h"
#include "Matrix.h"

Matrix::Matrix()
{
	M11 = M12 = M13 = M14 = 0;
	M21 = M22 = M23 = M24 = 0;
	M31 = M32 = M33 = M34 = 0;
	M41 = M42 = M43 = M44 = 0;
}

Matrix::Matrix(float p11, float p12, float p13, float p14, float p21, float p22, float p23, float p24, float p31, float p32, float p33, float p34, float p41, float p42, float p43, float p44)
{
	M11 = p11;
	M12 = p12;
	M13 = p13;
	M14 = p14;
	M21 = p21;
	M22 = p22;
	M23 = p23;
	M24 = p24;
	M31 = p31;
	M32 = p32;
	M33 = p33;
	M34 = p34;
	M41 = p41;
	M42 = p42;
	M43 = p43;
	M44 = p44;
}

Matrix Matrix::FromYawPitchRoll(float y, float p, float r)
{
	Matrix m = RotationX(p);
	m *= RotateAxis(y, m.GetUp());
	m *= RotateAxis(r, m.GetForward());

	return m;
}

Vector3 Matrix::GetBackward() const		{ return -GetForward(); }
Vector3 Matrix::GetDown() const			{ return -GetUp(); }
Vector3 Matrix::GetForward() const		{ return Vector3(M13, M23, M33); }
Vector3 Matrix::GetLeft() const			{ return Vector3(M11, M21, M31); }
Vector3 Matrix::GetRight() const		{ return -GetLeft(); }
Vector3 Matrix::GetUp() const			{ return Vector3(M12, M22, M32); }
Vector3 Matrix::GetTranslation() const	{ return Vector3(M14, M24, M34); }

Matrix Matrix::View(const Vector3& pos, const Vector3& targ, const Vector3& up)
{
	Vector3 zAxis = Vector3::Normalize(targ - pos);
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	float dotX = -Vector3::Dot(xAxis, pos);
	float dotY = -Vector3::Dot(yAxis, pos);
	float dotZ = -Vector3::Dot(zAxis, pos);

	return Matrix(
		xAxis.X,	yAxis.X,	zAxis.X,	0,
		xAxis.Y,	yAxis.Y,	zAxis.Y,	0,
		xAxis.Z,	yAxis.Z,	zAxis.Z,	0,
		dotX,		dotY,		dotZ,		1);
}

Matrix Matrix::Perspective(float fov, float asp, float n, float f)
{
	float tangent = tanf(fov * 0.5f * DEG2RAD);
	float height = n * tangent;
	float width = height * asp;

	float l = -width;
	float b = -height;

	return Matrix(
		2 * n / (width - l),	0,						(width + l) / (width - l),		0,
		0,						2 * n / (height - b),	(height + b) / (height - b),	0,
		0,						0,						-(f + n) / (f - n),				-2 * f * n / (f - n),
		0,						0,						-1,								0);
}

Matrix Matrix::RotateAxis(float rads, const Vector3& axis)
{
	float l = axis.GetLength();
	float x2 = square(axis.X);
	float y2 = square(axis.Y);
	float z2 = square(axis.Z);
	float L = sqrtf(l);
	float c = cosf(rads);
	float s = sinf(rads);

	float m11 = (x2 + (y2 + z2) * c) / l;
	float m12 = (axis.X * axis.Y * (1 - c) - axis.Z * L * s) / l;
	float m13 = (axis.X * axis.Z * (1 - c) + axis.Y * L * s) / l;
	float m21 = (axis.X * axis.Y * (1 - c) + axis.Z * L * s) / l;
	float m22 = (y2 + (x2 + z2) * c) / l;
	float m23 = (axis.Y * axis.Z * (1 - c) - axis.X * L * s) / l;
	float m31 = (axis.X * axis.Z * (1 - c) - axis.Y * L * s) / l;
	float m32 = (axis.Y * axis.Z * (1 - c) + axis.X * L * s) / l;
	float m33 = (z2 + (x2 + y2) * c) / l;

	return Matrix(
		m11, m12, m13, 0,
		m21, m22, m23, 0,
		m31, m32, m33, 0,
		0, 0, 0, 1);
}

Matrix Matrix::RotationX(float thera)
{
	float cos = cosf(thera);
	float sin = sinf(thera);

	return Matrix(
		1, 0, 0, 0,
		0, cos, -sin, 0,
		0, sin, cos, 0,
		0, 0, 0, 1);
}

Matrix Matrix::RotationY(float thera)
{
	float cos = cosf(thera);
	float sin = sinf(thera);

	return Matrix(
		cos, 0, sin, 0,
		0, 1, 0, 0,
		-sin, 0, cos, 0,
		0, 0, 0, 1);
}

Matrix Matrix::RotationZ(float thera)
{
	float cos = cosf(thera);
	float sin = cosf(thera);

	return Matrix(
		cos, -sin, 0, 0,
		sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix Matrix::Scale(float s)
{
	return Matrix(
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1);
}

Matrix Matrix::Scale(const Vector3& s)
{
	return Matrix(
		s.X, 0, 0, 0,
		0, s.Y, 0, 0,
		0, 0, s.Z, 0,
		0, 0, 0, 1);
}

Matrix Matrix::Translation(const Vector3& pos)
{
	return Matrix(
		1, 0, 0, pos.X,
		0, 1, 0, pos.Y,
		0, 0, 1, pos.Z,
		0, 0, 0, 1);
}

Matrix Matrix::operator*(const Matrix& o) const
{
	/* | M11 M12 M13 M14 | *
	 * | M21 M22 M23 M24 | *
	 * | M31 M32 M33 M34 | *
	 * | M41 M42 M43 M44 | */

	float m11 = (M11 * o.M11) + (M12 * o.M21) + (M13 * o.M31) + (M14 * o.M41);
	float m12 = (M11 * o.M12) + (M12 * o.M22) + (M13 * o.M32) + (M14 * o.M42);
	float m13 = (M11 * o.M13) + (M12 * o.M23) + (M13 * o.M33) + (M14 * o.M43);
	float m14 = (M11 * o.M14) + (M12 * o.M24) + (M13 * o.M34) + (M14 * o.M44);

	float m21 = (M21 * o.M11) + (M22 * o.M21) + (M23 * o.M31) + (M24 * o.M41);
	float m22 = (M21 * o.M12) + (M22 * o.M22) + (M23 * o.M32) + (M24 * o.M42);
	float m23 = (M21 * o.M13) + (M22 * o.M23) + (M23 * o.M33) + (M24 * o.M43);
	float m24 = (M21 * o.M14) + (M22 * o.M24) + (M23 * o.M34) + (M24 * o.M44);

	float m31 = (M31 * o.M11) + (M32 * o.M21) + (M33 * o.M31) + (M34 * o.M41);
	float m32 = (M31 * o.M12) + (M32 * o.M22) + (M33 * o.M32) + (M34 * o.M42);
	float m33 = (M31 * o.M13) + (M32 * o.M23) + (M33 * o.M33) + (M34 * o.M43);
	float m34 = (M31 * o.M14) + (M32 * o.M24) + (M33 * o.M34) + (M34 * o.M44);

	float m41 = (M41 * o.M11) + (M42 * o.M21) + (M43 * o.M31) + (M44 * o.M41);
	float m42 = (M41 * o.M12) + (M42 * o.M22) + (M43 * o.M32) + (M44 * o.M42);
	float m43 = (M41 * o.M13) + (M42 * o.M23) + (M43 * o.M33) + (M44 * o.M43);
	float m44 = (M41 * o.M14) + (M42 * o.M24) + (M43 * o.M34) + (M44 * o.M44);

	return Matrix(
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44);
 }

void Matrix::operator*=(const Matrix& o)
{
	float m11 = (M11 * o.M11) + (M12 * o.M21) + (M13 * o.M31) + (M14 * o.M41);
	float m12 = (M11 * o.M12) + (M12 * o.M22) + (M13 * o.M32) + (M14 * o.M42);
	float m13 = (M11 * o.M13) + (M12 * o.M23) + (M13 * o.M33) + (M14 * o.M43);
	float m14 = (M11 * o.M14) + (M12 * o.M24) + (M13 * o.M34) + (M14 * o.M44);

	float m21 = (M21 * o.M11) + (M22 * o.M21) + (M23 * o.M31) + (M24 * o.M41);
	float m22 = (M21 * o.M12) + (M22 * o.M22) + (M23 * o.M32) + (M24 * o.M42);
	float m23 = (M21 * o.M13) + (M22 * o.M23) + (M23 * o.M33) + (M24 * o.M43);
	float m24 = (M21 * o.M14) + (M22 * o.M24) + (M23 * o.M34) + (M24 * o.M44);

	float m31 = (M31 * o.M11) + (M32 * o.M21) + (M33 * o.M31) + (M34 * o.M41);
	float m32 = (M31 * o.M12) + (M32 * o.M22) + (M33 * o.M32) + (M34 * o.M42);
	float m33 = (M31 * o.M13) + (M32 * o.M23) + (M33 * o.M33) + (M34 * o.M43);
	float m34 = (M31 * o.M14) + (M32 * o.M24) + (M33 * o.M34) + (M34 * o.M44);

	float m41 = (M41 * o.M11) + (M42 * o.M21) + (M43 * o.M31) + (M44 * o.M41);
	float m42 = (M41 * o.M12) + (M42 * o.M22) + (M43 * o.M32) + (M44 * o.M42);
	float m43 = (M41 * o.M13) + (M42 * o.M23) + (M43 * o.M33) + (M44 * o.M43);
	float m44 = (M41 * o.M14) + (M42 * o.M24) + (M43 * o.M34) + (M44 * o.M44);

	M11	= m11;
	M12	= m12;
	M13	= m13;
	M14	= m14;

	M21	= m21;
	M22	= m22;
	M23	= m23;
	M24	= m24;

	M31	= m31;
	M32	= m32;
	M33	= m33;
	M34	= m34;

	M41	= m41;
	M42	= m42;
	M43	= m43;
	M44	= m44;
}

Vector4 Matrix::operator>>(const Vector3& v) const
{
	float x = (v.X * M11) + (v.Y * M21) + (v.Z * M31) + M41;
	float y = (v.X * M12) + (v.Y * M22) + (v.Z * M32) + M42;
	float z = (v.X * M13) + (v.Y * M23) + (v.Z * M33) + M43;
	float w = (v.X * M14) + (v.Y * M24) + (v.Z * M34) + M44;

	return Vector4(x, y, z, w);
}

void Matrix::Transform(Matrix *m, Vector3 *source, Vector4 *dest, int length)
{
	for (int i = 0; i < length; i++)
	{
		dest[i] = *m >> source[i];
	}
}