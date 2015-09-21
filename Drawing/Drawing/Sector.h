#ifndef SECTOR_H
#define SECTOR_H

#include "Vector3.h"

/* A sector is a triangular space in the map defining walls or portals to other sectors. */
class Sector
{
public:
	Sector(const Vector3* a, const Vector3* b, const Vector3* c, float floor, float ceiling);
	~Sector();
	/* Get all vertices accociated with this sector. */
	const Vector3** GetVertices();
	float GetFloor();
	float GetCeiling();
	/* Try get the neighboring sector at the line between a and b (returns NULL if the vertices are invalid or no neighbor is found). */
	Sector* GetNeighbor(const Vector3* a, const Vector3* b);
	/* Try set the neighbor at the specified line (returns false is the vertices are invalid or the neightbor has already been set). */
	bool SetNeighbor(int line, Sector* sector);

private:
	const Vector3* m_A;
	const Vector3* m_B;
	const Vector3* m_C;
	float m_Floor, m_Ceiling;
	/* Nullifies the sector pointer at the specified line. */
	void DeleteConnection(const Vector3* a, const Vector3* b);

	struct Line
	{
		const Vector3* A;
		const Vector3* B;
		bool IsWall;
		Sector* Neighbor;

	public:
		Line();
		void Init(const Vector3* a, const Vector3* b);
		~Line();
	} m_AB, m_BC, m_CA;
};

#endif