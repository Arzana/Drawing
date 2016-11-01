#pragma once
#include <Shapes.h>

#define CUBE_POINTS		0
#define CUBE_LINES		1
#define CUBE_TRIANGLES	2

const Vertex cubeModel[8] =
{
	Vertex(1, 1, 1, CLR_BLACK),		//0
	Vertex(1, 1, -1, CLR_RED),		//1
	Vertex(1, -1, -1, CLR_GREEN),	//2
	Vertex(-1, -1, -1, CLR_BLUE),	//3
	Vertex(-1, -1, 1, CLR_CYAN),	//4
	Vertex(-1, 1, 1, CLR_MAGENTA),	//5
	Vertex(-1, 1, -1, CLR_YELLOW),	//6
	Vertex(1, -1, 1, CLR_WHITE)		//7
};

void RenderCube(const int type);