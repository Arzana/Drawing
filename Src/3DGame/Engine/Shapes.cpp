#define _USE_CLIPPING
#define _VECT_CONV

#define vect(vrtx)		p->vrtx.v
#define clr(vrtx)		p->vrtx.c
#define vx(vrtx)		vect(vrtx).X
#define vy(vrtx)		vect(vrtx).Y
#define vz(vrtx)		vect(vrtx).Z
#define SORT_ARGS		(const Vertex *v0, const Vertex *v1)

#include <vector>
#include "Utils.h"
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

Line::Line(const Vertex * v0, const Vertex * v1)
	: v0(*v0), v1(*v1)
{ }

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
	: v0(v0), v1(v1), v2(v2)
{ }

Triangle::Triangle(const Vertex * v0, const Vertex * v1, const Vertex * v2)
	: v0(*v0), v1(*v1), v2(*v2)
{ }

Line Triangle::GetLine(int l) const
{
	l = clamp(0, 2, l);
	if (l == 0) return Line(v0, v1);
	if (l == 1) return Line(v1, v2);
	return Line(v2, v0);
}

bool Triangle::IsInside(const Vect3 v)
{
	Vect2 *vs1 = &V3ToV2(&(v1.v - v0.v));
	Vect2 *vs2 = &V3ToV2(&(v2.v - v0.v));
	Vect2 *q = &V3ToV2(&(v - v0.v));

	float r = Vect2::PrepDot(vs1, vs2);
	float s = Vect2::PrepDot(q, vs2) / r;
	float t = Vect2::PrepDot(vs1, q) / r;

	return s >= 0 && t >= 0 && s + t <= 1;
}

Rectangle::Rectangle(void)
	: x(0), y(0), w(0), h(0)
{ }

Rectangle::Rectangle(int x, int y, int w, int h)
	: x(x), y(y), w(w), h(h)
{ }

ViewPort::ViewPort(Rectangle screen, float far, float near)
	: screen(screen), far(far), near(near)
{ }

ViewPort::ViewPort(int x, int y, int w, int h, float f, float n)
	: screen(x, y, w, h), far(f), near(n)
{ }

int ComputeMask(const Vect3 v, const ViewPort vp)
{
	int code = INSIDE;

	if (v.X < vp.screen.x) code |= LEFT;
	else if (v.X > vp.screen.w) code |= RIGHT;
	if (v.Y < vp.screen.y) code |= BOTTOM;
	else if (v.Y > vp.screen.h) code |= TOP;
	if (v.Z < vp.near) code |= NEAR;
	else if (v.Z > vp.far) code |= FAR;

	return code;
}

bool LineClip(Line * p, const ViewPort vp)
{
	int m0 = ComputeMask(vect(v0), vp);
	int m1 = ComputeMask(vect(v1), vp);

	while (m0 | m1)
	{
		if (m0 & m1) return false;

		float x, y, z, a;
		int mc = m0 ? m0 : m1;

		if (mc & TOP) y = vp.screen.h;
		else if (mc & BOTTOM) y = vp.screen.y;
		else if (mc & RIGHT) x = vp.screen.w;
		else if (mc & LEFT) x = vp.screen.x;
		else if (mc & NEAR) z = vp.near;
		else if (mc & FAR) z = vp.far;

		if (mc & VERTICAL)
		{
			a = invLerp(vy(v0), vy(v1), y);
			z = lerp(vz(v0), vz(v1), a);
			x = lerp(vx(v0), vx(v1), a);
		}
		else if (mc & HORIZONTAL)
		{
			a = invLerp(vx(v0), vx(v1), x);
			z = lerp(vz(v0), vz(v1), a);
			y = lerp(vy(v0), vy(v1), a);
		}
		else if (mc & DEPTH)
		{
			a = invLerp(vz(v0), vz(v1), z);
			x = lerp(vx(v0), vx(v1), a);
			y = lerp(vy(v0), vy(v1), a);
		}

		if (mc == m0)
		{
			vect(v0) = Vect3(x, y, z);
			clr(v0) = Color::Lerp(clr(v0), clr(v1), a);
			m0 = ComputeMask(vect(v0), vp);
		}
		else
		{
			vect(v1) = Vect3(x, y, z);
			clr(v1) = Color::Lerp(clr(v0), clr(v1), a);
			m1 = ComputeMask(vect(v1), vp);
		}
	}

	return true;
}

