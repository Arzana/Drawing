#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Vector2.h"
#include "Line.h"

struct Triangle
{
public:
	Vector2 A;
	Vector2 B;
	Vector2 C;

	Triangle(const Vector2& a, const Vector2& b, const Vector2& c);
	Triangle(const Vector3& a, const Vector3& b, const Vector3& c);
	/*	Check is the triangles are visible to the screen and change or remove them is they are not.
	Returns the new length of the triangle pointer. */
	static int CheckVisibility(Vector4 *vertices, int length);
	void SortVerticesByY();
private:
	std::vector<Vector2*> GetVertices();
};

#endif