#pragma once

#include "Vect2.h"
#include "Color.h"

typedef struct Vertex
{
	Vect3 v;
	Color c;

	Vertex(Vect3 v, Color c);
	Vertex(float x, float y, float z, Color c);
} Vrtx;

struct Line 
{
	Vertex v0, v1;

	Line(Vertex v0, Vertex v1);
	Line(const Vertex *v0, const Vertex *v1);
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

#ifdef _USE_LINE_CLIP
typedef int OutCode;

#define INSIDE		0
#define LEFT		1
#define RIGHT		2
#define HORIZONTAL	3
#define BOTTOM		4
#define TOP			8
#define VERTICAL	12

OutCode ComputeOutCode(const Vect3 v, const Rect viewPort);
bool LineClip(Line *l, const Rect viewPort);
#endif

#ifdef _USE_GF_INTERNAL
void SortVerticesByY(Vertex *v0, Vertex *v1, Vertex *v2);
#endif