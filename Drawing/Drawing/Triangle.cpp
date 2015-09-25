#include "Triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
{
	A = a;
	B = b;
	C = c;
}

int Triangle::CheckVisibility(Triangle *triangles, int length)
{
	//https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
}