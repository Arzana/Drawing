#pragma once

#include "Vect2.h"
#include "Color.h"

#undef far
#undef near

typedef struct Vertex
{
	vect3 v;
	Color c;

	Vertex(void);
	Vertex(vect3 v, Color c);
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z, Color c);

	inline bool operator ==(const Vertex &r) const { return v == r.v && c == r.c; }
	inline bool operator !=(const Vertex &r) const { return v != r.v || c != r.c; }
} vrtx;

typedef struct Line 
{
	vrtx v0, v1;

	Line(vrtx v0, vrtx v1);
	Line(const vrtx *v0, const vrtx *v1);
} line;

typedef struct Triangle
{
	vrtx v0, v1, v2;

	Triangle(vrtx v0, vrtx v1, vrtx v2);
	Triangle(const vrtx *v0, const vrtx *v1, const vrtx *v2);
	line GetLine(int l) const;
	bool IsInside(vrtx *v);
} trgl;

typedef struct Rectangle
{
	int x, y, w, h;

	Rectangle(void);
	Rectangle(int x, int y, int w, int h);
} Rect;

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

int ComputeMask(const vect3 v, const ViewPort viewPort);
bool LineClip(line *l, const ViewPort viewPort);
vrtx* TriangleClip(trgl *t, int *len, const ViewPort viewPort);
void SortVerticesBySpecial(vrtx *v0, vrtx *v1, vrtx *v2, const ViewPort vp);
#endif

#ifdef _USE_GF_INTERNAL
void SortVerticesByY(vrtx *v0, vrtx *v1, vrtx *v2);
#endif