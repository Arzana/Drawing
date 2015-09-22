#include <iostream>
#include "Map.h"

Map::Map()
	:m_Vertices(NULL), m_Sectors(NULL), m_Error("None")
{
	m_Count = 0;
}

Map::~Map()
{
	for (int i = 0; i < m_Count; i++)
	{
		m_Sectors[i].~Sector();
	}

	delete m_Sectors;
	delete m_Vertices;
	m_Count = 0;
}

Sector* Map::GetSector(int index)
{
	return &m_Sectors[index];
}

int Map::GetLength()
{
	return m_Count;
}

bool Map::Load(const char* loc)
{
	FILE* fp = fopen(loc, "r");			// Try open file with read only permission.

	if (!fp)
	{
		m_Error = "Could not open file.";
		return false;
	}

	char Buf[256], word[256], *ptr;		// Make a sentence, word and pointer buffer.
	Vector3 v;
	int n, m, vCount = 0;

	struct TempSector
	{
		float floor;	// Floor height
		float ceiling;	// Ceiling height
		int a;			// Index of Vector3 A
		int b;			// Index of Vector3 B
		int c;			// Index of Vector3 C
		int n1;			// Index of neighbor at AB
		int n2;			// Index of neighbor at BC
		int n3;			// Index of neighbor at CA

		TempSector() :floor(0), ceiling(20), a(-1), b(-1), c(-1), n1(-1), n2(-1), n3(-1) { }

	} *sectors = NULL;

	while (fgets(Buf, sizeof(Buf), fp))	// Read sentece or 256 characters.
	{
		switch (sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')				// Scan the buffer for the first word and length it took to get there.
		{																					// If the operation was succefull swith the first character, else none.
		case('V') :						// Vector3
			sscanf(ptr += n, "%f%n", &v.Z, &n);												// Scan the buffer for the Z coördinate.

			while (sscanf(ptr += n, "%f%n", &v.X, &n) == 1)									// Scan the buffer for remaining X coördinates.
			{
				m_Vertices = (Vector3*)realloc(m_Vertices, ++vCount * sizeof(*m_Vertices));	// ReAllocate the Vector3 array to hold a new Vector3.
				m_Vertices[vCount - 1] = v;
			}
			break;
		case('S') :						// Sector
			sectors = (TempSector*)realloc(sectors, ++m_Count * sizeof(*sectors));			// ReAllocate the sector array to hold a new sector.
			TempSector* sect = &sectors[m_Count - 1];
			int* num = NULL;

			sscanf(ptr += n, "%f%f%n", &sect->floor, &sect->ceiling, &n);					// Scan the buffer for the floor and ceiling height.

			for (m = 0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#';)		// Scan the buffer for the Vector3 positions and neighboring sectors.
			{																				// If the line starts with a # than its a comment line.
				num = (int*)realloc(num, ++m * sizeof(*num));
				num[m - 1] = word[0] == 'X' ? -1 : atoi(word);								// If word[0] is equal to X than that that line is a solid wall.
			}

			m *= 0.5f;
			for (n = 0; n < m; n++)
			{
				int index = num[m + n];

				if (n == 0) sect->n1 = index;
				else if (n == 1) sect->n2 = index;
				else if (n == 2) sect->n3 = index;
				else
				{
					m_Error = "To many neighbors in sector: " + (m_Count - 1);
					return false;
				}
			}

			for (n = 0; n < m; n++)
			{
				int index = num[n];

				if (n == 0) sect->a = index;
				else if (n == 1) sect->b = index;
				else if (n == 2) sect->c = index;
				else
				{
					m_Error = "To many vertices in sector: " + (m_Count - 1);
					return false;
				}
			}

			free(num);
			break;
		}
	}

	fclose(fp);

	m_Sectors = (Sector*)malloc(m_Count * sizeof(Sector));

	for (int i = 0; i < m_Count; i++)		// Initialize the sectors.
	{
		TempSector* sect = &sectors[i];

		if (sect->a == -1 || sect->b == -1 || sect->c == -1)
		{
			m_Error = "Vertices missing in sector: " + i;
			return false;
		}

		m_Vertices[sect->a].Y = sect->floor;
		m_Vertices[sect->b].Y = sect->floor;
		m_Vertices[sect->c].Y = sect->floor;

		m_Sectors[i] = Sector(&m_Vertices[sect->a], &m_Vertices[sect->b], &m_Vertices[sect->c], sect->floor, sect->ceiling);

		sect = NULL;
	}

	for (int i = 0; i < m_Count; i++)		// Set the neighbors of the sectors
	{
		TempSector* sect = &sectors[i];
		Sector* s = &m_Sectors[i];

		if (sect->n1 != -1) s->SetNeighbor(0, &m_Sectors[sect->n1]);
		if (sect->n2 != -1) s->SetNeighbor(1, &m_Sectors[sect->n2]);
		if (sect->n3 != -1) s->SetNeighbor(2, &m_Sectors[sect->n3]);

		sect = NULL;
		s = NULL;
	}

	free(sectors);

	return true;
}

const char* Map::GetError()
{
	return m_Error;
}