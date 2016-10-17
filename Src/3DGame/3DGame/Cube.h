#pragma once
#include <Shapes.h>

const size_t cubeModelVertices = 8;
const size_t cubeModelIndices = 12;

const Vertex cubeModel[cubeModelVertices] =
{
	Vertex(1, 1, 1, CLR_BLACK),
	Vertex(1, 1, -1, CLR_RED),
	Vertex(1, -1, -1, CLR_GREEN),
	Vertex(-1, -1, -1, CLR_BLUE),
	Vertex(-1, -1, 1, CLR_CYAN),
	Vertex(-1, 1, 1, CLR_MAGENTA),
	Vertex(-1, 1, -1, CLR_YELLOW),
	Vertex(1, -1, 1, CLR_WHITE)
};

const int indices[cubeModelIndices][2] = 
{
	{5, 4},
	{5, 0},
	{4, 7},
	{7, 0},
	{6, 5},
	{1, 0},
	{3, 4},
	{2, 7},
	{6, 3},
	{6, 1},
	{3, 2},
	{2, 1}
};

void RenderCube(void);