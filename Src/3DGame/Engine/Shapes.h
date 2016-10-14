#pragma once

#include "Vect2.h"
#include "Color.h"

typedef struct Vertex
{
	Vect3 v;
	Color c;

	Vertex(Vect3 v, Color c);
} Vrtx;

struct Line 
{
	Vertex v0, v1;
};

typedef struct Triangle
{
	Vertex v0, v1, v2;
} Trgl;

typedef struct Rectangle
{
	int x, y, w, h;

	Rectangle(void);
	Rectangle(int x, int y, int w, int h);
} Rect;