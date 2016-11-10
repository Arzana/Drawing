#define _USE_CLIPPING
#define _VECT_CONV

#define vect(vrtx)		p->vrtx.v
#define clr(vrtx)		p->vrtx.c
#define vx(vrtx)		vect(vrtx).X
#define vy(vrtx)		vect(vrtx).Y
#define vz(vrtx)		vect(vrtx).Z
#define SORT_ARGS		(const Vertex *v0, const Vertex *v1)
#define SPEC_VEC(mask)	(bool(mask & HORIZONTAL) + bool(mask & VERTICAL) + bool(mask & DEPTH) > 1)

#include <vector>
#include "Utils.h"
#include "Shapes.h"

Vertex::Vertex(void)
	: v(), c()
{ }

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

bool Triangle::IsInside(const Vect3 v, Color * c)
{
	Vect2 *vs1 = &V3ToV2(&(v1.v - v0.v));
	Vect2 *vs2 = &V3ToV2(&(v2.v - v0.v));
	Vect2 *vs3 = &V3ToV2(&(v - v0.v));

	float r = Vect2::PrepDot(vs1, vs2);
	float s = Vect2::PrepDot(vs3, vs2) / r;
	float t = Vect2::PrepDot(vs1, vs3) / r;

	if (s >= 0 && t >= 0 && s + t <= 1)
	{
		float u = 1.0f - s - t;
		*c = v0.c * s + v1.c * t + v2.c * u;
		return true;
	}

	return false;
}

Rectangle::Rectangle(void)
	: x(0), y(0), w(0), h(0)
{ }

Rectangle::Rectangle(int x, int y, int w, int h)
	: x(x), y(y), w(w), h(h)
{ }

Polygon::Polygon(void)
	: v(NULL), polyVLen(0)
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

Vertex* TriangleClip(Triangle * p, int * len, const ViewPort vp)
{
	std::vector<Vertex> temp;
	SortVerticesBySpecial(&p->v0, &p->v1, &p->v2, vp);

	Vertex v;
	if (p->IsInside(v.v = Vect3(vp.screen.x, vp.screen.y, vp.near), &v.c)) temp.push_back(v);
	if (p->IsInside(v.v = Vect3(vp.screen.w, vp.screen.y, vp.near), &v.c)) temp.push_back(v);
	if (p->IsInside(v.v = Vect3(vp.screen.x, vp.screen.h, vp.near), &v.c)) temp.push_back(v);
	if (p->IsInside(v.v = Vect3(vp.screen.w, vp.screen.h, vp.near), &v.c)) temp.push_back(v);

	for (size_t i = 0; i < 3; i++)
	{
		Line *l = &p->GetLine(i);
		if (LineClip(l, vp))
		{
			if (!Contains(temp, &l->v0)) temp.push_back(l->v0);
			if (!Contains(temp, &l->v1)) temp.push_back(l->v1);
		}
	}

	if ((*len = temp.size()) == 0) return NULL;

	Vertex *poly = malloc_s(Vertex, *len);
	for (size_t i = 0; i < *len; i++)
	{
		poly[i] = temp.at(i);
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

void SortVerticesBySpecial(Vertex * v0, Vertex * v1, Vertex * v2, const ViewPort vp)
{
	std::vector<Vertex> mult;
	std::vector<Vertex> norm;

	int mask = ComputeMask(v0->v, vp);
	if (SPEC_VEC(mask)) mult.push_back(*v0);
	else norm.push_back(*v0);

	mask = ComputeMask(v1->v, vp);
	if (SPEC_VEC(mask))
	{
		if (mult.size() > 0)
		{
			Vertex v3 = mult.at(0);
			if (v3.v.Y <= v1->v.Y)
			{
				if (v3.v.Y == v1->v.Y && v3.v.X > v1->v.X) mult.insert(mult.begin(), *v1);
				else mult.push_back(*v1);
			}
			else mult.push_back(*v1);
		}
		else mult.push_back(*v1);
	}
	else norm.push_back(*v1);

	mask = ComputeMask(v2->v, vp);
	if (SPEC_VEC(mask))
	{
		if (mult.size() > 0)
		{
			Vertex v3 = mult.at(0);
			if (v3.v.Y <= v2->v.Y)
			{
				if (v3.v.Y == v2->v.Y && v3.v.X > v2->v.X) mult.insert(mult.begin(), *v2);
				else mult.push_back(*v2);
			}
			else mult.push_back(*v2);
		}
		else mult.push_back(*v2);
	}
	else norm.push_back(*v2);

	mult.insert(mult.end(), norm.begin(), norm.end());
	*v0 = mult.at(0);
	*v1 = mult.at(1);
	*v2 = mult.at(2);
}