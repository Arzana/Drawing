#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Matrix.h"
#include "Line.h"

struct Triangle
{
public:
	Vector3 A;
	Vector3 B;
	Vector3 C;

	Triangle(const Vector3& a, const Vector3& b, const Vector3& c);
	static int CheckVisibility(Triangle *triangles, int length);
};

#endif