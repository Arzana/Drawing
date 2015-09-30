#include "Triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
{
	A = a;
	B = b;
	C = c;
}

int Triangle::CheckVisibility(Triangle *triangles, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (!TriangleClip(triangles[i]))									// The triangle is not visible.
		{
			if (--length == 0) return 0;

			triangles[i] = triangles[length];
			memmove(triangles, triangles, length * sizeof(triangles));		// Shift the array 1 position to the left.

			continue;
		}

		i++;
	}

	return length;
}

bool Triangle::TriangleClip(Triangle& triangle)
{
	std::vector<Vector3*> vertices = triangle.GetVertices();
	int failIndex = 0;

	for (int j = 0; j < vertices.size(); j++)
	{
		Line edge = Line(*vertices[j], *vertices[(j + 1) % vertices.size()]);
		if (edge.Get2DLength() == 0) return false;

		bool visible = Line::CohenSutherlandLineClip(edge);

		*vertices[j] = edge.A;
		*vertices[(j + 1) % vertices.size()] = edge.B;

		if (!visible)
		{
			failIndex++;
			if (failIndex > 1) return false;	// 2 edges have already been deemed hidden wich meens all 3 vertices are hidden.
		}
	}

	return true;
}

std::vector<Vector3*> Triangle::GetVertices()
{
	std::vector<Vector3*> vertices = std::vector<Vector3*>();
	vertices.push_back(&A);
	vertices.push_back(&B);
	vertices.push_back(&C);

	return vertices;
}

void Triangle::SortVerticesByY()
{
	Vector3 high = A.Y < B.Y ? (A.Y < C.Y ? A : (B.Y < C.Y ? B : C)) : (B.Y < C.Y ? B : (C.Y < A.Y ? C : A));
	Vector3 low = A.Y > B.Y ? (A.Y > C.Y ? A : (B.Y > C.Y ? B : C)) : (B.Y > C.Y ? B : (C.Y > A.Y ? C : A));
	Vector3 mid;

	if (high == A)
	{
		if (low == B) mid = C;
		else mid = B;
	}
	else if (high == B)
	{
		if (low == A) mid = C;
		else mid = A;
	}
	else
	{
		if (low == A) mid = B;
		else mid = A;
	}

	A = high;
	B = mid;
	C = low;
}