#include "Cube.h"
#include <GF.h>

void RenderCube(void)
{
	GF_StartRender(GF_LINES);
	GF_SetBufferLength(cubeModelIndices << 1);

	for (size_t i = 0; i < cubeModelIndices; i++)
	{
		GF_AddPoint(cubeModel[indices[i][0]]);
		GF_AddPoint(cubeModel[indices[i][1]]);
	}

	GF_EndRender();
}
