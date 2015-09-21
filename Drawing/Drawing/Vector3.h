#ifndef VECTOR3_H
#define VECTOR3_H

/* A three dimenstion geomatric primitive. */
struct Vector3
{
public:
	float X, Y, Z;

	Vector3 operator	-()							const;
	Vector3 operator	-(const Vector3& other)		const;
	Vector3 operator	+(const Vector3& other)		const;
	void operator	+=(const Vector3& other);
	Vector3 operator	/(float divider)			const;
	Vector3 operator	/(const Vector3& other)		const;
	Vector3 operator	*(float scalar)				const;
	Vector3 operator	*(const Vector3& other)		const;
	bool operator		==(const Vector3& other)	const;
	bool operator		!=(const Vector3& other)	const;

	Vector3();
	Vector3(float value);
	Vector3(float x, float y, float z);

	/* Clamp the value between a specified min and a max value. */
	static Vector3 Clamp(const Vector3& min, const Vector3& max, const Vector3& value);
	/* Clamp the value between a specified min and a max value. */
	static void Clamp(const Vector3& min, const Vector3& max, Vector3* value);
	/* Get the cross product between two vectors. */
	static Vector3 Cross(const Vector3& a, const Vector3& b);
	/* Get the direct distance between 2 vectors. */
	static float Distance(const Vector3& left, const Vector3& right);
	static float DistanceSquared(const Vector3& left, const Vector3& right);
	/* Get the dot product between 2 vectors. */
	static float Dot(const Vector3& a, const Vector3& b);
	/* Get the length of the vector. */
	float GetLength() const;
	float GetLenghtSquared() const;
	/* Perform linear interpollation between 2 vectors with a amount from 0 to 1. */
	static Vector3 Lerp(const Vector3& left, const Vector3& right, float amount);
	/* Get the maximum value from the vectors. */
	static Vector3 Max(const Vector3& left, const Vector3& right);
	/* Get the minimum value from the vectors. */
	static Vector3 Min(const Vector3& left, const Vector3& right);
	/* Normalize the vector to a length of 1. */
	void Normalize();
	/* Normalize a vector to a length of 1. */
	static Vector3 Normalize(const Vector3& value);
};

typedef Vector3* pVect3;

#define VECT_BACKWARD	Vector3(0, 0, -1)
#define VECT_DOWN		Vector3(0, -1, 0)
#define VECT_FORWARD	Vector3(0, 0, 1)
#define VECT_LEFT		Vector3(-1, 0, 0)
#define VECT_NEGATIVE	Vector3(-1, -1, -1)
#define VECT_ONE		Vector3(1)
#define VECT_RIGHT		Vector3(1, 0, 0)
#define VECT_UP			Vector3(0, 1, 0)
#define VECT_ZERO		Vector3()

#endif