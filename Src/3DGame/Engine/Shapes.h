#pragma once

#include "Triangle.h"
#include "Rectangle.h"

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