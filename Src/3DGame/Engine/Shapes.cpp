#define _USE_LINE_CLIP

#include "Shapes.h"

Vertex::Vertex(Vect3 v, Color c)
	: v(v), c(c)
{ }

Vertex::Vertex(float x, float y, float z, Color c)
	: v(Vect3(x, y, z)), c(c)
{ }

Line::Line(Vertex v0, Vertex v1)
	: v0(v0), v1(v1)
{ }

Rectangle::Rectangle(void)
	: x(0), y(0), w(0), h(0)
{ }

Rectangle::Rectangle(int x, int y, int w, int h)
	: x(x), y(y), w(w), h(h)
{ }

OutCode ComputeOutCode(const Vect3 v, const Rect vp)
{
	OutCode code = INSIDE;

	if (v.X < vp.x) code |= LEFT;
	else if (v.X > vp.w) code |= RIGHT;
	if (v.Y < vp.y) code |= BOTTOM;
	if (v.Y > vp.h) code |= TOP;

	return code;
}

bool LineClip(Line * l, const Rect vp)
{
	OutCode oc0 = ComputeOutCode(l->v0.v, vp);
	OutCode oc1 = ComputeOutCode(l->v1.v, vp);
	bool result = false;

	while (true)
	{
		if (!(oc0 | oc1))
		{
			result = true;
			break;
		}
		else if (oc0 & oc1) break;

		float x, y, z;
		OutCode occ = oc0 ? oc0 : oc1;

		if (occ & TOP)
		{
			x = l->v0.v.X + (l->v1.v.X - l->v0.v.X) * (vp.h - l->v0.v.Y) / (l->v1.v.Y - l->v0.v.Y);
			y = vp.h - 1;
		}
		else if (occ & BOTTOM)
		{
			x = l->v0.v.X + (l->v1.v.X - l->v0.v.X) * (vp.y - l->v0.v.Y) / (l->v1.v.Y - l->v0.v.Y);
			y = vp.y;
		}
		else if (occ & RIGHT)
		{
			y = l->v0.v.Y + (l->v1.v.Y - l->v0.v.Y) * (vp.w - l->v0.v.X) / (l->v1.v.X - l->v0.v.X);
			x = vp.w - 1;
		}
		else if (occ & LEFT)
		{
			y = l->v0.v.Y + (l->v1.v.Y - l->v0.v.Y) * (vp.x - l->v0.v.X) / (l->v1.v.X - l->v0.v.X);
			x = vp.x;
		}

		if (occ & VERTICAL) z = lerp(l->v0.v.Z, l->v1.v.Z, invLerp(l->v0.v.Y, l->v1.v.Y, y));
		if (occ & HORIZONTAL) z = lerp(l->v0.v.Z, l->v1.v.Z, invLerp(l->v0.v.X, l->v1.v.X, x));

		if (occ == oc0)
		{
			l->v0.v.X = x;
			l->v0.v.Y = y;
			l->v0.v.Z = z;
			oc0 = ComputeOutCode(l->v0.v, vp);
		}
		else
		{
			l->v1.v.X = x;
			l->v1.v.Y = y;
			l->v1.v.Z = z;
			oc1 = ComputeOutCode(l->v1.v, vp);
		}
	}

	return result;
}

template <typename T>
void Swap(T *v0, T *v1)
{
	T temp = *v0;
	*v0 = *v1;
	*v1 = temp;
}

void SortVerticesByY(Vertex * v0, Vertex * v1, Vertex * v2)
{
	if (v0->v.Y > v1->v.Y) Swap(v0, v1);
	if (v0->v.Y > v2->v.Y) Swap(v0, v2);
	if (v1->v.Y > v2->v.Y) Swap(v1, v2);
}
