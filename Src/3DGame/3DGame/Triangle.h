#pragma once

#include <Shapes.h>

#define TRIANGLE_POINTS		0
#define TRIANGLE_LINES		1
#define TRIANGLE_TRIANGLES	2

const Vertex triangleModel[3]
{
	Vertex(0.0f, -0.5f, 1.0f, CLR_RED),
	Vertex(-0.5f, 0.5f, 1.0f, CLR_GREEN),
	Vertex(0.5f, 0.5f, 1.0f, CLR_BLUE)
};

void RenderTriangle(const int type);