size_t GetNextPoint(std::vector<Vertex> * l, int *dir)
{
	if (l->size() <= 1) return 0;
	size_t r = 0;

	if (*dir == 0)
	{
		Sort<Vertex>(l, []SORT_ARGS { return v0->v.Y < v1->v.Y; });
		Vertex p = l->at(0);
		bool rightMost = true;

		for (size_t i = 1; i < l->size(); i++)
		{
			Vertex c = l->at(i);
			if (c.v.Y > p.v.Y) break;

			rightMost = false;
			if (c.v.X < p.v.X)
			{
				p = c;
				r = i;
			}
		}

		if (rightMost) ++(*dir);
	}
	else if (*dir == 1)
	{
		Sort<Vertex>(l, []SORT_ARGS { return v0->v.X > v1->v.X; });
		Vertex p = l->at(0);
		bool downMost = true;

		for (size_t i = 1; i < l->size(); i++)
		{
			Vertex c = l->at(i);
			if (c.v.X < p.v.X) break;

			downMost = false;
			if (c.v.Y < p.v.Y)
			{
				p = c;
				r = i;
			}
		}

		if (downMost) ++(*dir);
	}
	else if (*dir == 2)
	{
		Sort<Vertex>(l, []SORT_ARGS { return v0->v.Y > v1->v.Y; });
		Vertex p = l->at(0);
		bool leftMost = true;

		for (size_t i = 1; i < l->size(); i++)
		{
			Vertex c = l->at(i);
			if (c.v.Y < p.v.Y) break;

			leftMost = false;
			if (c.v.X > p.v.X)
			{
				p = c;
				r = i;
			}
		}

		if (leftMost) ++(*dir);
	}
	else if (*dir == 3)
	{
		Sort<Vertex>(l, []SORT_ARGS { return v0->v.X < v1->v.X; });
		Vertex p = l->at(0);

		for (size_t i = 1; i < l->size(); i++)
		{
			Vertex c = l->at(i);
			if (c.v.Y > p.v.Y)
			{
				p = c;
				r = i;
			}
		}
	}

	return r;
}

Vertex* TriangleClip(Triangle * p, int * len, const ViewPort vp)
{
	std::vector<Vertex> temp;

	Vect3 c;
	if (p->IsInside(c = Vect3(vp.screen.x, vp.screen.y, vp.near))) temp.push_back(Vertex(c, CLR_BLACK));
	if (p->IsInside(c = Vect3(vp.screen.x, vp.screen.h, vp.near))) temp.push_back(Vertex(c, CLR_BLACK));
	if (p->IsInside(c = Vect3(vp.screen.w, vp.screen.h, vp.near))) temp.push_back(Vertex(c, CLR_BLACK));
	if (p->IsInside(c = Vect3(vp.screen.w, vp.screen.y, vp.near))) temp.push_back(Vertex(c, CLR_BLACK));

	for (size_t i = 0; i < 3; i++)
	{
		Line *l = &p->GetLine(i);
		if (LineClip(l, vp))
		{
			if (!Contains(temp, &l->v0)) temp.push_back(l->v0);
			if (!Contains(temp, &l->v1)) temp.push_back(l->v1);
		}
	}

	if (!(*len = temp.size())) return NULL;

	Vertex *poly = malloc_s(Vertex, temp.size());
	int dir = 0;
	size_t i = 0, j = GetNextPoint(&temp, &dir);
	while (temp.size() > 0)
	{
		poly[i++] = temp.at(j);
		temp.erase(temp.begin() + j);
		j = GetNextPoint(&temp, &dir);
	}

	return poly;
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
