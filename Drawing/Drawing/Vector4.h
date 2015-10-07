#ifndef VECTOR4_H
#define VECTOR4_H

#include "Vector3.h"

/* A four dimenstion geomatric primitive. */
struct Vector4
{
public:
	float X, Y, Z, W;

	Vector4 operator	-()							const;
	Vector4 operator	-(const Vector4& other)		const;
	Vector4 operator	+(const Vector4& other)		const;
	void operator		+=(const Vector4& other);
	Vector4 operator	/(float divider)			const;
	Vector4 operator	/(const Vector4& other)		const;
	Vector4 operator	*(float scalar)				const;
	Vector4 operator	*(const Vector4& other)		const;
	bool operator		==(const Vector4& other)	const;
	bool operator		!=(const Vector4& other)	const;

	Vector4();
	Vector4(float value);
	Vector4(float x, float y, float z, float w);

	/* Clamp the value between a specified min and a max value. */
	static Vector4 Clamp(const Vector4& min, const Vector4& max, const Vector4& value);
	/* Clamp the value between a specified min and a max value. */
	static void Clamp(const Vector4& min, const Vector4& max, Vector4* value);
	/* Get the dot product between 2 vectors. */
	static float Dot(const Vector4& a, const Vector4& b);
	/* Get the length of the vector. */
	float GetLength() const;
	float GetLenghtSquared() const;
	/* Perform linear interpollation between 2 vectors with a amount from 0 to 1. */
	static Vector4 Lerp(const Vector4& left, const Vector4& right, float amount);
	/* Get the maximum value from the vectors. */
	static Vector4 Max(const Vector4& left, const Vector4& right);
	/* Get the minimum value from the vectors. */
	static Vector4 Min(const Vector4& left, const Vector4& right);
	/* Normalize the vector to a length of 1. */
	void Normalize();
	/* Normalize a vector to a length of 1. */
	static Vector4 Normalize(const Vector4& value);
	/* Converts the Homogenous coordinate to Cartesian an coordinate. */
	Vector3 ToCartesian() const;
	/* Converts the Homogenous coordinates to Cartesian coordinates. */
	static void ToCartesian(Vector4 *source, Vector3 *destination, int length);
};

typedef Vector4* pVect4;

#endif