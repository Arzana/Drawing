#pragma once

#define VECT2_ONE		Vect2(1)
#define VECT2_UNITX		Vect2(1, 0)
#define VECT2_UNITY		Vect2(0, 1)
#define VECT2_ZERO		Vect2()
#define VECT2_NEGATIVE	Vect2(-1)

typedef struct Vector2
{
	float X, Y;

	Vector2(void);
	Vector2(float v);
	Vector2(float x, float y);

	Vector2 operator -(void) const;
	Vector2 operator -(const Vector2 &r) const;
	Vector2 operator +(const Vector2 &r) const;
	Vector2 operator /(float r) const;
	Vector2 operator /(const Vector2 &r) const;
	Vector2 operator *(float r) const;
	Vector2 operator *(const Vector2 &r) const;

	Vector2 operator -=(const Vector2 &r);
	Vector2 operator +=(const Vector2 &r);
	Vector2 operator /=(float r);
	Vector2 operator /=(const Vector2 &r);
	Vector2 operator *=(float r);
	Vector2 operator *=(const Vector2 &r);

	bool operator ==(const Vector2 &r) const;
	bool operator !=(const Vector2 &r) const;

	static Vector2 Abs(const Vector2 *v);
	void Abs();
	float Area(void) const;
	static Vector2 Add(const Vector2 *v1, const Vector2 *v2);
	static float Angle(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Barycentric(const Vector2 *v1, const Vector2 *v2, const Vector2 *v3, float b2, float b3);
	static Vector2 CatmullRom(const Vector2 *v1, const Vector2 *v2, const Vector2 *v3, const Vector2 *v4, float a);
	static Vector2 Clamp(const Vector2 *min, const Vector2 *max, const Vector2 *v);
	void Clamp(const Vector2 *min, const Vector2 *max);
	static float Distance(const Vector2 *v1, const Vector2 *v2);
	static float DistanceSquared(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Divide(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Divide(const Vector2 *v1, float v2);
	static float Dot(const Vector2 *v1, const Vector2 *v2);
	static bool Equals(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Hermite(const Vector2 *v1, const Vector2 *t1, const Vector2 *v2, const Vector2 *t2, float a);
	float Length(void) const;
	float LengthSquared(void) const;
	static Vector2 Lerp(const Vector2 *min, const Vector2 *max, float a);
	static Vector2 Lerp(const Vector2 *min, const Vector2 *max, const Vector2 *a);
	static Vector2 InvLerp(const Vector2 *min, const Vector2 *max, float v);
	static Vector2 InvLerp(const Vector2 *min, const Vector2 *max, const Vector2 *v);
	static Vector2 Max(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Min(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Multiply(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Multiply(const Vector2 *v1, float v2);
	static Vector2 Negate(const Vector2 *v);
	void Normalize(void);
	static Vector2 Normalize(const Vector2 *v);
	static float PrepDot(const Vector2 *v1, const Vector2 *v2);
	static Vector2 Reflect(const Vector2 *v, const Vector2 *n);
	static Vector2 SmoothStep(const Vector2 *v1, const Vector2 *v2, float a);
	static Vector2 Subtract(const Vector2 *v1, const Vector2 *v2);
} Vect2;