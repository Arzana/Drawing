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
	/*	Check is the triangles are visible to the screen and change or remove them is they are not.
	Returns the new length of the triangle pointer. */
	static int CheckVisibility(Triangle *triangles, int length);
	void SortVerticesByY();
private:
	static bool TriangleClip(Triangle& triangle);
	std::vector<Vector3*> GetVertices();
};

#endif