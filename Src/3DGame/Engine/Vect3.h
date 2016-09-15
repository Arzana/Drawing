#pragma once

#define VECT3_ONE		Vector3(1)
#define VECT3_RIGHT		Vector3(1, 0, 0)
#define VECT3_LEFT		Vector3(-1, 0, 0)
#define VECT3_UP		Vector3(0, 1, 0)
#define VECT3_DOWN		Vector3(0, -1, 0)
#define VECT3_BACK		Vector3(0, 0, 1)
#define VECT3_FORWARD	Vector3(0, 0, -1)
#define VECT3_ZERO		Vector3()
#define VECT3_NEGATIVE	Vector3(-1)

typedef struct Vector3
{
public:
	float X, Y, Z;

	Vector3(void);
	Vector3(float v);
	Vector3(float x, float y, float z);
} Vect3;