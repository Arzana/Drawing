#pragma once

#define VECT4_ONE		Vector4(1)
#define VECT4_UNITX		Vector4(1, 0, 0, 0)
#define VECT4_UNITY		Vector4(0, 1, 0, 0)
#define VECT4_UNITZ		Vector4(0, 0, 1, 0)
#define VECT4_UNITW		Vector4(0, 0, 0, 1)
#define VECT4_ZERO		Vector4()
#define VECT4_NEGATIVE	Vector4(-1)

typedef struct Vector4
{
	float X, Y, Z, W;

	Vector4(void);
	Vector4(float v);
	Vector4(float x, float y, float z, float w);

	Vector4 operator -(void) const;
	Vector4 operator -(const Vector4 &r) const;
	Vector4 operator +(const Vector4 &r) const;
	Vector4 operator /(float r) const;
	Vector4 operator /(const Vector4 &r) const;
	Vector4 operator *(float r) const;
	Vector4 operator *(const Vector4 &r) const;

	Vector4 operator -=(const Vector4 &r);
	Vector4 operator +=(const Vector4 &r);
	Vector4 operator /=(float r);
	Vector4 operator /=(const Vector4 &r);
	Vector4 operator *=(float r);
	Vector4 operator *=(const Vector4 &r);

	bool operator ==(const Vector4 &r) const;
	bool operator !=(const Vector4 &r) const;
} Vect4;