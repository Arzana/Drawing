#include "Triangle.h"
#include <GF.h>

void RenderTriangle(const int type)
{
	if (type == TRIANGLE_POINTS) GF_StartRender(GF_POINTS);
	else if (type == TRIANGLE_LINES) GF_StartRender(GF_LINE_LOOP);
	else if (type == TRIANGLE_TRIANGLES) GF_StartRender(GF_TRIANGLES);
	else return;

	GF_SetBufferLength(3);

	GF_AddPoint(triangleModel[0]);
	GF_AddPoint(triangleModel[1]);
	GF_AddPoint(triangleModel[2]);

	GF_EndRender();
}