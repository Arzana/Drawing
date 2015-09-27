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
		bool visible = Line::CohenSutherlandLineClip(edge);

		*vertices[j] = edge.A;
		*vertices[(j + 1) % vertices.size()] = edge.B;

		if (!visible)
		{
			failIndex++;
			if (failIndex > 1) return false;
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