#include "Sector.h"
#include <iostream>

#define FIND(mA, mB, mC, v)		(mA == v ? 1 : (mB == v ? 2 : (mC == v ? 3 : 0)))
#define AB(a, b)				((a == 1 || b == 1) && (a == 2 || b == 2))
#define BC(a, b)				((a == 2 || b == 2) && (a == 3 || b == 3))

Sector::Sector(const Vector3* vA, const Vector3* vB, const Vector3* vC, float f, float c)
	:m_A(vA), m_B(vB), m_C(vC)
{
	m_Floor = f;
	m_Ceiling = c;
	m_AB.Init(m_A, m_B);
	m_BC.Init(m_B, m_C);
	m_CA.Init(m_C, m_A);
}

Sector::~Sector()
{
	m_AB.~Line();
	m_BC.~Line();
	m_CA.~Line();
}

float Sector::GetCeiling()
{
	return m_Ceiling;
}

float Sector::GetFloor()
{
	return m_Floor;
}

Sector* Sector::GetNeighbor(const Vector3* a, const Vector3* b)
{
	int vA = FIND(m_A, m_B, m_C, a);
	int vB = FIND(m_A, m_B, m_C, b);

	if (vA == 0 || vB == 0 || vA == vB) return NULL;

	if (AB(vA, vB)) return m_AB.Neighbor;
	if (BC(vA, vB)) return m_BC.Neighbor;
	
	return m_CA.Neighbor;
}

const Vector3** Sector::GetVertices()
{
	const Vector3** result = new const Vector3*[3];
	result[0] = m_A;
	result[1] = m_B;
	result[2] = m_C;

	return result;
}

void Sector::DeleteConnection(const Vector3* a, const Vector3* b)
{
	int vA = FIND(m_A, m_B, m_C, a);
	int vB = FIND(m_A, m_B, m_C, b);

	if (vA == 0 || vB == 0 || vA == vB) return;

	if (AB(vA, vB))
	{
		m_AB.IsWall = true;
		m_AB.Neighbor = NULL;
	}
	else if (BC(vA, vB))
	{
		m_BC.IsWall = true;
		m_BC.Neighbor = NULL;
	}
	else
	{
		m_CA.IsWall = true;
		m_CA.Neighbor = NULL;
	}
}

bool Sector::SetNeighbor(int l, Sector* s)
{
	if (l == 0 && !m_AB.Neighbor)
	{
		m_AB.IsWall = false;
		m_AB.Neighbor = s;

		return true;
	}
	else if (l == 1 && !m_BC.Neighbor)
	{
		m_BC.IsWall = false;
		m_BC.Neighbor = s;

		return true;
	}
	else if (l == 2 && !m_CA.Neighbor)
	{
		m_CA.IsWall = false;
		m_CA.Neighbor = s;

		return true;
	}

	return false;
}

Sector::Line::Line()
{
	A = NULL;
	B = NULL;
	IsWall = true;
	Neighbor = NULL;
}

void Sector::Line::Init(const Vector3* a, const Vector3* b)
{
	A = a;
	B = b;
}

Sector::Line::~Line()
{
	A = NULL, B = NULL;

	if (Neighbor)
	{
		Neighbor->DeleteConnection(A, B);
		Neighbor = NULL;
	}
}