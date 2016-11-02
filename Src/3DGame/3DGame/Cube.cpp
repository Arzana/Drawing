#include "Cube.h"
#include <GF.h>

const size_t vectLen = 8;
const size_t lineLen = 12;
const size_t trglLen = 12;

const int lines[lineLen][2] =
{
	{ 0, 1 },
	{ 0, 5 },
	{ 0, 7 },
	{ 1, 2 },
	{ 1, 6 },
	{ 2, 3 },
	{ 2, 7 },
	{ 3, 4 },
	{ 3, 6 },
	{ 4, 5 },
	{ 4, 7 },
	{ 5, 6 }
};

const int trgls[trglLen][3] =
{
	{ 0, 1, 2 },
	{ 0, 2, 7 },
	{ 0, 1, 6 },
	{ 0, 5, 6 },
	{ 0, 4, 5 },
	{ 0, 4, 7 },
	{ 3, 1, 2 },
	{ 3, 1, 6 },
	{ 3, 2, 7 },
	{ 3, 4, 7 },
	{ 3, 4, 5 },
	{ 3, 5, 6 }
};

void RenderCube(const int type)
{
	if (type == CUBE_POINTS)
	{
		GF_StartRender(GF_POINTS);
		GF_SetBufferLength(vectLen);

		for (size_t i = 0; i < vectLen; i++)
		{
			GF_AddPoint(cubeModel[i]);
		}

		GF_EndRender();
	}
	else if (type == CUBE_LINES)
	{
		GF_StartRender(GF_LINES);
		GF_SetBufferLength(lineLen << 1);

		for (size_t i = 0; i < lineLen; i++)
		{
			GF_AddPoint(cubeModel[lines[i][0]]);
			GF_AddPoint(cubeModel[lines[i][1]]);
		}

		GF_EndRender();
	}
	else if (type == CUBE_TRIANGLES)
	{
		GF_StartRender(GF_TRIANGLES);
		GF_SetBufferLength(trglLen * 3);

		for (size_t i = 0; i < trglLen; i++)
		{
			GF_AddPoint(cubeModel[trgls[i][0]]);
			GF_AddPoint(cubeModel[trgls[i][1]]);
			GF_AddPoint(cubeModel[trgls[i][2]]);
		}

		GF_EndRender();
	}
}
