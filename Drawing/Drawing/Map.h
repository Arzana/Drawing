#ifndef MAP_H
#define MAP_H

#include "Sector.h"

using namespace std;

class Map
{
public:
	Map();
	~Map();
	/* Load a map from a specified location. */
	bool Load(const char* location);
	/* Get the error thrown by Load. */
	const char* GetError();
	Sector* GetSector(int index);
	/* Get the sector count. */
	int GetLength();

private:
	Sector* m_Sectors;
	Vector3* m_Vertices;
	int m_Count;
	const char* m_Error;
};

#endif