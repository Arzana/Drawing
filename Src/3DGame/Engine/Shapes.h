#pragma once

#include "Vect2.h"
#include "Color.h"

typedef struct Vertex
{
	Vect3 v;
	Color c;

	Vertex(void);
	Vertex(Vect3 v, Color c);
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z, Color c);

	inline bool operator ==(const Vertex &r) const { return v == r.v && c == r.c; }
	inline bool operator !=(const Vertex &r) const { return v != r.v || c != r.c; }
} Vrtx;

typedef struct Line 
{
	Vertex v0, v1;

	Line(Vertex v0, Vertex v1);
	Line(const Vertex *v0, const Vertex *v1);
} line;

typedef struct Triangle
{
	Vertex v0, v1, v2;

	Triangle(Vertex v0, Vertex v1, Vertex v2);
	Triangle(const Vertex *v0, const Vertex *v1, const Vertex *v2);
	Line GetLine(int l) const;
	bool IsInside(Vertex *v);
} Trgl;

typedef struct Rectangle
{
	int x, y, w, h;

	Rectangle(void);
	Rectangle(int x, int y, int w, int h);
} Rect;

typedef struct Polygon
{
	Vertex *v;
	int polyVLen;

	Polygon(void);
} Poly;

typedef struct ViewPort
{
	Rect screen;
	float far, near;

	ViewPort(Rect screen, float far, float near);
	ViewPort(int x, int y, int w, int h, float f, float n);
} ViewPort;

#ifdef _USE_CLIPPING
#define INSIDE		0b000000
#define LEFT		0b000001
#define RIGHT		0b000010
#define HORIZONTAL	0b000011
#define BOTTOM		0b000100
#define TOP			0b001000
#define VERTICAL	0b001100
#define NEAR		0b010000
#define FAR			0b100000
#define DEPTH		0b110000
#define OUTSIDE		0b111111

#include <vector>

int ComputeMask(const Vect3 v, const ViewPort viewPort);
bool LineClip(Line *l, const ViewPort viewPort);
Vertex* TriangleClip(Triangle *t, int *len, const ViewPort viewPort);
void SortVerticesBySpecial(Vertex * v0, Vertex * v1, Vertex * v2, const ViewPort vp);
#endif

#ifdef _USE_GF_INTERNAL
void SortVerticesByY(Vertex *v0, Vertex *v1, Vertex *v2);
#endif