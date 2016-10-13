#pragma once

#include "Vect2.h"
#include "Color.h"

typedef struct Vertex
{
	Vect2 v;
	Color c;
} Vrtx;

typedef struct Line 
{
	Vertex v0, v1;
};

typedef struct Triangle
{
	Vertex v0, v1, v2;
} Trgl;