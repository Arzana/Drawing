#include "Triangle.h"

Triangle::Triangle(const Vector2& a, const Vector2& b, const Vector2& c)
{
	A = a;
	B = b;
	C = c;
}

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
{
	A = Vector2(a.X, a.Y);
	B = Vector2(b.X, b.Y);
	C = Vector2(c.X, c.Y);
}

int Triangle::CheckVisibility(Vector4 *s, int length)
{
	int result = length;

	for (int i = 0; i < length; i++)
	{

	}

	return result;
}

std::vector<Vector2*> Triangle::GetVertices()
{
	std::vector<Vector2*> vertices = std::vector<Vector2*>();
	vertices.push_back(&A);
	vertices.push_back(&B);
	vertices.push_back(&C);

	return vertices;
}

void Triangle::SortVerticesByY()
{
	Vector2 high = A.Y < B.Y ? (A.Y < C.Y ? A : (B.Y < C.Y ? B : C)) : (B.Y < C.Y ? B : (C.Y < A.Y ? C : A));
	Vector2 low = A.Y > B.Y ? (A.Y > C.Y ? A : (B.Y > C.Y ? B : C)) : (B.Y > C.Y ? B : (C.Y > A.Y ? C : A));
	Vector2 mid;

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