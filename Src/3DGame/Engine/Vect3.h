#pragma once

#define VECT3_ONE		Vector3(1)
#define VECT3_RIGHT		Vector3(1, 0, 0)
#define VECT3_LEFT		Vector3(-1, 0, 0)
#define VECT3_UP		Vector3(0, 1, 0)
#define VECT3_DOWN		Vector3(0, -1, 0)
#define VECT3_BACK		Vector3(0, 0, -1)
#define VECT3_FORWARD	Vector3(0, 0, 1)
#define VECT3_ZERO		Vector3()
#define VECT3_NEGATIVE	Vector3(-1)

typedef struct Vector3
{
	float X, Y, Z;

	Vector3(void);
	Vector3(float v);
	Vector3(float x, float y, float z);

	Vector3 operator -(void) const;
	Vector3 operator -(const Vector3 &r) const;
	Vector3 operator +(const Vector3 &r) const;
	Vector3 operator /(float r) const;
	Vector3 operator /(const Vector3 &r) const;
	Vector3 operator *(float r) const;
	Vector3 operator *(const Vector3 &r) const;

	Vector3 operator -=(const Vector3 &r);
	Vector3 operator +=(const Vector3 &r);
	Vector3 operator /=(float r);
	Vector3 operator /=(const Vector3 &r);
	Vector3 operator *=(float r);
	Vector3 operator *=(const Vector3 &r);

	bool operator ==(const Vector3 &r) const;
	bool operator !=(const Vector3 &r) const;

	static Vector3 Abs(const Vector3 *v);
	void Abs(void);
	static Vector3 Add(const Vector3 *v1, const Vector3 *v2);
	float Area(void) const;
	static Vector3 Barycentric(const Vector3 *v1, const Vector3 *v2, const Vector3 *v3, float b2, float b3);
	static Vector3 CatmullRom(const Vector3 *v1, const Vector3 *v2, const Vector3 *v3, const Vector3 *v4, float a);
	static Vector3 Clamp(const Vector3 *min, const Vector3 *max, const Vector3 *v);
	void Clamp(const Vector3 *min, const Vector3 *max);
	static Vector3 Cross(const Vector3 *v1, const Vector3 *v2);
	static float Distance(const Vector3 *v1, const Vector3 *v2);
	static float DistanceSquared(const Vector3 *v1, const Vector3 *v2);
	static Vector3 Divide(const Vector3 *v1, float v2);
	static Vector3 Divide(const Vector3 *v1, const Vector3 *v2);
	static float Dot(const Vector3 *v1, const Vector3 *v2);
	static bool Equals(const Vector3 *v1, const Vector3 *v2);
	static Vector3 Hermite(const Vector3 *v1, const Vector3 *t1, const Vector3 *v2, const Vector3 *t2, float a);
	float Length(void) const;
	float LengthSquared(void) const;
	static Vector3 Lerp(const Vector3 *min, const Vector3 *max, float a);
	static Vector3 Lerp(const Vector3 *min, const Vector3 *max, const Vector3 *a);
	static Vector3 InvLerp(const Vector3 *min, const Vector3 *max, float v);
	static Vector3 InvLerp(const Vector3 *min, const Vector3 *max, const Vector3 *v);
	static Vector3 Max(const Vector3 *v1, const Vector3 *v2);
	static Vector3 Min(const Vector3 *v1, const Vector3 *v2);
	static Vector3 Multiply(const Vector3 *v1, float v2);
	static Vector3 Multiply(const Vector3 *v1, const Vector3 *v2);
	static Vector3 Negate(const Vector3 *v);
	void Normalize(void);
	static Vector3 Normalize(const Vector3 *v);
	static Vector3 Reflect(const Vector3 *v, const Vector3 *n);
	static Vector3 SmoothStep(const Vector3 *v1, const Vector3 *v2, float a);
	static Vector3 Subtract(const Vector3 *v1, const Vector3 *v2);
	float Volume(void) const;
} Vect3;