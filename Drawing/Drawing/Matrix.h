#ifndef MATRIX_H
#define MATRIX_H

#include "Vector4.h"

struct Matrix
{
public:
	float M11, M12, M13, M14;
	float M21, M22, M23, M24;
	float M31, M32, M33, M34;
	float M41, M42, M43, M44;

	Matrix operator *(const Matrix& other) const;
	void operator *=(const Matrix& other);
	Vector4 operator >>(const Vector3& vect) const;

	Matrix();
	Matrix(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);

	/* Rotate the matrix with three values: yaw, pitch and roll. */
	static Matrix FromYawPitchRoll(float yaw, float pitch, float roll);
	/* Create a view matrix. */
	static Matrix View(const Vector3& cameraPosition, const Vector3& target, const Vector3& up);
	/* Create a  perspective projection matrix using field of view. */
	static Matrix Perspective(float fovY, float aspr, float near, float far);
	/* Rotate the matrix around a specified axis. */
	static Matrix RotateAxis(float radians, const Vector3& axis);
	/* Rotate the matrix around the z-axis (pitch). */
	static Matrix RotationX(float radians);
	/* Rotate the matrix around the y-axis (yaw). */
	static Matrix RotationY(float radians);
	/* Rotate the matrix around the z-axis (roll). */
	static Matrix RotationZ(float radians);
	/* Scale the matrix to a specified amount. */
	static Matrix Scale(float scale);
	/* Scale every matrix dimension to a specified amount. */
	static Matrix Scale(const Vector3& scale);
	/* Translate the matrix to a specified amount. */
	static Matrix Translation(const Vector3& pos);
	/* Transforms a array of vector3. */
	static void Transform(Matrix *matrix, Vector3 *source, Vector4 *destination, int length);
	/* Get the backward vector of the specified matrix. */
	Vector3 GetBackward() const;
	/* Get the down vector of the specified matrix. */
	Vector3 GetDown() const;
	/* Get the forward vector of the specified matrix. */
	Vector3 GetForward() const;
	/* Get the left vector of the specified matrix. */
	Vector3 GetLeft() const;
	/* Get the right vector of the specified matrix. */
	Vector3 GetRight() const;
	/* Get the up vector of the specified matrix. */
	Vector3 GetUp() const;
	/* Gets the translation vector of the matrix. */
	Vector3 GetTranslation() const;
};

typedef Matrix* pMatrix;

#define MTRX_Identity	Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 , 0, 0, 0, 0, 1)

#endif