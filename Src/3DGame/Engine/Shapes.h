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

typedef struct Line 
{
	Vertex v0, v1;

	Line(Vertex v0, Vertex v1);
	Line(const Vertex *v0, const Vertex *v1);
} Line;

typedef struct Triangle
{
	Vertex v0, v1, v2;

	Triangle(Vertex v0, Vertex v1, Vertex v2);
	Triangle(const Vertex *v0, const Vertex *v1, const Vertex *v2);
} Trgl;

typedef struct Rectangle
{
	int x, y, w, h;

	Rectangle(void);
	Rectangle(int x, int y, int w, int h);
} Rect;

typedef struct ViewPort
{
	Rectangle screen;
	float far, near;

	ViewPort(Rectangle screen, float far, float near);
	ViewPort(int x, int y, int w, int h, float f, float n);
} ViewPort;

#ifdef _USE_CLIPPING
#define INSIDE		0
#define LEFT		1
#define RIGHT		2
#define HORIZONTAL	3
#define BOTTOM		4
#define TOP			8
#define VERTICAL	12
#define NEAR		16
#define FAR			32
#define DEPTH		48

int ComputeMask(const Vect3 v, const ViewPort viewPort);
bool LineClip(Line *l, const ViewPort viewPort);
bool TriangleClip(Triangle *t, int *len);
#endif

#ifdef _USE_GF_INTERNAL
void SortVerticesByY(Vertex *v0, Vertex *v1, Vertex *v2);
#endif