#ifndef Vector2_H
#define Vector2_H

/* A two dimenstion geomatric primitive. */
struct Vector2
{
public:
	float X, Y;

	Vector2 operator	-()							const;
	Vector2 operator	-(const Vector2& other)		const;
	Vector2 operator	+(const Vector2& other)		const;
	void operator		+=(const Vector2& other);
	Vector2 operator	/(float divider)			const;
	Vector2 operator	/(const Vector2& other)		const;
	Vector2 operator	*(float scalar)				const;
	Vector2 operator	*(const Vector2& other)		const;
	bool operator		==(const Vector2& other)	const;
	bool operator		!=(const Vector2& other)	const;

	Vector2();
	Vector2(float value);
	Vector2(float x, float y);

	/* Clamp the value between a specified min and a max value. */
	static Vector2 Clamp(const Vector2& min, const Vector2& max, const Vector2& value);
	/* Clamp the value between a specified min and a max value. */
	static void Clamp(const Vector2& min, const Vector2& max, Vector2* value);
	/* Get the direct distance between 2 vectors. */
	static float Distance(const Vector2& left, const Vector2& right);
	static float DistanceSquared(const Vector2& left, const Vector2& right);
	/* Get the dot product between 2 vectors. */
	static float Dot(const Vector2& a, const Vector2& b);
	/* Get the length of the vector. */
	float GetLength() const;
	float GetLenghtSquared() const;
	/* Perform linear interpollation between 2 vectors with a amount from 0 to 1. */
	static Vector2 Lerp(const Vector2& left, const Vector2& right, float amount);
	/* Get the maximum value from the vectors. */
	static Vector2 Max(const Vector2& left, const Vector2& right);
	/* Get the minimum value from the vectors. */
	static Vector2 Min(const Vector2& left, const Vector2& right);
	/* Normalize the vector to a length of 1. */
	void Normalize();
	/* Normalize a vector to a length of 1. */
	static Vector2 Normalize(const Vector2& value);
};

#endif