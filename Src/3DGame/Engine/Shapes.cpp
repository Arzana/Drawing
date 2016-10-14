#include "Shapes.h"

Vertex::Vertex(Vect3 v, Color c)
{
	this->v = v;
	this->c = c;
}

Rectangle::Rectangle(void)
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

Rectangle::Rectangle(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}