#include <amp.h>
#include "Mtrx4.h"
#include "MathEx.h"

#define __GPU	restrict(cpu, amp)

using namespace concurrency;

bool Matrix4::Equals(const Matrix4 * m1, const Matrix4 * m2) __GPU
{
	return m1->M11 == m2->M11 && m1->M12 == m2->M12 && m1->M13 == m2->M13 && m1->M14 == m2->M14
		&& m1->M21 == m2->M21 && m1->M22 == m2->M22 && m1->M23 == m2->M23 && m1->M24 == m2->M24
		&& m1->M31 == m2->M31 && m1->M32 == m2->M32 && m1->M33 == m2->M33 && m1->M34 == m2->M34
		&& m1->M41 == m2->M41 && m1->M42 == m2->M42 && m1->M43 == m2->M43 && m1->M44 == m2->M44;
}

Matrix4 Matrix4::CreateYawPitchRoll(float yaw, float pitch, float roll)
{
	mtrx4 result = mtrx4::CreateRotation(&VECT3_FORWARD, roll);
	result *= mtrx4::CreateRotation(&VECT3_UP, yaw);
	return result * mtrx4::CreateRotation(&VECT3_RIGHT, pitch);
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

Matrix4 Matrix4::CreateOrthographic(float width, float height, float n, float f) __GPU
{
	float t = height / 2;
	float r = width / 2;
	float b = -t;
	float l = -r;

	float m11 = 2 / (r - l);
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
	float c = cosf(rads);
	float s = sinf(rads);
	float xx = square(axis->X);
	float xy = axis->X * axis->Y;
	float xz = axis->X * axis->Z;
	float yy = sqrt(axis->Y);
	float yz = axis->Y * axis->Z;
	float zz = square(axis->Z);
	float omc = 1 - c;

	float m11 = xx * omc + c;
	float m12 = xy * omc - axis->Z * s;
	float m13 = xz * omc + axis->Y * s;
	float m21 = xy * omc + axis->Z * s;
	float m22 = yy * omc + c;
	float m23 = yz * omc - axis->X * s;
	float m31 = xz * omc - axis->Y * s;
	float m32 = yz * omc + axis->X * s;
	float m33 = zz * omc + c;

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

Matrix4 Matrix4::CreateRotationQ(const Quaternion * q) __GPU
{
	float xx = square(q->X);
	float yy = square(q->Y);
	float zz = square(q->Z);
	float xy = q->X * q->Y;
	float zw = q->Z * q->W;
	float zx = q->Z * q->X;
	float yw = q->Y * q->W;
	float yz = q->Y * q->Z;
	float xw = q->X * q->W;

	float m11 = 1.0f - (2.0f * (yy + zz));
	float m12 = 2.0f * (xy + zw);
	float m13 = 2.0f * (zx - yw);
	float m21 = 2.0f * (xy - zw);
	float m22 = 1.0f - (2.0f * (zz + xx));
	float m23 = 2.0f * (yz + xw);
	float m31 = 2.0f * (zx + yw);
	float m32 = 2.0f * (yz - xw);
	float m33 = 1.0f - (2.0f * (yy + xx));

	return Matrix4(
		m11, m12, m13, 0,
		m21, m22, m23, 0,
		m31, m32, m33, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(float scale) __GPU
{
	return Matrix4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(const Vector3 * scale) __GPU
{
	return Matrix4(scale->X, 0, 0, 0, 0, scale->Y, 0, 0, 0, 0, scale->Z, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::CreateTranslation(const Vector3 * pos) __GPU
{
	return Matrix4(1, 0, 0, pos->X, 0, 1, 0, pos->Y, 0, 0, 1, pos->Z, 0, 0, 0, 1);
}

Matrix4 Matrix4::Multiply(const Matrix4 * m1, const Matrix4 * m2) __GPU
{
	float m11 = m1->M11 * m2->M11 + m1->M12 * m2->M21 + m1->M13 * m2->M31 + m1->M14 * m2->M41;
	float m12 = m1->M11 * m2->M12 + m1->M12 * m2->M22 + m1->M13 * m2->M32 + m1->M14 * m2->M42;
	float m13 = m1->M11 * m2->M13 + m1->M12 * m2->M23 + m1->M13 * m2->M33 + m1->M14 * m2->M43;
	float m14 = m1->M11 * m2->M14 + m1->M12 * m2->M24 + m1->M13 * m2->M34 + m1->M14 * m2->M44;

	float m21 = m1->M21 * m2->M11 + m1->M22 * m2->M21 + m1->M23 * m2->M31 + m1->M24 * m2->M41;
	float m22 = m1->M21 * m2->M12 + m1->M22 * m2->M22 + m1->M23 * m2->M32 + m1->M24 * m2->M42;
	float m23 = m1->M21 * m2->M13 + m1->M22 * m2->M23 + m1->M23 * m2->M33 + m1->M24 * m2->M43;
	float m24 = m1->M21 * m2->M14 + m1->M22 * m2->M24 + m1->M23 * m2->M34 + m1->M24 * m2->M44;

	float m31 = m1->M31 * m2->M11 + m1->M32 * m2->M21 + m1->M33 * m2->M31 + m1->M34 * m2->M41;
	float m32 = m1->M31 * m2->M12 + m1->M32 * m2->M22 + m1->M33 * m2->M32 + m1->M34 * m2->M42;
	float m33 = m1->M31 * m2->M13 + m1->M32 * m2->M23 + m1->M33 * m2->M33 + m1->M34 * m2->M43;
	float m34 = m1->M31 * m2->M14 + m1->M32 * m2->M24 + m1->M33 * m2->M34 + m1->M34 * m2->M44;

	float m41 = m1->M41 * m2->M11 + m1->M42 * m2->M21 + m1->M43 * m2->M31 + m1->M44 * m2->M41;
	float m42 = m1->M41 * m2->M12 + m1->M42 * m2->M22 + m1->M43 * m2->M32 + m1->M44 * m2->M42;
	float m43 = m1->M41 * m2->M13 + m1->M42 * m2->M23 + m1->M43 * m2->M33 + m1->M44 * m2->M43;
	float m44 = m1->M41 * m2->M14 + m1->M42 * m2->M24 + m1->M43 * m2->M34 + m1->M44 * m2->M44;

	return Matrix4(
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44);
}

Vector4 Matrix4::Transform(const Matrix4 * m, const Vector3 * v) __GPU
{
	float x = v->X * m->M11 + v->Y * m->M12 + v->Z * m->M13 + m->M14;
	float y = v->X * m->M21 + v->Y * m->M22 + v->Z * m->M23 + m->M24;
	float z = v->X * m->M31 + v->Y * m->M32 + v->Z * m->M33 + m->M34;
	float w = v->X * m->M41 + v->Y * m->M42 + v->Z * m->M43 + m->M44;

	return Vector4(x, y, z, w);
}

void Matrix4::Transform(const Matrix4 * m, const Vector3 * src, Vector4 * dest, const size_t length)
{
	array_view<const vect3, 1> arr_src(length, src);
	array_view<vect4, 1> arr_dest(length, dest);
	const mtrx4 mtrx = *m;

	parallel_for_each(
		arr_dest.extent,
		[=](index<1> i) __GPU
	{
		arr_dest[i] = mtrx * arr_src[i];
	});
}

Matrix4 Matrix4::Transpose(const Matrix4 * m) __GPU
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
